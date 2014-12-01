/*
 * DecodingProcess.h
 *
 *  Created on: 21.08.2014
 *      Author: mareikeziese
 */

#ifndef DECODINGPROCESS_H_
#define DECODINGPROCESS_H_
#include "decoder/Converter.h"
#include "decoder/Decoder.h"
#include "decoder/GridFitter.h"
#include "decoder/Localizer.h"
#include "decoder/Recognizer.h"
#include "decoder/Transformer.h"
#include "decoder/datastructure/Decoding.h"
#include "decoder/datastructure/Ellipse.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <boost/filesystem.hpp>
#include <iostream>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <unistd.h>
#include <vector>

using namespace boost::filesystem;
using namespace std;
using namespace cv;
using namespace decoder;

class DecodingProcess {
public:
    DecodingProcess() {}
    virtual ~DecodingProcess() {}

    void process(string filename) const;
};

#endif /* DECODINGPROCESS_H_ */

