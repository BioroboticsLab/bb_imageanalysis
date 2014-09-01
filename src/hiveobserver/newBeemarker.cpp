/*
 * newBeemarker.cpp
 *
 *  Created on: 07.08.2014
 *      Author: mareikeziese
 */

#ifndef BEEMARKER_H_
#define BEEMARKER_H_



#include <iostream>
#include <boost/filesystem.hpp>
#include <ostream>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/serialization/vector.hpp>
#include "../decoder/datastructure/BBList.h"

using namespace boost::filesystem;
using namespace std;
using namespace cv;
using namespace decoder;

BBList tags;
path directory;
//string modified_image;
//string tags_file;
vector<string> camera_images;
int mode = 1;
int countedWithoutTag = 0;
int countedUndefinedTag = 0;
int countedTags = 0;
bool openGL = false;


const string EXPORT_PATH =
		"/Users/mareikeziese/git/bb_imganalysis/beecounter_data/";

void BeecounterCallback(int event, int x, int y, int flags, void* userdata) {

	Mat* img = (Mat*) userdata;

	if (event == cv::EVENT_LBUTTONDOWN) {

		//cout << "Left button of the mouse is clicked - position (" << x << ", "
			//	<< y << ")" << endl;
		Point center(x, y);
		//tags
		if (mode == 1) {
			tags.AddPoint(center);
			circle(*img, center, 20, Scalar(0, 255, 0), 3);
		}
		//undefined
		if (mode == 2) {
			//tags.AddPoint(center);
			circle(*img, center, 20, Scalar(255, 0, 0), 3);
			countedUndefinedTag++;
		}
		//noTag
		if (mode == 3) {
			//tags.AddPoint(center);
			circle(*img, center, 20, Scalar(0, 0, 255), 3);
			countedWithoutTag++;
		}

		if (openGL) {
			try {
				updateWindow("Bee Counter");
			} catch (int e) {
				openGL = false;
				cout
						<< "OpenCV is not compiled with OpenGL, refresh manually with pressing enter "
						<< '\n';
			}
			//string text = "Box "
			//	+ boost::lexical_cast<std::string>(tags.size());
			//cv::putText(*img, text, center, FONT_HERSHEY_COMPLEX_SMALL, 1.5,
			//	Scalar(0, 255, 0));
		}
	}

	//
}

int redrawWindow(string windowName, Mat img) {
	destroyAllWindows();
//Create a window
	namedWindow("Bee Counter", WINDOW_NORMAL);

//set the callback function for any mouse event
	setMouseCallback("Bee Counter", BeecounterCallback, &img);

//show the image
	imshow("Bee Counter", img);

// Wait until user press some key
	int keyst = waitKey(0);

// press a to refresh window
	if (keyst == 97) {
		cout << "Complete Testgeneration" << endl;
		return 1;
		//for ( unsigned int i = 0; i < tags.size(); i++) {
		//	oa << tags[i];
		//}
		return 1;
		//press ESC to abort generation;
	} else if (keyst == 27) {
		cerr << "Aborting Testgeneration";
		return 0;

		//mark tags t
	} else if (keyst == 116) {
		mode = 1;
		cout << "set mode for bees with tag, try to mark the center of the tags"
				<< endl;
		return redrawWindow(windowName, img);
		//undefined u
	} else if (keyst == 117) {
		mode = 2;
		cout << "set mode for bees, where it's undefined, if they have a tag"
				<< endl;
		return redrawWindow(windowName, img);
		//no tag n
	} else if (keyst == 110) {
		mode = 3;
		cout << "set mode for bees with no tags" << endl;
		return redrawWindow(windowName, img);

	} else {

		cout << "key pressed : " << keyst << endl;
		return redrawWindow(windowName, img);

	}

}
void generateCameraResults(int id, string image_name, path dir, void* out_file) {

	tags = BBList();
	int startCountUndefined = countedUndefinedTag;
	int startCountWithoutTag = countedWithoutTag;
	if (image_name.size() == 0) {
		cerr << "no image given for camera " << id << endl;
		return;
	}
	path image(image_name);
	if (!exists(image)) {
		cerr << "the image for camera " << id << " doesn't exist" << endl;
		return;
	}

	path result_directory(dir.string() + "Camera" + to_string(id));
	create_directory(result_directory);

	path copyfile(
			result_directory.string() + "/original"
					+ image.extension().string());

	copy_file(image, copyfile);
	string modified_image = result_directory.string() + "/modified"
			+ image.extension().string();
	//cout << modified_image << "hier" << endl;
	string tags_file = result_directory.string() + "/tags.txt";

	Mat img = imread(image_name);

//if fail to read the image
	if (img.empty()) {
		cout << "Error loading the image" << endl;
		return;
	}

	tags = BBList();
	std::cout << "Press 'a' to complete marking, Press 'ESC' for aborting "
			<< endl;
	cout << "set mode for bees with tag, try to mark the center of the tags";
	mode = 1;
	int response = redrawWindow("Bee Counter", img);
	if(response == 1){
	countedTags = countedTags + tags.size();

	std::ostream* output = (std::ostream*) out_file;
	*output << "results for camera " << id << ":\n";
	*output << "bees with tags :" << tags.size() << ":" << endl;
	*output << "bees without tags :"
			<< to_string(countedWithoutTag - startCountWithoutTag) << ":"
			<< endl;
	*output << "undefined bees :"
			<< to_string(countedUndefinedTag - startCountUndefined) << ":"
			<< endl;
	*output << "bees total :"
			<< tags.size() + countedWithoutTag - startCountWithoutTag
					+ countedUndefinedTag - startCountUndefined << endl << endl
			<< endl;

	cout << "Generate Image" << endl;
			imwrite(modified_image, img);
			cout << "Generate Tags" << endl;
			std::ofstream ofs(tags_file);
			boost::archive::text_oarchive oa(ofs);
			oa & tags;

	//imwrite(modified_image, img);
	}

}

int main(int argc, char** argv) {

	camera_images = vector<string>();

// Read image from file
	if (argc > 1) {
	char* image_name_cam_0 = argv[1];
	camera_images.push_back(image_name_cam_0);
	}else{
		cout << "Usage img_cam_0 [img_cam_1] [img_cam_2] [img_cam_3]" <<endl;

	}

	if (argc > 2) {
		char* image_name_cam_1 = argv[2];
		camera_images.push_back(image_name_cam_1);
	}
	if (argc > 3) {
	 char* image_name_cam_2 = argv[3];
	 camera_images.push_back(image_name_cam_2);
	}
	if (argc > 4) {
	 char* image_name_cam_3 = argv[4];
	 camera_images.push_back(image_name_cam_3);}

	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%F-%H-%M", timeinfo);

	path directory = path(EXPORT_PATH + "results_" + buffer + "/");
//string testname =//file.stem().string();

//directory = path(EXPORT_PATH+ testname);

	if (is_directory(directory)) {
		std::cerr << "Ein Test f�r diese Eingabe besteht bereits" << endl;
		return 0;
	}
	create_directory(directory);
	boost::iostreams::stream_buffer<boost::iostreams::file_sink> buf(
			directory.string() + "/results.txt");
	std::ostream out_file(&buf);

	for (unsigned int i = 0; i < camera_images.size(); i++) {
		generateCameraResults(i, camera_images[i], directory, &out_file);
	}

	out_file << "results total :" << endl;
		out_file << "bees with tags total:" << countedTags << ":" << endl;
		out_file << "bees without tags total :"
				<<countedWithoutTag<< ":"
				<< endl;
		out_file << "undefined bees total :"
				<< countedUndefinedTag << ":"
				<< endl;
		out_file << "bees total :"
				<<to_string(countedTags + countedWithoutTag +  countedUndefinedTag) << endl << endl
				<< endl;



	return 1;

}

#endif /* BEEMARKER_H_ */
