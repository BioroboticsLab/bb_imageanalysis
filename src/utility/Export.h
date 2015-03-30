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
//#include <src/utility/serialization.h>
#include <ostream>
#include <iostream>
#include <fstream>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>


class Export {
public:
	Export();
	virtual ~Export();
	static void writeCSV(std::vector<pipeline::Tag> taglist, std::string exportfile);
	static void writeSerializedObjects(std::vector<pipeline::Tag> taglist, std::string exportfile);

};


#endif /* EXPORT_H_ */
