/*
 * Export.h
 *
 *  Created on: 30.03.2015
 *      Author: mareikeziese
 */

#ifndef EXPORT_H_
#define EXPORT_H_

#include <vector>
#include <src/pipeline/datastructure/Tag.h>
#include <src/pipeline/datastructure/TagCandidate.h>
#include <src/pipeline/datastructure/PipelineGrid.h>
#include <ostream>
#include <iostream>
#include <fstream>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/split_free.hpp>



class Export {
public:
	Export();
	virtual ~Export();
	static void writeCSV(std::vector<pipeline::Tag> taglist, std::string exportfile);
	static void writeSerializedObjects(std::vector<pipeline::Tag> taglist, std::string exportfile);
	static std::vector<pipeline::Tag> readSerializedObjects(std::string exportfile);

};


#endif /* EXPORT_H_ */
