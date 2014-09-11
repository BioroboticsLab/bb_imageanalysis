/**
 * serialization methods for necessary structures
 */

#include "decoder/datastructure/Ellipse.h"
#include "decoder/datastructure/Grid.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <iostream>
#include <fstream>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/split_free.hpp>

using namespace cv;
using namespace std;
using namespace decoder;

BOOST_SERIALIZATION_SPLIT_FREE(Mat)
namespace boost {
	namespace serialization {

		// Ellipses
		template <class Archive>
		void serialize(Archive &ar, Ellipse &ell, const unsigned int version) {
			ar & ell.vote;
			ar & ell.cen;
			ar & ell.angle;
			ar & ell.axis;
			ar & ell.transformedImage;
		}

		// Grid
		template <class Archive>
		void serialize(Archive &ar, Grid &g, const unsigned int version) {
			ar & g.size;
			ar & g.x;
			ar & g.y;
			ar & g.angle;
			ar & g.tilt;
			ar & g.permutation;
			ar & g.ell;
			ar & (g._score.metric);
			ar & g._score.value;
		}

		// Point2i (opencv)
		template <class Archive>
		void serialize(Archive &ar, Point2i &point, const unsigned int version) {
			ar & point.x;
			ar & point.y;
		}



		// Size (opencv)
		template <class Archive>
		void serialize(Archive &ar, Size &size, const unsigned int version) {
			ar & size.width;
			ar & size.height;
		}

		// Mat (opencv)
		// http://cheind.wordpress.com/2011/12/06/serialization-of-cvmat-objects-using-boost/

		//[>* Serialization support for cv::Mat <]
		template <class Archive>
		void save(Archive & ar, const Mat& m, const unsigned int version) {
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
		template <class Archive>
		void load(Archive & ar, Mat& m, const unsigned int version) {
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
