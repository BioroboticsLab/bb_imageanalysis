#include <array>
#include <vector>

#include <boost/filesystem.hpp>

#include "catch.hpp"

#include <pipeline/Preprocessor.h>
#include <pipeline/Localizer.h>
#include <pipeline/EllipseFitter.h>
#include <pipeline/GridFitter.h>
#include <pipeline/Decoder.h>

#include <pipeline/settings/Settings.h>

#include "bb_imganalysis/utility/util.h"

TEST_CASE("Pipeline finishes with test data and returns results", "[integration]") {
	pipeline::Preprocessor preprocessor;
	pipeline::Localizer localizer;
	pipeline::EllipseFitter ellipseFitter;
	pipeline::GridFitter gridFitter;
	pipeline::Decoder decoder;

    boost::filesystem::path basePath(TEST_DATA_PATH);
    boost::filesystem::path configFile = basePath / "settings.json";

    util::loadSettingsFile(preprocessor,
                           localizer,
                           ellipseFitter,
                           gridFitter,
                           decoder,
                           configFile);

    for (boost::filesystem::recursive_directory_iterator end, dir(basePath); dir != end; ++dir ) {
		if (boost::filesystem::is_regular_file(*dir)) {
			if (boost::filesystem::extension(*dir) == ".jpeg") {
				INFO(*dir);

                cv::Mat img = cv::imread((*dir).path().string(), CV_LOAD_IMAGE_GRAYSCALE);
				std::vector<pipeline::Tag> taglist;
                pipeline::PreprocessorResult preprocessed = preprocessor.process(img);
                REQUIRE(!preprocessed.originalImage.empty());
                REQUIRE(!preprocessed.preprocessedImage.empty());
                REQUIRE(!preprocessed.claheImage.empty());

                taglist = localizer.process(std::move(preprocessed));
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
