/**
 * serialization methods for necessary structures
 */

#include <src/pipeline/datastructure/Tag.h>
#include <src/pipeline/datastructure/TagCandidate.h>
#include <src/pipeline/datastructure/Ellipse.h>
#include <src/pipeline/datastructure/PipelineGrid.h>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <iostream>
#include <fstream>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/split_free.hpp>

using namespace pipeline;

BOOST_SERIALIZATION_SPLIT_FREE(cv::Mat)
namespace boost {
namespace serialization {


// Tag
template<class Archive>
void serialize(Archive &ar, Tag &t, const unsigned int version) {
	ar & t.getBox();
	ar & t.getCandidates();
}

// TagCandidate
template<class Archive>
void serialize(Archive &ar, TagCandidate &t, const unsigned int version) {
	ar & t.getEllipse();
	ar & t.getGrids();
	ar & t.getDecodings();

}


// Ellipses
template<class Archive>
void serialize(Archive &ar, pipeline::Ellipse &ell, const unsigned int version) {
	ar & ell._vote;
	ar & ell._cen;
	ar & ell._angle;
	ar & ell._axis;
}



// Decoding
template<class Archive>
void serialize(Archive &ar, decoding_t &d, const unsigned int version) {
	//ar & d.to_string();

}


// Grid
template<class Archive>
void serialize(Archive &ar, PipelineGrid &g, const unsigned int version) {
	/*ar & g.getBoundingBox();
	ar & g.getCenter();
	ar & g.getRadius();
	ar & g.getXRotation();
	ar & g.getYRotation();
	ar & g.getZRotation();
	ar & g.getOuterRingEdgeCoordinates();*/
}


// Rect (opencv)
template<class Archive>
void serialize(Archive &ar, const cv::Rect &rec, const unsigned int version) {
	ar & rec.height;
	ar & rec.width;
	ar & rec.x;
	ar & rec.y;
}

// Point2i (opencv)
template<class Archive>
void serialize(Archive &ar, const cv::Point2i &point, const unsigned int version) {
	ar & point.x;
	ar & point.y;
}

// Size (opencv)
template<class Archive>
void serialize(Archive &ar, const cv::Size &size, const unsigned int version) {
	ar & size.width;
	ar & size.height;
}



// Mat (opencv)
// http://cheind.wordpress.com/2011/12/06/serialization-of-cvmat-objects-using-boost/

//[>* Serialization support for cv::Mat <]
template<class Archive>
void save(Archive & ar, const cv::Mat& m, const unsigned int version) {
	size_t elem_size = m.elemSize();
	size_t elem_type = m.type();

	ar & m.cols;
	ar & m.rows;
	ar & elem_size;
	ar & elem_type;

	const size_t data_size = m.cols * m.rows * elem_size;
	ar & make_array(m.ptr(), data_size);
}

//[>* Serialization support for cv::Mat <]
template<class Archive>
void load(Archive & ar, cv::Mat& m, const unsigned int version) {
	int cols, rows;
	size_t elem_size, elem_type;

	ar & cols;
	ar & rows;
	ar & elem_size;
	ar & elem_type;

	m.create(rows, cols, elem_type);

	size_t data_size = m.cols * m.rows * elem_size;
	ar & make_array(m.ptr(), data_size);
}
}
}
