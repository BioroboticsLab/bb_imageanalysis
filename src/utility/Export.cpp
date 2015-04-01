/*
 * Export.cpp
 *
 *  Created on: 30.03.2015
 *      Author: mareikeziese
 */

#include "Export.h"

#include <iostream>
#include <fstream>
#include <ostream>

#include <boost/format.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
//#include <boost/archive/binary_oarchive.hpp>
//#include <boost/archive/binary_iarchive.hpp>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <src/pipeline/datastructure/Tag.h>
#include <src/pipeline/datastructure/TagCandidate.h>
#include <src/pipeline/datastructure/PipelineGrid.h>

void Export::writeCSV(std::vector<pipeline::Tag> const& taglist, std::string const& path)
{
	boost::iostreams::stream_buffer<boost::iostreams::file_sink> buf(path);
	std::ostream out(&buf);

	boost::format csvLine("%1%,%2%,%3%,%4%,%5%,%6%,%7%,%8%,%9%,%10%");

	for (size_t tagIdx = 0; tagIdx < taglist.size(); ++tagIdx) {
		pipeline::Tag const& tag = taglist[tagIdx];
		if (tag.isValid()) {
			const auto& candidates = tag.getCandidatesConst();

			for (size_t candidateIdx = 0; candidateIdx < candidates.size(); ++candidateIdx) {
				const pipeline::TagCandidate& candidate = candidates[candidateIdx];

				auto const& decodings = candidate.getDecodings();
				auto const& grids     = candidate.getGridsConst();
				assert(grids.size() == decodings.size());

				for (size_t gridIdx = 0; gridIdx < grids.size(); ++gridIdx) {
					PipelineGrid const& grid             = grids[gridIdx];
					pipeline::decoding_t const& decoding = decodings[gridIdx];

					csvLine % tagIdx % candidateIdx % gridIdx
					        % grid.getCenter().x % grid.getCenter().y
					        % grid.getXRotation() % grid.getYRotation() % grid.getZRotation()
					        % candidate.getEllipse().getVote()
					        % decoding.to_string();

					out << csvLine << std::endl;
				}
			}
		}
	}
}

void Export::writeSerializedObjects(const std::vector<pipeline::Tag> &taglist,
                                    const std::string &path)
{
	std::ofstream ofs(path);

//	boost::archive::text_oarchive oa(ofs);
//	boost::archive::binary_oarchive oa(ofs);
	boost::archive::xml_oarchive oa(ofs);

	oa & BOOST_SERIALIZATION_NVP(taglist);
}

std::vector<pipeline::Tag> Export::readSerializedObjects(const std::string &path)
{
	std::ifstream ifs(path);

//	boost::archive::text_oarchive oa(ofs);
//	boost::archive::binary_iarchive ia(ifs);
	boost::archive::xml_iarchive ia(ifs);

	std::vector<pipeline::Tag> taglist;
	ia & BOOST_SERIALIZATION_NVP(taglist);

	return taglist;
}
