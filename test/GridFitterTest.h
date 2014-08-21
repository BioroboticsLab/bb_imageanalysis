/*
 * GridFitterTest.h
 *
 *  Created on: 21.08.2014
 *      Author: amjad
 */

#ifndef GRIDFITTERTEST_H_
#define GRIDFITTERTEST_H_

#include "gtest/gtest.h"

class GridFitterTest: public testing::Test {
public:
	GridFitterTest();
	virtual ~GridFitterTest();

	void SetUp();
	void TearDown();
};

#endif /* GRIDFITTERTEST_H_ */
