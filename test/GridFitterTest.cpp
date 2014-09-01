/*
 * GridFitterTest.cpp
 *
 *  Created on: 21.08.2014
 *      Author: amjad
 */

#include "GridFitterTest.h"

using namespace boost::filesystem;
using namespace boost::archive;
using namespace cv;
using namespace decoder;

GridFitterTest::GridFitterTest() {
	// TODO Auto-generated constructor stub
}

GridFitterTest::~GridFitterTest() {
	// TODO Auto-generated destructor stub
}

void GridFitterTest::SetUp() {

}

void GridFitterTest::TearDown() {

}

TEST(GridFitterTest, TestGridPositions) {
	path ellipsesPath ("./test/data/grid_fitter/ellipses/");
	path gridsPath ("./test/data/grid_fitter/grids/");

	directory_iterator endIter;
	int i = 0;
	int wrongCount = 0;
	for (directory_iterator dirItr (ellipsesPath); dirItr != endIter; ++dirItr) {
		try {
			if (is_regular_file(dirItr->status())) {
				ifstream ellipseFile(dirItr->path().string().c_str());
				text_iarchive ellipseArchive(ellipseFile);
				vector<Ellipse> ellipses;
				ellipseArchive >> ellipses;
				ellipseFile.close();
				GridFitter fitter (i++, ellipses);
				vector<Grid> grids = fitter.process();

				path gridFilePath = gridsPath / dirItr->path().filename();

				ifstream gridFile(gridFilePath.string().c_str());
				text_iarchive gridArchive(gridFile);
				Grid g;
				gridArchive >> g;

				bool isRight = false;
				for (unsigned int j = 0; j < grids.size(); j++) {
					Grid grid = grids[j];
					if (abs(grid.x - g.x) <= 5 && abs(grid.y - g.y) <= 5 && abs(grid.angle - g.angle) <= 5) {
						isRight = true;
						break;
					}
				}

				if (!isRight) {
					wrongCount++;
				}
			}
		}
		catch (const std::exception & ex) {
			std::cout << dirItr->path().filename() << " " << ex.what() << std::endl;
		}
	}

	ASSERT_LE((float) wrongCount / i, 0.2);
}
