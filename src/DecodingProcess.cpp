/*
 * DecodingProcess.cpp
 *
 *  Created on: 21.08.2014
 *      Author: mareikeziese
 */

#include "DecodingProcess.h"

DecodingProcess::DecodingProcess() {
	// TODO Auto-generated constructor stub

}

DecodingProcess::~DecodingProcess() {
	// TODO Auto-generated destructor stub
}

void DecodingProcess::process(string filename) {

	Converter converter = Converter();
	Localizer localizer = Localizer();
	Recognizer recognizer = Recognizer();
	Transformer transformer = Transformer();
	GridFitter gridfitter = GridFitter();
	Decoder decoder = Decoder();

	Mat image = converter.process(filename);
	vector<Tag> taglist = localizer.process(image);
	recognizer.process(taglist);
	transformer.process(taglist);
	gridfitter.process(taglist);
	decoder.process(taglist);

    size_t i = 0;
    for (Tag const& tag : taglist) {
        ++i;
        if (tag.isValid()) {

            cout << "Tag " << i << ":" << endl;
            Vector<TagCandidate> candidates = tag.getCandidates();

            cout << "	" << candidates.size() << " Kandidaten " << endl;

            for (unsigned int j = 0; j < candidates.size(); j++) {
                TagCandidate candidate = candidates[j];
                cout << "	Kandidat " << j << ":" << endl;
                cout << "		Ellipsenscore " << candidate.getEllipse().getVote()
                        << ":" << endl;
                vector<Decoding> decodings = candidate.getDecodings();

                for (unsigned int k = 0; k < decodings.size(); k++) {
                    Decoding decoding = decodings[k];
                    cout << "		Decoding " << k << ":" << endl;
                    cout << "			Id " << decoding.id << ":" << endl;
                    cout << "			Score " << decoding.score << ":" << endl;

                }

            }
        } else {
            cout << "Tag " << i << " ist kein gltiges Tag gewesen" << endl;
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
