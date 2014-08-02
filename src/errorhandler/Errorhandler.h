/*
 * Errorhandler.h
 *
 *  Created on: 16.07.2014
 *      Author: mareikeziese
 */

#ifndef ERRORHANDLER_H_
#define ERRORHANDLER_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <boost/filesystem.hpp>
#include "config.h"


using namespace boost::filesystem;
using namespace std;


class Errorhandler {
public:
	Errorhandler();
	Errorhandler(string log_dir);
	Errorhandler(string log_dir, string filename);
	virtual ~Errorhandler();
	virtual bool log(int level,string message);
	virtual bool checkValidDirectory(string directory);
private:
	string log_dir_;
	string filename_;


};

#endif /* ERRORHANDLER_H_ */
