/*
 * GridFitterTest.cpp
 *
 *  Created on: 21.08.2014
 *      Author: amjad
 */

#include "GridFitterTest.h"

GridFitterTest::GridFitterTest() {
	// TODO Auto-generated constructor stub

}

GridFitterTest::~GridFitterTest() {
	// TODO Auto-generated destructor stub
}

// TODO ich brauch noch nutzbare Gridpositionen and Winkel  als vergleich!!! Und ich sollte auch die Ellipsen filtern!!!
void GridFitterTest::SetUp() {
	// TODO Load serialized ellipses and Grid Positions and Angles
}

void GridFitterTest::TearDown() {
	// TODO Free serialized ellipses
}

TEST(GridFitterTest, TestGridPoisitions) {
	// TODO Testrahmen: 90% der Grids sollten schon nahe der Position sein, die erwartet wird (abweichung von einigen Pixeln und 0.5 bis Grad im Winkel könnte man ja druchgehen lassen)
}
