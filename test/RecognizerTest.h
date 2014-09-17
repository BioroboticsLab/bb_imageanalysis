/*
 * LocalizerTest.h
 *
 *  Created on: 31.07.2014
 *      Author: mareikeziese
 */

#ifndef RECOGNIZERTEST_H_
#define RECOGNIZERTEST_H_


#include <iostream>
#include <boost/filesystem.hpp>
#include <ostream>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#include "../src/decoder/Converter.h"
#include "../src/decoder/Localizer.h"
#include "../src/decoder/Recognizer.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <boost/lexical_cast.hpp>
#include <boost/serialization/vector.hpp>
#include <time.h>
#include "../src/decoder/datastructure/BBList.h"
#include "../src/decoder/datastructure/Tag.h"
#include "gtest/gtest.h"
#include "config.h"
#include "../src/config.h"
#include "Helpers.h"

using namespace boost::filesystem;
using namespace std;
using namespace decoder;
using namespace cv;

class RecognizerTest: public testing::Test {
public:
	RecognizerTest();
	virtual ~RecognizerTest();

	void SetUp();
	void TearDown();
};

//namespace io = boost::iostreams;



#endif /* RECOGNIZERTEST_H_ */
