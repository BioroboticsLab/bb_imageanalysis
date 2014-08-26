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

GridFitterTest::GridFitterTest() {
	// TODO Auto-generated constructor stub
}

GridFitterTest::~GridFitterTest() {
	// TODO Auto-generated destructor stub
}

// TODO ich brauch noch nutzbare Gridpositionen and Winkel  als vergleich!!! Und ich sollte auch die Ellipsen filtern!!!
void GridFitterTest::SetUp() {

}

void GridFitterTest::TearDown() {
	// TODO Free serialized ellipses
}

TEST(GridFitterTest, TestGridPositions) {
	// TODO Testrahmen: 90% der Grids sollten schon nahe der Position sein, die erwartet wird (abweichung von einigen Pixeln und 0.5 bis Grad im Winkel könnte man ja druchgehen lassen)
		path full_path("./test/data/grid_fitter_ellipses/");

		directory_iterator end_iter;
		for (directory_iterator dir_itr(full_path); dir_itr != end_iter; ++dir_itr) {
			try {
				if (is_regular_file(dir_itr->status())) {
					ifstream file(dir_itr->path().string().c_str());
					text_iarchive ia(file);
					vector<Ellipse> ellipses;
					ia >> ellipses;
					file.close();
					for (unsigned int i = 0; i < ellipses.size(); i++) {
						stringstream ss;
						ss << dir_itr->path().filename() << " " << i;
						namedWindow(ss.str());
						imshow(ss.str(), ellipses[i].transformedImage);
					}
					while (waitKey() != 10);
					destroyAllWindows();
		        }
			}
			catch (const std::exception & ex) {
				std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
			}
		}
}
