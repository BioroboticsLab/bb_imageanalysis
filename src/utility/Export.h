/*
 * Export.h
 *
 *  Created on: 30.03.2015
 *      Author: mareikeziese
 */

#pragma once

#include <string>
#include <vector>

namespace pipeline {
class Tag;
}

class Export {
public:
	/**
	 *  writes a csv file with the following format:
	 *			tmp_id 	: temporary Id, to determine, which rows belongs to the same detection
	 * 			angle   : z- rotation of the Grid
	 *			x		: x- coordinate of the Grid
	 *			y		: y- coordinate of the Grid
	 *			vote	: vote for the TagCandidate
	 *			id		: decoded Id
	 *
	 *	there may be several (possible duplicated) decoded IDs
	 *
	 * @param taglist
	 */
	static void writeCSV(std::vector<pipeline::Tag> const& taglist, std::string const& path);

	static void writeSerializedObjects(std::vector<pipeline::Tag> const& taglist, std::string const& path);

	static std::vector<pipeline::Tag> readSerializedObjects(std::string const& path);
};
