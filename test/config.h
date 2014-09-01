/*
 * config.h
 *
 *  Created on: 30.07.2014
 *      Author: mareikeziese
 */

#ifndef TESTCONFIG_H_
#define TESTCONFIG_H_

#include <stdio.h>
#include <unistd.h>

using namespace std;

namespace testconfig {

/**************************************
 *
 * 			config for localizer-tests
 *
 **************************************/
const string TEST_ROI_PATH =
		"/Users/mareikeziese/git/bb_imganalysis/test/data/localizer_testdata/";
const string TEST_ROI_CONFIGS_PATH= "/Users/mareikeziese/git/bb_imganalysis/test/data/localizer_configs";
const string TEST_ROI_RESULT_PATH =
		"/Users/mareikeziese/git/bb_imganalysis/test/data/localizer_testresult/";
const bool TEST_ROI_USE_TESTCONFIGS = true;
const bool TEST_EXPORT_ROI_RESULT_BIGIMAGES = true;
const bool TEST_EXPORT_ROI_RESULT_SUBIMAGES = false;
const bool TEST_EXPORT_ROI_SHOW_SUCCESS = true;
const bool TEST_EXPORT_ROI_SHOW_NOTFOUND = true;
const bool TEST_EXPORT_ROI_SHOW_FAILED = true;
const int TEST_EXPORT_ROI_TOLERANCE = 30;


/**************************************
 *
 * 			config for recognizer-tests
 *
 **************************************/
const string TEST_RECOGNIZER_PATH =
		"/Users/mareikeziese/git/bb_imganalysis/test/data/recognizer_testdata/";

}

#endif /* CONFIG_H_ */
