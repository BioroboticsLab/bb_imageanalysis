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

TEST_CASE("Pipeline finishes with test data and returns results", "[integration]") {
	pipeline::Preprocessor preprocessor;
	pipeline::Localizer localizer;
	pipeline::EllipseFitter ellipseFitter;
	pipeline::GridFitter gridFitter;
	pipeline::Decoder decoder;

    boost::filesystem::path basePath(TEST_DATA_PATH);
    boost::filesystem::path configFile = basePath / "settings.json";

    if (boost::filesystem::is_regular_file(configFile)) {
        pipeline::settings::preprocessor_settings_t preprocessor_settings;
        pipeline::settings::localizer_settings_t localizer_settings;
        pipeline::settings::ellipsefitter_settings_t ellipsefitter_settings;
        pipeline::settings::gridfitter_settings_t gridfitter_settings;

        for (pipeline::settings::settings_abs* settings :
             std::array<pipeline::settings::settings_abs*, 4>({&preprocessor_settings,
                                                               &localizer_settings,
                                                               &ellipsefitter_settings,
                                                               &gridfitter_settings}))
        {
            settings->loadFromJson(configFile.string());
        }

        // TODO: don't test deeplocalizer for now
        localizer_settings.setValue(pipeline::settings::Localizer::Params::DEEPLOCALIZER_FILTER, false);

        preprocessor.loadSettings(preprocessor_settings);
        localizer.loadSettings(localizer_settings);
        ellipseFitter.loadSettings(ellipsefitter_settings);
        gridFitter.loadSettings(gridfitter_settings);
    } else {
        REQUIRE(false);
    }

    for (boost::filesystem::recursive_directory_iterator end, dir(basePath); dir != end; ++dir ) {
		if (boost::filesystem::is_regular_file(*dir)) {
			if (boost::filesystem::extension(*dir) == ".jpeg") {
				INFO(*dir);

                cv::Mat img = cv::imread((*dir).path().string(), CV_LOAD_IMAGE_GRAYSCALE);
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
