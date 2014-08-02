/*
 * BoundingBoxesGenerator.cpp
 *
 *  Created on: 28.07.2014
 *      Author: mareikeziese
 */

#include "BoundingBoxesGenerator.h"

BoundingBoxesGenerator::BoundingBoxesGenerator() {
	// TODO Auto-generated constructor stub

}

BoundingBoxesGenerator::~BoundingBoxesGenerator() {
	// TODO Auto-generated destructor stub
}

void showBBOverview(vector<BoundingBox> bb, Mat image) {

	Mat output;

	int positive;

	for (unsigned int i = 0; i < bb.size(); i++) {

		image.copyTo(output);

		Point x1 = Point(bb[i].box_.x, bb[i].box_.y);
		Point x2 = Point(bb[i].box_.x + bb[i].box_.width, bb[i].box_.y);
		Point y1 = Point(bb[i].box_.x, bb[i].box_.y + bb[i].box_.height);
		Point y2 = Point(bb[i].box_.x + bb[i].box_.width,
				bb[i].box_.y + bb[i].box_.height);

		cv::line(output, x1, x2, Scalar(255, 0, 0), 3);
		cv::line(output, x1, y1, Scalar(255, 0, 0), 3);
		cv::line(output, y1, y2, Scalar(255, 0, 0), 3);
		cv::line(output, x2, y2, Scalar(255, 0, 0), 3);

		string text = "Box " + boost::lexical_cast<std::string>(i);
		cv::putText(output, text, x1, FONT_HERSHEY_COMPLEX_SMALL, 1.5,
				Scalar(255, 0, 0));

		namedWindow("Result", WINDOW_NORMAL);
		imshow("Result", output);
		int keyst = waitKey(0);
		destroyWindow("Result");
		//output.copyTo(outputcp);

		Mat subimage(output, bb[i].box_ );
		namedWindow("Result " + boost::lexical_cast<std::string>(i),
				WINDOW_NORMAL);
		imshow("Result " + boost::lexical_cast<std::string>(i), subimage);
		keyst = waitKey(0);
		if (keyst == 121) {
			cout << "BB " << i << " accepted ";

		}

		destroyWindow("Result " + boost::lexical_cast<std::string>(i));

	}

}

int main(int argc, char** argv) {

	char* image_name = argv[1];

	path image_file(image_name);
	if (!exists(image_file) || !is_regular_file(image_file)) {
		cerr << "given image does not exist";
		return 0;
	}

	Converter conv = Converter();
	cv::Mat gray_image = conv.process(image_name);

	Localizer localizer = Localizer();
	vector<BoundingBox> bb = localizer.process(gray_image);

	cout << bb.size() << " Boxes found " << endl;

	showBBOverview(bb, gray_image);

//
//		for ( unsigned int i = 0; i < bb.size(); i++) {
//
//		cout << "BoundingBox " << i << endl;
//		Mat subImage(gray_image, bb[i].box_);
//		imshow("BoundingBox", subImage);
//
//
//		}

	cout << "done";

}

