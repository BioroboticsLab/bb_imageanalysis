#pragma once

#include <string>
#include <vector>

namespace pipeline {
class Tag;
}

class Export {
public:
	/**
	 * writes a csv file with the following format:
	 *         tagIdx       : unique sequential id of the tag
	 *         candidateIdx : sequential id of the candidate per tag
	 *         gridIdx      : sequential id of the grid/decoding per candidate
	 *         xpos         : x coordinate of the grid center
	 *         ypos         : y coordinate of the grid center
	 *         xRotation    : rotation of the grid in x plane
	 *         yRotation    : rotation of the grid in y plane
	 *         zRotation    : rotation of the grid in z plane
	 *         vote         : candidate score
	 *         id           : decoded id
	 *
	 * there may be several (possible duplicated) decoded IDs
	 *
	 * @param taglist
	 */
	static void writeCSV(std::vector<pipeline::Tag> const& taglist, std::string const& path);

	static void writeSerializedObjects(std::vector<pipeline::Tag> const& taglist, std::string const& path);

	static std::vector<pipeline::Tag> readSerializedObjects(std::string const& path);
};
