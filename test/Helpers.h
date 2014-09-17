/*
 * Helpers.h
 *
 *  Created on: 12.09.2014
 *      Author: mareikeziese
 */



#ifndef HELPERS_H_
#define HELPERS_H_

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <ostream>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#include "../src/decoder/Converter.h"
#include "../src/decoder/Localizer.h"
#include "../src/decoder/datastructure/TagList.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <boost/lexical_cast.hpp>
#include <boost/serialization/vector.hpp>
#include "../src/decoder/datastructure/BBList.h"
#include "../src/decoder/datastructure/Tag.h"
#include "gtest/gtest.h"
#include "config.h"
#include "../src/config.h"

using namespace boost::filesystem;
using namespace std;
using namespace decoder;
using namespace cv;





namespace TestHelpers {


BBList loadTags(string filename);

bool isPossibleCenter(Point p, Rect rec, int tolerance);

} /* namespace TestHelpers */


#endif /* HELPERS_H_ */
