/*
 * Localizer.h
 *
 *  Created on: 03.07.2014
 *      Author: mareikeziese
 */

#ifndef LOCALIZER_H_
#define LOCALIZER_H_

#include <stdio.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <math.h>
#include <fstream>

using std;
using namespace cv;

namespace decoder {

class Localizer {
public:
	Localizer();
	virtual ~Localizer();

	void process(cv::Mat image);

private:
/**
 *
 */

	/**
	 * gray scale comb image
	 */
	//cv::Mat cannyEdgeMap;
	 //boundingBoxes;

	/** Canny lower threshold */
	static int const lowerCannyThreshold = 30;
	/** Canny higher threshold \see  */
	static int const higherCannyThreshold = 60;
	/** binarization threshold for tag locating \see highlightTags */
	static int const lbin = 60;
	/** number of erosions \see highlightTags */
	static int const numberOfErosions = 1;
	/** number of dilations \see highlightTags */
	static int const numberOfDilations = 10;
	/** maximal size of a possible tag */
	static int const maxTagSize = CV_PI*54*54;


	Mat computeSobelMap(Mat grayImage);
	Mat computeBlobs(Mat sobel);
	Mat highlightTags(Mat &grayImage);
	vector<Rect> locateTagCandidates(Mat blobImage);
	cv::Mat Localizer::computeCannyEdgeMap(Mat grayImage);




};

} /* namespace decoder */

#endif /* LOCALIZER_H_ */
