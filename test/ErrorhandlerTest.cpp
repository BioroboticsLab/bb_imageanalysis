/*
 * FooTest.cpp
 *
 *  Created on: 29.06.2014
 *      Author: mareikeziese
 */

#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>

#include "gtest/gtest.h"
#include "../src/errorhandler/Errorhandler.h"
#include <boost/filesystem.hpp>


using namespace boost::filesystem;

using namespace std;

namespace {
class ErrorhandlerTest: public ::testing::Test {
protected:
	//Errorhandler err_handler;

public:
	/**
	 * function that is called immediately before a test
	 */
	static string foldername_;
	static string directory_;

	void SetUp() {

		path directory(this->directory_);
		//setting up folder structure for logging
		if (!is_directory(this->directory_)) {
			cerr << "Fehler in chdir: " << strerror(errno) << endl;
		} else{
			path p(this->directory_+this->foldername_);
			create_directory(p);
		}

	}

	/**
	 * function that is called immediately after a test
	 */
	void TearDown() {
		path filename(this->directory_+this->foldername_+"error.log");
		path directory(this->directory_+this->foldername_);
		bool response = remove(filename);
		bool response2 = remove(directory);


	}


};

string ErrorhandlerTest::directory_ = "/Users/mareikeziese/git/bb_imganalysis/logging/";
string ErrorhandlerTest::foldername_ = "test/";



/**
 * tests if handler fails if logging-directory does not exist
 */
TEST_F(ErrorhandlerTest, ErrorhandlerFolderNotExist) {
	ASSERT_ANY_THROW(Errorhandler(ErrorhandlerTest::directory_ + "test2"));
}

/**
 * tests if directory is empty
 */
TEST_F(ErrorhandlerTest, ErrorhandlerNoDirectoryGiven) {
	ASSERT_ANY_THROW(Errorhandler(""));
}




/**
 * checks if log-file is created
 */
TEST_F(ErrorhandlerTest, ErrorhandlerFileIsCreated) {

	string fulldirectory = ErrorhandlerTest::directory_ + ErrorhandlerTest::foldername_;



	Errorhandler handler = Errorhandler(fulldirectory);
	handler.log(1, "Das hier ist ein Fehler");
	string filename= fulldirectory + "error.log";

	path errorfile(filename);

	EXPECT_TRUE(exists(errorfile));
	EXPECT_TRUE(is_regular_file(errorfile));


}



}
