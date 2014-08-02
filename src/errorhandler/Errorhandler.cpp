/*
 * Errorhandler.cpp
 *
 *  Created on: 16.07.2014
 *      Author: mareikeziese
 */

#include "Errorhandler.h"

/**
 * construtor using default logging path
 */
Errorhandler::Errorhandler() {

	path p(DEFAULT_LOGGING_PATH);
	if (this->checkValidDirectory(DEFAULT_LOGGING_PATH)) {
		this->log_dir_ = DEFAULT_LOGGING_PATH;
	} else {
		throw new std::exception();
	}
	this->filename_ = DEFAULT_LOGGING_FILENAME;
}

/**
 *
 * @param log_dir custom logging path
 */
Errorhandler::Errorhandler(string log_dir) {

	if (this->checkValidDirectory(log_dir)) {
		this->log_dir_ = log_dir;
	} else {
		throw new std::exception();
	}
	this->filename_ = DEFAULT_LOGGING_FILENAME;
}
/**
 *
 * @param log_dir custom logging path
 * @param filename custom filename
 */
Errorhandler::Errorhandler(string log_dir, string filename) {

	if (this->checkValidDirectory(log_dir)) {
		this->log_dir_ = log_dir;
	} else {
		throw new std::exception();
	}
	this->filename_ = filename;


}

bool Errorhandler::checkValidDirectory(string directory) {
	path p(directory);

	//if no directory given throw exception
	if (!exists(p)) {
		return false;
	}
	//if logging directory does not exist throw exception
	if (!is_directory(p)) {
		return false;
	}
	return true;
}

Errorhandler::~Errorhandler() {

}
/**
 * write a new row in the log file
 *
 * @param level error-level
 * @param message message for the error
 * @return true if writing works, false else
 */
bool Errorhandler::log(int level, string message) {
	ofstream log_file;
	std::stringstream filename;
	filename << "error.log";

	log_file.open(this->log_dir_ + filename.str(), std::ofstream::app);
	log_file << "[Errorlevel:" << level << "] :" << message << std::endl;
	log_file.close();
	return true;
}

