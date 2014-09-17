/*
 * manualGridFitter.cpp
 *
 *  Created on: 02.09.2014
 *      Author: amjad
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem.hpp>

#include "../serialization.h"
#include "../decoder/datastructure/Grid.h"

using namespace decoder;
using namespace boost::filesystem;
using namespace boost::archive;

// Manual Gridfitter ======
bool inRot = false;
bool inMove = false;
int prevY;
string windowName = "Manual Grid Fit";
void mouseCallback(int event, int x, int y, int flags, void* userdata) {
	Grid* grid = (Grid*) userdata;

	if (event == EVENT_LBUTTONDOWN && !inRot) {
		inMove = true;
	} else if (event == EVENT_LBUTTONUP) {
		Grid g (grid->size, grid->angle, 0, grid->x, grid->y, grid->ell);

		cout << "Data:" << endl;
		cout << "Score: " << g.score() << " | x: " << g.x << " | y: " << g.y << " | angle: " << g.angle << endl << endl;

		inMove = false;
	} else if (event == EVENT_MOUSEMOVE && inMove) {
		grid->x = x;
		grid->y = y;
	} else if (event == EVENT_RBUTTONDOWN && !inMove) {
		prevY = y;
		inRot = true;
	} else if (event == EVENT_RBUTTONUP) {
		inRot = false;
	} else if (event == EVENT_MOUSEMOVE && inRot) {
		if (prevY - y > 0) {
			grid->angle -= 0.5;
		} else if (prevY - y < 0) {
			grid->angle += 0.5;
		}
		prevY = y;
	}
	imshow(windowName, grid->drawGrid());
}

/**
 * opens a window and gives ability to fit grid manually
 *
 * @param g the grid
 */
void manualGridFit(Grid &g) {
	cout << "Initial Data:" << endl;
	cout << "Score: " << g.score() << " | x: " << g.x << " | y: " << g.y << " | angle: " << g.angle << endl << endl;
	namedWindow(windowName, WINDOW_NORMAL);

	setMouseCallback(windowName, mouseCallback, &g);

	imshow(windowName, g.drawGrid());

	while(waitKey() != 10); // Wait until enter is pressed
	destroyAllWindows();
}

/**
 * main function
 */
int main(int argc, char** argv) {

	path gridsPath;
	if (argc < 2) {
		gridsPath = path("/Users/mareikeziese/git/bb_imganalysis/test/data/grid_fitter/grids/");
	} else {
		gridsPath = path(argv[1]);
		if (!is_directory(gridsPath)) {
			cerr << "You need to give a directory with serialized Grids!";
			return 1;
		}
	}

	// Search Grid files in dir
	directory_iterator endIter;
	for (directory_iterator dirItr (gridsPath); dirItr != endIter; ++dirItr) {
		if (is_regular_file(dirItr->status())) {
			// Load Grid from file
			ifstream gridFile(dirItr->path().string().c_str());
			text_iarchive gridArchive(gridFile);
			Grid g;
			gridArchive >> g;

			// Start manual GridFitter
			manualGridFit(g);
		}
	}

	return 0;
}


