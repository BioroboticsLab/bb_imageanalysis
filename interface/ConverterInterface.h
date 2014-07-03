/*
 * ConverterInterface.h
 *
 *  Created on: 29.06.2014
 *      Author: mareikeziese
 */

#ifndef CONVERTERINTERFACE_H_
#define CONVERTERINTERFACE_H_


#include <stdio.h>
#include <unistd.h>
#include <fstream>


using namespace std;

namespace decoder_interface {

class Converter_Interface {
public:
	Converter_Interface();
	virtual ~Converter_Interface();
	/**
		 * @return cv::Math image
		 */
		virtual void process(const string& filename);
	private:

};

} /* namespace decoder_interface */

#endif /* CONVERTERINTERFACE_H_ */
