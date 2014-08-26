/*
 * GridFitterTest.h
 *
 *  Created on: 21.08.2014
 *      Author: amjad
 */

#ifndef GRIDFITTERTEST_H_
#define GRIDFITTERTEST_H_

#include "gtest/gtest.h"
#include <boost/filesystem.hpp>
#include "../src/decoder/GridFitter.h"
#include "../src/decoder/datastructure/Grid.h"
#include "../src/decoder/datastructure/Ellipse.h"
#include "../src/serialization.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class GridFitterTest: public testing::Test {
public:
	GridFitterTest();
	virtual ~GridFitterTest();

	void SetUp();
	void TearDown();
};

#endif /* GRIDFITTERTEST_H_ */
