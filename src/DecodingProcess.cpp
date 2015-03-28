/*
 * DecodingProcess.cpp
 *
 *  Created on: 21.08.2014
 *      Author: mareikeziese
 */

#include "DecodingProcess.h"

#include <chrono>
#include <cstdlib>
#include <iostream>

#include <opencv2/opencv.hpp>

#include "pipeline/Preprocessor.h"
#include "pipeline/Localizer.h"
#include "pipeline/EllipseFitter.h"
#include "pipeline/GridFitter.h"
#include "pipeline/Decoder.h"

namespace {
class MeasureTimeRAII {
public:
    MeasureTimeRAII(std::string const& what)
        : _start(std::chrono::steady_clock::now())
        , _what(what)
    {}
    ~MeasureTimeRAII()
    {
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << _what << " took "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - _start).count()
                  << "ms.\n";
    }
private:
    std::chrono::steady_clock::time_point _start;
    std::string _what;
};

}

void DecodingProcess::process(std::string const& filename) const {
	pipeline::Preprocessor preprocessor;
	pipeline::Localizer localizer;
	pipeline::EllipseFitter ellipseFitter;
	pipeline::GridFitter gridFitter;
	pipeline::Decoder decoder;

	cv::Mat img = cv::imread(filename);
	cv::Mat preprocessedImg;
    std::vector<pipeline::Tag> taglist;
    {
        MeasureTimeRAII measure("Preprocessor");
		preprocessedImg = preprocessor.process(img);
    }
    {
        MeasureTimeRAII measure("Localizer");
		taglist = localizer.process(std::move(img), std::move(preprocessedImg));
    }
    {
        MeasureTimeRAII measure("EllipseFitter");
		taglist = ellipseFitter.process(std::move(taglist));
    }
    {
        MeasureTimeRAII measure("GridFitter");
        taglist = gridFitter.process(std::move(taglist));
    }
    {
        MeasureTimeRAII measure("Decoder");
        taglist = decoder.process(std::move(taglist));
    }

	using namespace pipeline;
    // remove invalid tags
    taglist.erase(std::remove_if(taglist.begin(), taglist.end(), [](Tag& tag) { return !tag.isValid(); }), taglist.end());
    std::cout << std::endl << taglist.size() << " Tags gefunden" << std::endl << std::endl;
    for (Tag& tag : taglist) {
        std::cout << "Tag: " << std::endl;
        vector<TagCandidate>& candidates = tag.getCandidates();
        std::cout << "\t" << candidates.size() << " Kandidaten " << std::endl;
        for (TagCandidate& candidate : candidates) {
            std::cout << "\tKandidat: " << std::endl;
            std::cout << "\t\tEllipsenscore " << candidate.getEllipse().getVote() << ":" << std::endl;
            for (decoding_t const& decoding : candidate.getDecodings()) {
                std::cout << "\t\tDecoding :" << endl;
				std::cout << "\t\t\tId "<< decoding.to_string() << std::endl;
            }
        }
    }
}

#include <boost/filesystem.hpp>

int main(int argc, char** argv) {
	if (argc != 2) {
        std::cerr << "Invalid number of arguments" << std::endl;
        return EXIT_FAILURE;
	}
	boost::filesystem::path image_name(argv[1]);

    if (!exists(image_name)) {
        std::cerr << "Invalid path" << std::endl;
        return EXIT_FAILURE;
    }

	DecodingProcess dprocess = DecodingProcess();
	dprocess.process(image_name.string());
}
