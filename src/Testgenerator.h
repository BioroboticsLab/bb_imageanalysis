/*
 * Testgenerator.h
 *
 *  Created on: 30.07.2014
 *      Author: mareikeziese
 */

#ifndef TESTGENERATOR_H_
#define TESTGENERATOR_H_

#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "config.h"
#include <boost/filesystem.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <fstream>
#include <boost/serialization/vector.hpp>
#include "BBList.h"


using namespace boost::filesystem;
using namespace std;
using namespace cv;
namespace decoder {

class Testgenerator {
public:
	Testgenerator();
	virtual ~Testgenerator();
};

} /* namespace decoder */

int redrawWindow(string windowName, Mat img);
void CallBackFunc(int event, int x, int y, int flags, void* userdata);

#endif /* TESTGENERATOR_H_ */


