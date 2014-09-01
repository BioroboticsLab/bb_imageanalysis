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

void DecodingProcess::process(string filename){

	Converter converter = Converter();
	Localizer localizer = Localizer();
	Recognizer rec = Recognizer();
	Transformer trans = Transformer();

	Mat image = converter.process(filename);
	TagList taglist = localizer.process(image);
	rec.process(taglist);
	trans.process(taglist);



}
