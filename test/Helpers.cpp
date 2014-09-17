/*
 * Helpers.cpp
 *
 *  Created on: 12.09.2014
 *      Author: mareikeziese
 */

#include "Helpers.h"

namespace TestHelpers {


BBList loadTags(string filename) {

	std::ifstream ifs(filename);
	boost::archive::text_iarchive ia(ifs);
	// read class state from archive
	BBList tags;
	ia & tags;
	return tags;

}

bool isPossibleCenter(Point p, Rect rec, int tolerance) {

	int centerx = rec.x + rec.width / 2;
	int centery = rec.y + rec.height / 2;
	return (p.x >= (centerx - tolerance) && p.x <= (centerx + tolerance)
			&& p.y >= centery - tolerance && p.y <= (centery + tolerance));
}


} /* namespace TestHelpers */
