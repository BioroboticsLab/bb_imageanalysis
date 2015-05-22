#include <vector>

#include <boost/filesystem.hpp>

#include "catch.hpp"

#include "src/pipeline/Preprocessor.h"
#include "src/pipeline/Localizer.h"
#include "src/pipeline/EllipseFitter.h"
#include "src/pipeline/GridFitter.h"
#include "src/pipeline/Decoder.h"

TEST_CASE("Pipeline finishes with test data and returns results", "[integration]") {
	pipeline::Preprocessor preprocessor;
	pipeline::Localizer localizer;
	pipeline::EllipseFitter ellipseFitter;
	pipeline::GridFitter gridFitter;
	pipeline::Decoder decoder;

	for (boost::filesystem::recursive_directory_iterator end, dir(TEST_DATA_PATH); dir != end; ++dir ) {
		if (boost::filesystem::is_regular_file(*dir)) {
			if (boost::filesystem::extension(*dir) == ".jpeg") {
				INFO(*dir);

				cv::Mat img = cv::imread((*dir).path().string());
				cv::Mat preprocessedImg;
				std::vector<pipeline::Tag> taglist;
				preprocessedImg = preprocessor.process(img);
				REQUIRE(!preprocessedImg.empty());

				taglist = localizer.process(std::move(img), std::move(preprocessedImg));
				REQUIRE(!taglist.empty());

				taglist = ellipseFitter.process(std::move(taglist));
				REQUIRE(!taglist.empty());

				taglist = gridFitter.process(std::move(taglist));
				REQUIRE(!taglist.empty());

				taglist = decoder.process(std::move(taglist));
				REQUIRE(!taglist.empty());

				for (pipeline::Tag const& tag : taglist) {
					for (pipeline::TagCandidate const& candidate : tag.getCandidatesConst()) {
						REQUIRE(!candidate.getGridsConst().empty());
						REQUIRE(!candidate.getDecodings().empty());
					}
				}
			}
		}
	}

}
