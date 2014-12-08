/*
 * DecodingProcess.cpp
 *
 *  Created on: 21.08.2014
 *      Author: mareikeziese
 */

#include "DecodingProcess.h"

#include <chrono>

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

void DecodingProcess::process(string filename) const {
    Converter converter;
    Localizer localizer;
    Recognizer recognizer;
    Transformer transformer;
    GridFitter gridfitter;
    Decoder decoder;

    Mat img = converter.process(filename);
    vector<Tag> taglist;
    {
        MeasureTimeRAII measure("Localizer");
        taglist = localizer.process(std::move(img));
    }
    {
        MeasureTimeRAII measure("Recognizer");
        taglist = recognizer.process(std::move(taglist));
    }
    {
        MeasureTimeRAII measure("Transformer");
        taglist = transformer.process(std::move(taglist));
    }
    {
        MeasureTimeRAII measure("GridFitter");
        taglist = gridfitter.process(std::move(taglist));
    }
    {
        MeasureTimeRAII measure("Decoder");
        taglist = decoder.process(std::move(taglist));
    }

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
            for (Decoding const& decoding : candidate.getDecodings()) {
                std::cout << "\t\tDecoding :" << endl;
				std::cout << "\t\t\tId "<< decoding.tagId << ":" << std::endl;
                std::cout << "\t\t\tScore " << decoding.score << ":" << endl;
            }
        }
    }
}

int main(int argc, char** argv) {
    path image_name;

    image_name = path(argv[1]);
    if (!exists(image_name)) {
        cerr << "No image given";
        return 1;
    }else{
        DecodingProcess dprocess = DecodingProcess();
        dprocess.process(image_name.string());
    }
}
