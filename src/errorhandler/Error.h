/*
 * Error.h
 *
 *  Created on: 21.07.2014
 *      Author: mareikeziese
 */

#ifndef ERROR_H_
#define ERROR_H_


#include <iostream>

#include <fstream>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>

using namespace std;


namespace errorhander {

class Error {
private:
	int loglevel_;
	string message_;
	string module_;
	string function_;

public:
	Error();
	Error(int loglevel);
	bool virtual report();
	virtual ~Error();
};

} /* namespace errorhander */

#endif /* ERROR_H_ */
