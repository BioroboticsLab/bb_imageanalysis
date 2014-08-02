/*
 * Error.cpp
 *
 *  Created on: 21.07.2014
 *      Author: mareikeziese
 */

#include "Error.h"
#include "Errorhandler.h"

namespace errorhander {

Error::Error(int loglevel) {
	this->loglevel_ = loglevel;

}

Error::~Error() {
	// TODO Auto-generated destructor stub
}

bool Error::report() {
	if(this->loglevel_ == 1){
		//Errorhandler handler = Errorhandler();
		//bool response = handler.log(this->loglevel_, this->message_);
		//return response;
		return true;
	}
	return false;
}


} /* namespace errorhander */
