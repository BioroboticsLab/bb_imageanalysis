/*
 * DecodingProcess.cpp
 *
 *  Created on: 21.08.2014
 *      Author: mareikeziese
 */

#include "DecodingProcess.h"

#include "dirent.h"
#include <stdio.h>
#include <chrono>
#include <cstdlib>
#include <iostream>

#include <opencv2/opencv.hpp>

#include "pipeline/Preprocessor.h"
#include "pipeline/Localizer.h"
#include "pipeline/EllipseFitter.h"
#include "pipeline/GridFitter.h"
#include "pipeline/Decoder.h"
#include <boost/filesystem.hpp>

#define DEBUG_PROGRAM

	using namespace boost::filesystem;



namespace {
class MeasureTimeRAII {
public:
	MeasureTimeRAII(std::string const& what) :
			_start(std::chrono::steady_clock::now()), _what(what) {
	}
	~MeasureTimeRAII() {
		std::chrono::steady_clock::time_point end =
				std::chrono::steady_clock::now();
		std::cout << _what << " took "

				<< std::chrono::duration_cast<std::chrono::milliseconds>(
						end - _start).count() << "ms.\n";
		Logger::log(
				std::stringstream().flush() << _what << " took "
						<< std::chrono::duration_cast<std::chrono::milliseconds>(
								end - _start).count() << "ms.",
				severity_level::normal);
	}
private:
	std::chrono::steady_clock::time_point _start;
	std::string _what;
};

}

void DecodingProcess::_loadMetaInfos(const std::string &filename) {

	this->meta_infos.camera_id = boost::lexical_cast<unsigned int>(
			filename.substr(4, 1));
	std::cout << "camera id " << this->meta_infos.camera_id << std::endl;

	Logger::log(
			std::stringstream().flush() << "identificate camera id = "
					<< this->meta_infos.camera_id,severity_level::normal);
	std::string timestring = filename.substr(6, 14);
	std::cout << "camera id " << timestring << std::endl;
	time_t timestamp = seconds_from_epoch(timestring);

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
	std::vector<pipeline::Tag> taglist2;

	std::stringstream ss;
	{

		try {
			MeasureTimeRAII measure("Preprocessor");
			preprocessedImg = preprocessor.process(img);

		} catch (const std::exception & e) {
			ss << "Error on Preprocessor: " << e.what() << " stop processing.";
			std::cout << ss.str();
			Logger::log(ss,severity_level::critical);
			return;
		}
	}
	{
		try {
			MeasureTimeRAII measure("Localizer");
			taglist = localizer.process(std::move(img),
					std::move(preprocessedImg));

#ifdef DEBUG_PROGRAM

			// shorten result for faster debugging
			size_t i = 0;
			while(i < 10 && i < taglist.size()){
				taglist2.push_back(taglist[i]);
				i++;
			}
			taglist = taglist2;
#endif

		} catch (const std::exception & e) {
			ss << "Error on Localizer: " << e.what() << " stop processing.";
			std::cout << ss.str();
			Logger::log(ss,severity_level::critical);
			return;
		}
	}
	{
		try {
			MeasureTimeRAII measure("EllipseFitter");
			taglist = ellipseFitter.process(std::move(taglist));
		} catch (const std::exception & e) {
			ss << "Error on EllipseFitter: " << e.what() << " stop processing.";
			std::cout << ss.str();
			Logger::log(ss,severity_level::critical);
			return;
		}
	}
	{
		try {
			MeasureTimeRAII measure("GridFitter");
			taglist = gridFitter.process(std::move(taglist));
		} catch (const std::exception & e) {
			ss << "Error on GridFitter: " << e.what() << " stop processing.";
			std::cout << ss.str();
			Logger::log(ss,severity_level::critical);
			return;
		}
	}
	{
		try {
			MeasureTimeRAII measure("Decoder");
			taglist = decoder.process(std::move(taglist));
		} catch (const std::exception & e) {

			ss << "Error on Decoder: " << e.what() << " stop processing.";
			std::cout << ss.str();
			Logger::log(ss,severity_level::critical);
			return;
		}
	}

	using namespace pipeline;

	path p (filename);
	Export ex = Export();
	ex.writeCSV(taglist, p.parent_path().string()+"/"+p.filename().string()+".csv");

	// remove invalid tags
	/*taglist.erase(
			std::remove_if(taglist.begin(), taglist.end(),
					[](Tag& tag) {return !tag.isValid();}), taglist.end());
	std::cout << std::endl << taglist.size() << " Tags gefunden" << std::endl
			<< std::endl;
	for (Tag& tag : taglist) {
		std::cout << "Tag: " << std::endl;
		vector<TagCandidate>& candidates = tag.getCandidates();
		std::cout << "\t" << candidates.size() << " Kandidaten " << std::endl;
		for (TagCandidate& candidate : candidates) {
			std::cout << "\tKandidat: " << std::endl;
			std::cout << "\t\tEllipsenscore "
					<< candidate.getEllipse().getVote() << ":" << std::endl;
			for (decoding_t const& decoding : candidate.getDecodings()) {
				std::cout << "\t\tDecoding :" << endl;
				std::cout << "\t\t\tId " << decoding.to_string() << std::endl;
			}
		}
	}*/
}



bool checkValidFilename(const string& filename) {

	//check if filename is given
	if (filename.size() == 0) {
		return false;
	}

	path p(filename);

	//check if file has the right extension
	if (p.extension().string() == ".png" || p.extension().string() == ".jpeg") {
		return true;
	}
	return false;
}

void listImagesDebug(const char *directoryName,
		vector<std::string> &imageFiles) {
	std::string path = std::string(directoryName);
	DIR *directory = opendir(path.c_str());
	struct dirent *file;
	while ((file = readdir(directory))) {
		if ((file->d_type & DT_DIR)) { // directories are uninteresting
			continue;
		}		// here be file..

		std::string filename = path + file->d_name;
		if (checkValidFilename(filename)) {
			imageFiles.push_back(filename);
		} else {
			Logger::log(
					std::stringstream().flush() << "ignoring file " << filename,
					severity_level::warning);
		}
	}
	closedir(directory);
}

int main(int argc, char** argv) {

// create a list of the files this process has to analyse
	vector<std::string> imageFiles;
	if (argc <= 1) {
		std::cerr << "directory is missing" << std::endl;
		return 1;
	}
	char* directoryName = argv[1];
	Logger::init(directoryName);
	Logger::log(
			std::stringstream().flush() << "start decoder process on folder "
					<< directoryName,severity_level::notification);
	listImagesDebug(directoryName, imageFiles);

	unsigned int imageCount = imageFiles.size();
	if (imageCount == 0) {
		cout << "no work found. shutdown" << endl;
		Logger::log(
				std::stringstream().flush() << "directory " << directoryName
						<< " empty. shutdown process",
				severity_level::notification);
		return 0;
	}

	Logger::log(
			std::stringstream().flush() << "start processing " << imageCount
					<< "images ..",severity_level::notification);
	cout << "I have to work on " << imageCount << " images." << endl;

	DecodingProcess dprocess = DecodingProcess();

	for (unsigned int imageNumber = 0; imageNumber < imageCount;
			++imageNumber) {
		cout << "process works on image " << imageFiles[imageNumber] << endl;
		Logger::log(
				std::stringstream().flush() << "process works on image "
						<< imageFiles[imageNumber],
				severity_level::notification);
		dprocess.process(imageFiles[imageNumber]);
		// After processing the image it is deleted to signalize that the processing was completed.
		// If the image is not deleted, it will be analyzed again.
		if (::remove(std::string(imageFiles[imageNumber]).c_str()) != 0)
			perror("Error deleting the image ");
	}
}
