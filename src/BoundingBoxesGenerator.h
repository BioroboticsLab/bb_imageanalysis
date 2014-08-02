/*
 * BoundingBoxesGenerator.h
 *
 *  Created on: 28.07.2014
 *      Author: mareikeziese
 */

#ifndef BOUNDINGBOXESGENERATOR_H_
#define BOUNDINGBOXESGENERATOR_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <boost/filesystem.hpp>
#include "decoder/Converter.h"
#include "decoder/Localizer.h"
#include "decoder/BoundingBox.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <math.h>
#include <boost/lexical_cast.hpp>

using namespace boost::filesystem;
using namespace std;
using namespace decoder;
using namespace cv;

class BoundingBoxesGenerator {
public:
	BoundingBoxesGenerator();
	virtual ~BoundingBoxesGenerator();

};

#endif /* BOUNDINGBOXESGENERATOR_H_ */
