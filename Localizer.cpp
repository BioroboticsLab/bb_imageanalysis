/*
 * Localizer.cpp
 *
 *  Created on: 03.07.2014
 *      Author: mareikeziese
 */

#include "Localizer.h"

namespace decoder {

Localizer::Localizer() {
	// TODO Auto-generated constructor stub

}

Localizer::~Localizer() {
	// TODO Auto-generated destructor stub
}


void Localizer::process(cv::Mat grayImage) {

	Mat sobel, blob;

	// compute the sobel derivative first
	sobel = this->computeSobelMap(grayImage);

	// and then locate the tags using the sobel map
	blob = this->computeBlobs(sobel);

	// compute canny edge map. Needed for ellipse detection but needs to be done only once per image.
	vector<Rect> cannyEdgeMap = this->computeCannyEdgeMap(grayImage);

}


/**
 * Highlight tag candidates in a comb image by intensity values
 *
 * @param grayImage
 * @return image with highlighted tags
 */
Mat Localizer::highlightTags(Mat &grayImage) {

	Mat imageCopy;
	//eroded image
	Mat erodedImage;
	//dilated image
	Mat dilatedImage;
	Mat binarizedImage;

	//binarization
	threshold(grayImage, binarizedImage, this->lbin, 255, CV_THRESH_BINARY);

	binarizedImage.copyTo(imageCopy);

	//erosion
	erodedImage = getStructuringElement(MORPH_ELLIPSE,
			Size(2 * this->numberOfErosions + 1,
					2 * this->numberOfErosions + 1),
			Point(this->numberOfErosions, this->numberOfErosions));
	erode(imageCopy, imageCopy, erodedImage);

	//dilation
	dilatedImage = getStructuringElement(MORPH_ELLIPSE,
			Size(2 * this->numberOfDilations + 1,
					2 * this->numberOfDilations + 1),
			Point(this->numberOfDilations, this->numberOfDilations));
	dilate(imageCopy, imageCopy, dilatedImage);

	return imageCopy;
}

/**
 *  Find blobs in the binary input image Ib and filter them by their size
 *
 * @param blobImage binary comb image with highlighted tag candidates
 * @return boundingBoxes output vector of size-filtered bounding boxes
 */

vector<Rect> Localizer::locateTagCandidates(Mat blobImage) {

	vector<Rect> boundingBoxes;
	vector<vector<Point2i> > contours;

	boundingBoxes = vector<Rect>();

	//find intra-connected white pixels
	cv::findContours(blobImage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	//extract contour bounding boxes for tag candidates
	for (vector<vector<Point2i> >::iterator contour = contours.begin();
			contour != contours.end(); ++contour) {

		//filter contours which are too big
		if (contour->size() < this->maxTagSize) {

			Rect rec = boundingRect(*contour) * 2;

			if ((rec.x + rec.width) > blobImage.cols) {
				rec.x -= abs(rec.x + rec.width - blobImage.cols);
			}

			if ((rec.y + rec.height) > blobImage.rows) {
				rec.y -= abs(rec.y + rec.height - blobImage.rows);
			}

			if (rec.x < 0) {
				rec.x = 0;
			}

			if (rec.y < 0) {
				rec.y = 0;
			}

			if ((rec.height * rec.width) > 800
					&& (rec.height * rec.width) < 20000) {
				boundingBoxes.push_back(rec);
			}
		}
	}

	return boundingBoxes;
}

/**
 * Computes the Sobel map for a given grayscale image.
 * @return sobelmap
 */
Mat Localizer::computeSobelMap(Mat grayImage) {

	Mat sobel;
	Mat imageCopy;
	// We need a copy because the GuassianBlur makes changes to the image
	grayImage.copyTo(imageCopy);

	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	cv::GaussianBlur(imageCopy, imageCopy, Size(7, 7), 0, 0, BORDER_DEFAULT);

	/// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	/// Gradient X
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	cv::Sobel(imageCopy, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	cv::convertScaleAbs(grad_x, abs_grad_x);

	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	cv::Sobel(imageCopy, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	cv::convertScaleAbs(grad_y, abs_grad_y);

	/// Total Gradient (approximate)
	cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, sobel);

	return sobel;

	//DEBUG_IMSHOW( "sobel", sobel );
}

/*
 Computes Blobs and finally finds the ROI's using the sobel map. The ROI's are stored in the boundingBoxes Vector.
 */

Mat Localizer::computeBlobs(Mat sobel) {

	Mat blob;
	blob = this->highlightTags(sobel);

	//DEBUG_IMSHOW("blob", blob);

	vector<Rect> boundingBoxes = this->locateTagCandidates(blob);

//#ifdef _DEBUG
//	image.copyTo(output);
//
//	for ( unsigned int i = 0; i < boundingBoxes.size(); i++) {
//
//		cv::rectangle(output, boundingBoxes[i], Scalar(0, 0, 255), 3);
//	}
//#endif
	return blob;
}

/**
 *
 *
 *
 * @param grayImage
 * @return
 */
Mat Localizer::computeCannyEdgeMap(Mat grayImage) {
	Mat localGrayImage;
	grayImage.copyTo(localGrayImage);
	cv::GaussianBlur(localGrayImage, localGrayImage, Size(3, 3), 0, 0,
			BORDER_DEFAULT);

	Mat cannyEdgeMap;
	Canny(localGrayImage, cannyEdgeMap, this->lowerCannyThreshold,
			this->higherCannyThreshold);

	return cannyEdgeMap;
	//DEBUG_IMSHOW( "cannyEdgeMap", cannyEdgeMap );
}

/**
 *  Scales a given OpenCV rectangle by a factor, conserving the rectangle's center.
 *
 * @param rectangle OpenCV rectangle to be scaled
 * @param scale  factor by which the rectangle is scaled
 * @return
 */
Rect operator*(const Rect rectangle, double scale) {
	Size s = Size((rectangle.height * scale), (rectangle.width * scale));
	Point2i c = Point(rectangle.x - (0.5 * (s.width - rectangle.width)),
			rectangle.y - (0.5 * (s.height - rectangle.height)));
	return (Rect(c, s));
}

} /* namespace decoder */
