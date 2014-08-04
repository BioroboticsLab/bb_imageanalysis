#ifndef CONFIG_H_
#define CONFIG_H_


#include <stdio.h>
#include <unistd.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

using namespace std;

namespace config{

const int DEBUG_MODE = false;

const string APPlICATION_ENVIROMENT = "development";


const string DEFAULT_LOCALIZER_CONFIG = "/Users/mareikeziese/git/bb_imganalysis/src/decoder/localizer.ini";

/******************************************
 *
 *  Config Localizer
 *
 ******************************************/


/**
 * Lower Threshold for Canny
 */
const int LOCALIZER_LCANNYTHRES = 30;
/**
 * Higher Threshold for Canny
 */
const int LOCALIZER_HCANNYTHRES = 60;
/**
 * Threshold for binarisation
 */
const int LOCALIZER_BINTHRES = 29;

/**
 * Number of erosions
 */
const int LOCALIZER_EROSIONNUM_1 = 25;


/**
 * Number of dilations
 */
const int LOCALIZER_DILATIONNUM_1 = 3;
/**
 * Number of erosions
 */
const int LOCALIZER_EROSIONNUM = 5;


/**
 * Number of dilations
 */
const int LOCALIZER_DILATIONNUM = 10;
/**
 *maximal size of a possible tag
 */
const int LOCALIZER_MAXTAGSIZE =  250;//CV_PI*54*54;
/**
 *  minimal size of bounding box
 */
const int LOCALIZER_MINTAGSIZE =  100;//CV_PI*54*54;


/******************************************
 *
 *  Config Testgenerator
 *
 ******************************************/

const string TEST_DATA_EXPORT_PATH = "/Users/mareikeziese/git/bb_imganalysis/test/data/roi/";

}

#endif /*CONFIG_H_ */
