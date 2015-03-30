/*
 * Export.cpp
 *
 *  Created on: 30.03.2015
 *      Author: mareikeziese
 */

#include <src/utility/Export.h>

using namespace pipeline;



Export::Export() {
	// TODO Auto-generated constructor stub

}

Export::~Export() {
	// TODO Auto-generated destructor stub
}
/**
 *  writes a csv file with the following format:
 *			tmp_id 	: temporary Id, to determine, which rows belongs to the same detection
 *			x		: x- coordinate of ellipse
 *			y		: y-coordinate of the ellipse
 *			vote	: vote for the ellipse
 *			id		: decoded Id
 *
 *	there may be several (possible duplicated ) decoded IDs
 *
 * @param taglist
 */
void Export::writeCSV(std::vector<Tag>taglist, std::string exportfile) {

	boost::iostreams::stream_buffer<boost::iostreams::file_sink> buf(exportfile);
	std::ostream out(&buf);

	for (size_t i = 0; i < taglist.size(); i ++) {

		Tag tag = taglist[i];
		if(tag.isValid()){
		std::vector<TagCandidate>& candidates = tag.getCandidates();
		for (TagCandidate& candidate : candidates) {
			for (decoding_t const& decoding : candidate.getDecodings()) {
				out << i <<"," << candidate.getEllipse().getCen().x << ","
						<< candidate.getEllipse().getCen().y << ","
						<< candidate.getEllipse().getAngle() << ","
						<< candidate.getEllipse().getVote() << ","
						<< decoding.to_string() << std::endl;
			}
		}
		}

	}
}

void Export::writeSerializedObjects(std::vector<pipeline::Tag> taglist, std::string exportfile){
	std::ofstream ofs(exportfile);
	boost::archive::binary_oarchive oa(ofs);
				//boost::archive::text_oarchive oa(ofs);
				oa & taglist;
}

std::vector<pipeline::Tag> Export::readSerializedObjects(std::string exportfile){
	std::vector<pipeline::Tag> taglist = std::vector<pipeline::Tag>() ;
	/*std::ifstream ifs(exportfile);
	boost::archive::binary_iarchive ia(ifs);
				//boost::archive::text_oarchive oa(ofs);
				ia & taglist;*/
				return taglist;
}

