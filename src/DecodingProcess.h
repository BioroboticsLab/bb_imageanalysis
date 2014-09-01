/*
 * DecodingProcess.h
 *
 *  Created on: 21.08.2014
 *      Author: mareikeziese
 */

#ifndef DECODINGPROCESS_H_
#define DECODINGPROCESS_H_
#include <stdio.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include <math.h>
#include "decoder/Converter.h"
#include "decoder/Localizer.h"
#include "decoder/Recognizer.h"
#include "decoder/Transformer.h"
#include "decoder/datastructure/TagList.h"


using namespace std;
using namespace cv;
using namespace decoder;

class DecodingProcess {
public:
	DecodingProcess();
	virtual ~DecodingProcess();
	void process(string filename);
};

#endif /* DECODINGPROCESS_H_ */
