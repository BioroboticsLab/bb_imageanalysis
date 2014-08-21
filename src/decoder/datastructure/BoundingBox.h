/*
 * BoundingBox.h
 *
 *  Created on: 31.07.2014
 *      Author: mareikeziese
 */

#ifndef BOUNDINGBOX_H_
#define BOUNDINGBOX_H_

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <fstream>
#include <boost/serialization/vector.hpp>
#include <iostream>

using namespace cv;

namespace decoder {

class BoundingBox {
private:

	/**************************************
	 *
	 * 			members
	 *
	 **************************************/

	Rect _box;
	//needed to serialize all the private members
	friend class boost::serialization::access;

	/**************************************
	 *
	 * 			stuff
	 *
	 **************************************/

public:

	/**************************************
	 *
	 * 			constructor
	 *
	 **************************************/

	BoundingBox() {

	}

	BoundingBox(Rect bb) {
		this->_box = bb;
	}

	/**************************************
	 *
	 * 			getter/setter
	 *
	 **************************************/

	const Rect& getBox() const {
		return _box;
	}

	void setBox(const Rect& box) {
		_box = box;
	}

	/**************************************
	 *
	 * 			stuff
	 *
	 **************************************/

	bool isPossibleCenter(Point p, int tolerance) {

		int centerx = this->_box.x + this->_box.width / 2;
		int centery = this->_box.y + this->_box.height / 2;
		return (p.x >= (centerx - tolerance) && p.x <= (centerx + tolerance)
				&& p.y >= centery - tolerance && p.y <= (centery + tolerance));
	}

	/**
	 * serialization via boost
	 */
	template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
		ar & this->_box.x & this->_box.y & this->_box.width & this->_box.height;
	}

};

} /* namespace decoder */

#endif /* BOUNDINGBOX_H_ */
