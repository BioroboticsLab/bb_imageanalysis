/*
 * Testgenerator.cpp
 *
 *  Created on: 30.07.2014
 *      Author: mareikeziese
 */

#include "Testgenerator.h"

namespace testgenerator {

vector<Point> tags;
path directory;
string modified_image;
string tags_file;
vector<Rect> ellipses;

/**
 *
 * @param windowName
 * @param img
 * @return
 */
int LocalizerRedrawWindow(string windowName, Mat img) {
	//destroyAllWindows();
	//Create a window
	namedWindow("My Window", WINDOW_NORMAL );

	//set the callback function for any mouse event
	setMouseCallback("My Window", LocalizerMouseCallback, &img);

	//show the image
	imshow("My Window", img);

	// Wait until user press some key
	int keyst = waitKey(0);

	// press a to refresh window
	if (keyst == 97) {
		cout << "Complete Localizer-Testgeneration" << endl;
		cout << "Generate Image" << endl;
		imwrite(modified_image, img);
		cout << "Generate Tags" << endl;

		ofstream ofs(tags_file);
		boost::archive::text_oarchive oa(ofs);
		oa << tags;
		ofs.close();


		//press ESC to abort generation;
	} else if (keyst == 27) {
		cerr << "Aborting Testgeneration";
	} else {
		LocalizerRedrawWindow(windowName, img);
	}

	return 0;
}

/**
 *
 * @param event
 * @param x
 * @param y
 * @param flags
 * @param userdata
 */
void LocalizerMouseCallback(int event, int x, int y, int flags,
		void* userdata) {

	Mat* img = (Mat*) userdata;

	if (event == cv::EVENT_LBUTTONDOWN) {

		Point center(x,y);
		tags.push_back(center);
		circle(*img, center, 20, Scalar(255, 0, 0), 3);

		string text = "Box " + boost::lexical_cast<std::string>(tags.size());
		cv::putText(*img, text, center, FONT_HERSHEY_COMPLEX_SMALL, 1.5,
				Scalar(0, 255, 0));

		//imshow("My Window", *img);
		LocalizerRedrawWindow("My Window", *img);
	}

}
/**
 *
 * @param image_name
 * @return
 */
string generateLocalizerData(string image_name) {
	// Read image from file
	//char* image_name = argv[1];

	cout << "Start generating localizer-testdata" << endl;
	path file(image_name);

	string testname = file.stem().string();

	directory = path(testconfig::TEST_ROI_PATH + testname);

	if (is_directory(directory)) {
		cerr << "there is already a test for this input " << endl;
		return 0;
	}
	create_directory(directory);

	path copyfile(directory.string() + "/original" + file.extension().string());

	copy_file(file, copyfile);
	modified_image = directory.string() + "/modified"
			+ file.extension().string();
	tags_file = directory.string() + "/tags.txt";

	Mat img = imread(image_name);

	//if fail to read the image
	if (img.empty()) {
		cerr << "Error loading the image" << endl;
		return "";
	}

	tags = vector<Point>();
	int response = LocalizerRedrawWindow("My Window", img);

	return directory.string() + "/original" + file.extension().string();
}


/**
 *
 * @param windowName
 * @param img
 * @return
 */
int RecognizerRedrawWindow(string windowName, Mat img) {
	destroyAllWindows();
	//Create a window
	namedWindow("My Window", WINDOW_NORMAL);

	//set the callback function for any mouse event
	setMouseCallback("My Window", RecognizerMouseCallback, &img);

	//show the image
	imshow("My Window", img);

	// Wait until user press some key
	int keyst = waitKey(0);

	// press a to refresh window
	if (keyst == 97) {
		cout << "Complete Localizer-Testgeneration" << endl;
		cout << "Generate Image" << endl;
		imwrite(modified_image, img);
		cout << "Generate Tags" << endl;

		ofstream ofs(tags_file);
		boost::archive::text_oarchive oa(ofs);
		oa << tags;
		ofs.close();


		//press ESC to abort generation;
	} else if (keyst == 27) {
		cerr << "Aborting Testgeneration";
	} else {
		RecognizerRedrawWindow(windowName, img);
	}

	return 0;
}

/**
 *
 * @param event
 * @param x
 * @param y
 * @param flags
 * @param userdata
 */
void RecognizerMouseCallback(int event, int x, int y, int flags,
		void* userdata) {

	Mat* img = (Mat*) userdata;
	//ellipse(subroiTest, currentEllipse.cen, currentEllipse.axis, currentEllipse.angle, 0, 360, Scalar(0,0,255));
	if (event == cv::EVENT_LBUTTONDOWN) {

		Point center(x,y);
		tags.push_back(center);
		circle(*img, center, 20, Scalar(255, 0, 0), 3);

		string text = "Box " + boost::lexical_cast<std::string>(tags.size());
		cv::putText(*img, text, center, FONT_HERSHEY_COMPLEX_SMALL, 1.5,
				Scalar(0, 255, 0));

		//redrawWindow("My Window", *img);
	}

}
/**
 *
 * @param localizer_testdata_original_image
 * @param use_test_configs
 * @return
 */
string generateRecognizerData(string localizer_testdata_original_image,
		bool use_test_configs) {
	// Read image from file
	//char* image_name = argv[1];

	cout << "Start generating Recognizer-Testdata" << endl;
	path testdata_image(localizer_testdata_original_image);

	//string tags_path = testdata_image.parent_path(),string()+"tags.txt";

	//load tags from file
	tags_file = directory.string() + "/tags.txt";
	ifstream ifs(tags_file);
	boost::archive::text_iarchive ia(ifs);
	vector<Point> rec_tags;
	ia >> rec_tags;
	ifs.close();
	TagList tmp_tags = TagList();

	Mat originalImage = imread(localizer_testdata_original_image);

	string canny_image_path = directory.string() + "/canny."+ testdata_image.extension().string();
	Mat cannyImage = imread(canny_image_path);

	for(int i = 0; i < rec_tags.size(); i ++){

		Point p = rec_tags[i];
		int imagesize = config::LOCALIZER_MAXTAGSIZE;
					Rect box = Rect(p.x - (imagesize / 2),
							p.y - (imagesize / 2), imagesize, imagesize);

					//make sure, that the box is not outside the image
					if ((box.x + box.width) > originalImage.cols) {
						box.x -= abs(
								box.x + box.width
										- originalImage.cols);
					}

					if ((box.y + box.height) > originalImage.rows) {
						box.y -= abs(
								box.y + box.height
										- originalImage.rows);
					}

					if (box.x < 0) {
						box.x = 0;
					}

					if (box.y < 0) {
						box.y = 0;
					}
					Tag tag = Tag(box);

					//create subimage
					Mat subImage(originalImage, box);
					tag.setOrigSubImage(subImage);
					Mat subcannyImage(cannyImage, box);
					tag.setCannySubImage(subcannyImage);

					tmp_tags.addTag(tag);
	}


	//int response = RecognizerRedrawWindow("My Window");


//
//	string testname = testdata_image.parent_path().stem().string();
//
//
//
//	directory = path(testconfig::TEST_RECOGNIZER_PATH + testname);
//
//	if (is_directory(directory)) {
//		cerr << "there is already a test for this input " << endl;
//		return 0;
//	}
//	create_directory(directory);
//
//	decoder::Converter converter = decoder::Converter();
//	Mat grayImage = converter.process(localizer_testdata_original_image);
//	decoder::Localizer localizer = decoder::Localizer();
//
//	//if localizer-testconfigs should be used
//	if (use_test_configs) {
//		vector<string> config_files = testgenerator::getAllFilesInFolder(
//				testconfig::TEST_ROI_CONFIGS_PATH);
//		path te_dir_tmp;
//
//		//iterate over all config files
//		for (int i = 0; i < config_files.size(); i++) {
//			path filename = path(config_files[i]);
//			te_dir_tmp = path(
//					directory.string() + "/" + filename.stem().string() + "/");
//			create_directory(te_dir_tmp);
//			localizer = decoder::Localizer(filename);
//			decoder::TagList tags = localizer.process(grayImage);
//
//		}
//
//	}

	return "";
}

/**
 *
 * @param folder
 * @return
 */
vector<string> getAllFilesInFolder(string folder) {
	vector<string> files = vector<string>();
	path p(folder);

	if (exists(p)) {
		if (is_directory(p)) {
			typedef vector<path> vec;             // store paths,
			vec v;                              // so we can sort them later
			copy(directory_iterator(p), directory_iterator(), back_inserter(v));
			sort(v.begin(), v.end());     // sort, since directory iteration
										  // is not ordered on some file systems
			for (vec::const_iterator it(v.begin()); it != v.end(); ++it) {
				try {
					path filename = *it;
					files.push_back(filename.string());

				} catch (int e) {
					cout << "An exception occurred. Exception Nr. " << e
							<< '\n';
				}
			}
		}
	}
	return files;
}

void compareLocalizerResults(vector<Point> tags, TagList taglist, TagList &found,
		TagList &wrongFound, TagList &notFound, Mat originalImage) {

	Rect rec;

	//for all tags that are manually marked
	for (int i = 0; i <= tags.size(); i++) {
		cv::Point p = tags[i];
		bool tagFound = false;

		//try to find a appropriate box in the list, that the localizer has produced
		for (int j = 0; j <= taglist.size(); j++) {

			Tag tag = taglist.getTag(j);
			Rect box = tag.getBox();
			//when the tag is found and the loop and remove the tag form the list
			if (isPossibleCenter(p, box,
					testconfig::TEST_EXPORT_ROI_TOLERANCE)) {

				tagFound = true;

				taglist.removeTag(j);
				break;
			}
		}
		//if the manually marked point has no appropriate box,
		//then create a new virtual tag and add it to the notFound List
		if (!tagFound) {
			Tag notfound_tag = Tag();
			int imagesize = config::LOCALIZER_MAXTAGSIZE - 30;
			Rect not_found_box = Rect(p.x - (imagesize / 2),
					p.y - (imagesize / 2), imagesize, imagesize);

			//make sure, that the box is not outside the image
			if ((not_found_box.x + not_found_box.width) > originalImage.cols) {
				not_found_box.x -= abs(
						not_found_box.x + not_found_box.width
								- originalImage.cols);
			}

			if ((not_found_box.y + not_found_box.height) > originalImage.rows) {
				not_found_box.y -= abs(
						not_found_box.y + not_found_box.height
								- originalImage.rows);
			}

			if (not_found_box.x < 0) {
				not_found_box.x = 0;
			}

			if (not_found_box.y < 0) {
				not_found_box.y = 0;
			}

			//create subimage
			Mat subImage(originalImage, not_found_box);
			notfound_tag.setOrigSubImage(subImage);

			notFound.addTag(notfound_tag);

		}

	}

	//all tags that are still in the list were not marked manually
	wrongFound = taglist;

}



void exportLocalizerTestResults(TagList found, TagList wrongFound,
		TagList notFound, Mat exportImage, path te_dir_tmp) {

	path tmp_success, tmp_failed, tmp_notfound;

	if (testconfig::TEST_EXPORT_ROI_RESULT_SUBIMAGES) {

		tmp_success = path(te_dir_tmp.string() + "/success");
		tmp_failed = path(te_dir_tmp.string() + "/failed");
		tmp_notfound = path(te_dir_tmp.string() + "/notfound");

		create_directory(tmp_success);
		create_directory(tmp_failed);
		create_directory(tmp_notfound);
	}

	//found in green
	 _exportLocalizerTestResults_helper(found, exportImage,
			tmp_success,testconfig::TEST_EXPORT_ROI_RESULT_BIGIMAGES && testconfig::TEST_EXPORT_ROI_SHOW_SUCCESS,
			testconfig::TEST_EXPORT_ROI_RESULT_SUBIMAGES && testconfig::TEST_EXPORT_ROI_SHOW_SUCCESS,
			 Scalar(0, 255, 0));

	//wrong found in blue
	 _exportLocalizerTestResults_helper(wrongFound, exportImage,
			tmp_failed,testconfig::TEST_EXPORT_ROI_RESULT_BIGIMAGES && testconfig::TEST_EXPORT_ROI_SHOW_FAILED,
			testconfig::TEST_EXPORT_ROI_RESULT_SUBIMAGES && testconfig::TEST_EXPORT_ROI_SHOW_FAILED,
			 Scalar(255, 0, 0));

	//not found in red
	 _exportLocalizerTestResults_helper(notFound, exportImage,
			tmp_notfound,testconfig::TEST_EXPORT_ROI_RESULT_BIGIMAGES && testconfig::TEST_EXPORT_ROI_SHOW_NOTFOUND,
			testconfig::TEST_EXPORT_ROI_RESULT_SUBIMAGES && testconfig::TEST_EXPORT_ROI_SHOW_NOTFOUND,
			Scalar(0, 0, 255));

	//export overview-image
	if (testconfig::TEST_EXPORT_ROI_RESULT_BIGIMAGES) {
		imwrite(te_dir_tmp.string() + "/results.jpeg", exportImage);
	}
}


void _exportLocalizerTestResults_helper(TagList tags, Mat &exportImage,
		path exportPath, bool exportBigImage, bool exportSubImage,
		Scalar color) {
	for (int i = 0; i < tags.size(); i++) {
		Tag tag = tags.getTag(i);
		Rect box = tag.getBox();

		if (exportSubImage) {
			string img_name = exportPath.string() + "/" + to_string(box.x) + "_"
					+ to_string(box.y) + "__" + to_string(box.width) + "_"
					+ to_string(box.height) + ".jpeg";

			imwrite(img_name, tag.getOrigSubImage());
		}
		if (exportBigImage) {

			line(exportImage, Point(box.x, box.y),
					Point(box.x + box.width, box.y), color);
			line(exportImage, Point(box.x, box.y),
					Point(box.x, box.y + box.height), color);
			line(exportImage, Point(box.x + box.width, box.y),
					Point(box.x + box.width, box.y + box.height),
					Scalar(255, 0, 0));
			line(exportImage, Point(box.x, box.y + box.height),
					Point(box.x + box.width, box.y + box.height), color);
		}

	}
}
}

int main(int argc, char** argv) {
	try {

		po::options_description desc("Allowed options");
		desc.add_options()("help", "produce help message")
		("l","generate localizer-testdata")
		("lf", po::value<string>(),"original image for marking tags")
		("r","generate recognizer-testdata")
		("rf", po::value<string>(),"path to localizer_testdata [optional if lf is setted]")
		("ruc","use localizer test-configs to generate tags");

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		if (vm.count("help")) {
			cout << desc << "\n";
			return 0;
		}

		string outputpath;

		if (!vm.count("l") && !vm.count("r")) {
			cerr << "Either l or r must be setted" << endl;
			cout << desc << "\n";
			return 1;
		}
		//try to generate localizer-testdata
		if (vm.count("l")) {

			if (vm.count("l") && !vm.count("lf")) {
				cerr << "lf must be setted in the case of using -l" << endl;
				cout << desc << "\n";
				return 1;
			} else {

				string image_name = vm["lf"].as<string>();
				outputpath = testgenerator::generateLocalizerData(image_name);
				if (outputpath.size() == 0) {
					return -1;
				}
			}
		}

		//try to generate recognizer-testdata
		if (vm.count("r")) {
			if (!vm.count("l") && !vm.count("rf")) {
				cerr << "rf or l must be setted in the case of using -r"
				<< endl;
				cout << desc << "\n";
				return 1;
			} else {
				//get paths for localizer-testdata
				if (vm.count("rf")) {
					outputpath = vm["rf"].as<string>();
				}
				bool use_localizer_test_configs = vm.count("ruc");

				string response = testgenerator::generateRecognizerData(
						outputpath, use_localizer_test_configs);

			}
		}

	} catch (std::exception& e) {
		cerr << "error: " << e.what() << "\n";
		return 1;
	} catch (...) {
		cerr << "Exception of unknown type!\n";
	}

	return 0;
}

