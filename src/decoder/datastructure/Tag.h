/*
 * Tag.h
 *
 *  Created on: 17.08.2014
 *      Author: mareikeziese
 */

#ifndef TAG_H_
#define TAG_H_

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <fstream>
#include <iostream>
#include "BoundingBox.h"
#include "TagCandidate.h"

using namespace std;
using namespace boost;
using namespace cv;

namespace decoder {

class Tag {
private:

	/**************************************
	 *
	 * 			members
	 *
	 **************************************/

	BoundingBox _boundingBox;

	Mat _origSubImage;

	Mat _cannySubImage;

	//marks if the tag is really a tag;
	bool _valid;

	//virtual id, just necessary for the decoding process;
	int id;

	//there may be multiple ellipses and grids for this location, so there is a list of candidates
	vector<TagCandidate> _candidates;

	//needed to serialize all the private members
	friend class boost::serialization::access;

	//needed to serialize class implicit
	template<class Archive> void serialize(Archive & ar,
			const unsigned int version);

public:

	Tag();
	Tag(Rect rec);
	virtual ~Tag();

	/**************************************
	 *
	 * 			getter/setter
	 *
	 **************************************/
	const BoundingBox& getBoundingBox() const;
	void setBoundingBox(const BoundingBox& boundingBox);
	const vector<TagCandidate>& getCandidates() const;
	void setCandidates(const vector<TagCandidate>& candidates);
	const Mat& getCannySubImage() const;
	void setCannySubImage(const Mat& cannySubImage);
	const Mat& getOrigSubImage() const;
	void setOrigSubImage(const Mat& origSubImage);
	bool isValid() const;
	void setValid(bool valid);
	int getId() const;
	void setId(int id);
	void addCandidate(TagCandidate c);
};

} /* namespace decoder */

// needed to be included for the function template
//#include "Tag.cpp"
#endif /* TAG_H_ */
