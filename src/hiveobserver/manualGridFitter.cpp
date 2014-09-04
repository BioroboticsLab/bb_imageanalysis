/*
 * manualGridFitter.cpp
 *
 *  Created on: 02.09.2014
 *      Author: amjad
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../decoder/datastructure/Grid.h"

using namespace decoder;

// Manual Gridfitter ======
bool inRot = false;
bool inMove = false;
int prevY;
Grid grid;
string windowName = "Manual Grid Fit";
void mouseCallback(int event, int x, int y, int flags, void* userdata) {
	if (event == EVENT_LBUTTONDOWN && !inRot) {
		inMove = true;
	} else if (event == EVENT_LBUTTONUP) {
		Grid g (grid.size, grid.angle, 0, grid.x, grid.y, grid.ell);
		cout << "Score: " << g.score() << endl;
		inMove = false;
	} else if (event == EVENT_MOUSEMOVE && inMove) {
		grid.x = x;
		grid.y = y;
	} else if (event == EVENT_RBUTTONDOWN && !inMove) {
		prevY = y;
		inRot = true;
	} else if (event == EVENT_RBUTTONUP) {
		inRot = false;
	} else if (event == EVENT_MOUSEMOVE && inRot) {
		if (prevY - y > 0) {
			grid.angle -= 0.5;
		} else if (prevY - y < 0) {
			grid.angle += 0.5;
		}
		prevY = y;
	}
	imshow(windowName, grid.drawGrid());
}

/**
 * opens a window and gives ability to fit grid manually
 *
 * @param g the grid
 */
void manualGridFit(Grid g) {
	grid = g;
	cout << "Previous Score: " << g.score() << endl;
	namedWindow(windowName, WINDOW_NORMAL);

	setMouseCallback(windowName, mouseCallback, NULL);

	imshow(windowName, grid.drawGrid());

	while(waitKey() != 10); // Wait until enter is pressed
	destroyAllWindows();
}


