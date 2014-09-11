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
#include "../src/config.h"
#include <boost/filesystem.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <fstream>
#include <boost/serialization/vector.hpp>
#include "../src/serialization.h"
#include "../src/decoder/datastructure/TagList.h"
#include <boost/program_options.hpp>
#include <iterator>

using namespace boost::filesystem;
using namespace std;
using namespace cv;
using namespace decoder;
namespace po = boost::program_options;

namespace testgenerator {

bool isPossibleCenter(Point p,Rect box, int tolerance);
vector<string> getAllFilesInFolder(string folder);
string generateLocalizerData(string filename);
int LocalizerRedrawWindow(string windowName, Mat img);
void LocalizerMouseCallback(int event, int x, int y, int flags, void* userdata);

int RecognizerRedrawWindow(string windowName, Mat img);
void RecognizerMouseCallback(int event, int x, int y, int flags, void* userdata);


string generateRecognizerData(string localizer_testdata_original_image, bool use_test_configs);

void exportLocalizerTestResults(TagList found, TagList wrongFound,
		vector<int> notFound, Mat exportImage, path te_dir_tmp);
void _exportLocalizerTestResults_helper(TagList tags, Mat &exportImage,
		path exportPath, bool exportBigImage, bool exportSubImage,
		Scalar color);



} /* namespace testgenerator */
#endif /* TESTGENERATOR_H_ */

