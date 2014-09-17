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
/**
 * @var path, where testdata is placed (generated with the testgenerator)
 */
const string TEST_ROI_PATH =
		"/Users/mareikeziese/git/bb_imganalysis/test/data/localizer_testdata/";

/**
 * @var path, where the results should be placed in (with ending backslash )
 */
const string TEST_ROI_RESULT_PATH =
		"/Users/mareikeziese/git/bb_imganalysis/test/data/localizer_testresult/";

/**
 * @var if the test should test with several test-configs, placed in config_folder
 */
const bool TEST_ROI_USE_TESTCONFIGS = true;

/**
 * @var folder with configs, that the test shall evaluate
 */
const string TEST_ROI_CONFIGS_PATH =
		"/Users/mareikeziese/git/bb_imganalysis/test/data/localizer_configs";

/**
 * @var if the test should export a big image, where all results are shown
 */
const bool TEST_EXPORT_ROI_RESULT_BIGIMAGES = true;

/**
 * @var if the test should export subimages (for each bounding box)
 */
const bool TEST_EXPORT_ROI_RESULT_SUBIMAGES = false;

/**
 * @var if the test should export roi's, that are successfully found
 */
const bool TEST_EXPORT_ROI_SHOW_SUCCESS = true;

/**
 * @var if the test should export roi's, that are not found
 */
const bool TEST_EXPORT_ROI_SHOW_NOTFOUND = true;

/**
 * @var if the test should export roi's, that are erroneously found
 */
const bool TEST_EXPORT_ROI_SHOW_FAILED = true;

/**
 * @var amount of pixels, that the center of the bounding box may differ from the position, that was clicked manually
 */
const int TEST_EXPORT_ROI_TOLERANCE = 30;

/**************************************
 *
 * 			config for recognizer-tests
 *
 **************************************/

/**
 * @var path, where testdata is placed (generated with the testgenerator)
 */
const string TEST_RECOGNIZER_PATH =
		"/Users/mareikeziese/git/bb_imganalysis/test/data/recognizer_testdata/";

/**
 * @var  path, where the results should be placed in (with ending backslash )
 */
const string TEST_RECOGNIZER_RESULT_PATH =
		"/Users/mareikeziese/git/bb_imganalysis/test/data/recognizer_testresult/";

/**
 * @var if the test should test with several test-configs, placed in config_folder
 */
const bool TEST_RECOGNIZER_USE_TESTCONFIGS = true;


/**
 * @var folder with configs, that the test shall evaluate
 */
const string TEST_RECOGNIZER_CONFIGS_PATH =
		"/Users/mareikeziese/git/bb_imganalysis/test/data/recognizer_configs";

/**
 * @var if the test should export a big image, where all results are shown
 */
const bool TEST_EXPORT_RECOGNIZER_RESULT_BIGIMAGES = true;


/**
 * @var if the test should export subimages (for each bounding box)
 */
const bool TEST_EXPORT_RECOGNIZER_RESULT_SUBIMAGES = true;

/**
 * @var if the test should export RECOGNIZER's, that are successfully found
 */
const bool TEST_EXPORT_RECOGNIZER_SHOW_SUCCESS = true;

/**
 * @var if the test should export RECOGNIZER's, that are not found
 */
const bool TEST_EXPORT_RECOGNIZER_SHOW_NOTFOUND = true;

/**
 * @var if the test should export RECOGNIZER's, that are erroneously found
 */
const bool TEST_EXPORT_RECOGNIZER_SHOW_FAILED = true;

}

#endif /* CONFIG_H_ */
