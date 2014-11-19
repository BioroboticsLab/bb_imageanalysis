/*
 * LocalizerTest.cpp
 *
 *  Created on: 31.07.2014
 *      Author: mareikeziese
 */
#include "RecognizerTest.h"

void RecognizerTest::SetUp() {
}

void RecognizerTest::TearDown() {
}

void excecuteRecognizerTest(Mat gray_image, path filename, path imagename,
		path te_dir_tmp, ostream &out, string configfile = "") {

	path tmp_success, tmp_failed, tmp_notfound;
	Rect rec;
	Mat sub_image;
	Localizer localizer;

	BBList tags = TestHelpers::loadTags(filename.string() + "/tags.txt");

	if (testconfig::TEST_EXPORT_RECOGNIZER_RESULT_SUBIMAGES) {

		tmp_success = path(te_dir_tmp.string() + "/success");
		tmp_failed = path(te_dir_tmp.string() + "/failed");
		tmp_notfound = path(te_dir_tmp.string() + "/notfound");

		create_directory(tmp_success);
		create_directory(tmp_failed);
		create_directory(tmp_notfound);
	}

	Mat notFoundImage;
	gray_image.copySize(notFoundImage);

	if (configfile.size() == 0) {
		localizer = Localizer();
	} else {

		localizer = Localizer();
	}

	vector<Tag> taglist = localizer.process(gray_image);

//	vector<Tag> notFound = vector<Tag *>();
	vector<Tag> found = vector<Tag>();

	for (int i = 0; i < tags.size(); i++) {
		cv::Point p = tags.getPoint(i);
		bool tagFound = false;

		for (int j = 0; j < taglist.size(); j++) {

			Tag tag = taglist[j];

			Rect box = tag.getBox();

			if (TestHelpers::isPossibleCenter(p, box,
					testconfig::TEST_EXPORT_ROI_TOLERANCE)) {

				//tagFound = true;
				found.push_back(tag);
				//taglist.erase(j);
				taglist.erase(taglist.begin() + j);
				break;
			}
		}

	}

	Recognizer recognizer = Recognizer(configfile);



	clock_t f_start = clock();
	vector<Tag> found_new = recognizer.process(found);
	clock_t f_stop = clock();
	double runtime_f = (double) (f_stop - f_start) / CLOCKS_PER_SEC;

	int false_found = 0;

	for (int i = 0; i < found_new.size(); i++) {
		Tag tag = found_new[i];
		if (!tag.isValid()) {

			false_found++;
			if (testconfig::TEST_EXPORT_RECOGNIZER_RESULT_SUBIMAGES) {

				string img_name = tmp_notfound.string() + "/Box"
						+ to_string(i + 1) + ".jpeg";
				imwrite(img_name, tag.getOrigSubImage());
				string img_name = tmp_failed.string() + "/Box_canny"
																				+ to_string(i + 1) + ".jpeg";
																		imwrite(img_name, tag.getCannySubImage());
			}
			if (testconfig::TEST_EXPORT_RECOGNIZER_RESULT_BIGIMAGES
					&& testconfig::TEST_EXPORT_RECOGNIZER_SHOW_NOTFOUND) {

				Rect box = tag.getBox();
				line(notFoundImage, Point(box.x, box.y),
						Point(box.x + box.width, box.y), Scalar(0, 0, 255));
				line(notFoundImage, Point(box.x, box.y),
						Point(box.x, box.y + box.height), Scalar(0, 0, 255));
				line(notFoundImage, Point(box.x + box.width, box.y),
						Point(box.x + box.width, box.y + box.height),
						Scalar(0, 255, 0));
				line(notFoundImage, Point(box.x, box.y + box.height),
						Point(box.x + box.width, box.y + box.height),
						Scalar(0, 255, 0));
				string text = "" + boost::lexical_cast<std::string>(i + 1);
				cv::putText(notFoundImage, text, Point(box.x, box.y),
						FONT_HERSHEY_COMPLEX_SMALL, 1.5, Scalar(0, 0, 255));

			}
		} else {
			if (testconfig::TEST_EXPORT_RECOGNIZER_RESULT_SUBIMAGES) {

				string img_name = tmp_success.string() + "/Box"
						+ to_string(i + 1) + ".jpeg";
				imwrite(img_name, tag.getOrigSubImage());
				string img_name = tmp_failed.string() + "/Box_canny"
																				+ to_string(i + 1) + ".jpeg";
																		imwrite(img_name, tag.getCannySubImage());
			}
			if (testconfig::TEST_EXPORT_RECOGNIZER_RESULT_BIGIMAGES
					&& testconfig::TEST_EXPORT_RECOGNIZER_SHOW_SUCCESS) {

				Rect box = tag.getBox();
				line(notFoundImage, Point(box.x, box.y),
						Point(box.x + box.width, box.y), Scalar(0, 255, 0));
				line(notFoundImage, Point(box.x, box.y),
						Point(box.x, box.y + box.height), Scalar(0, 255, 0));
				line(notFoundImage, Point(box.x + box.width, box.y),
						Point(box.x + box.width, box.y + box.height),
						Scalar(0, 255, 0));
				line(notFoundImage, Point(box.x, box.y + box.height),
						Point(box.x + box.width, box.y + box.height),
						Scalar(0, 255, 0));
				string text = "" + boost::lexical_cast<std::string>(i + 1);
				cv::putText(notFoundImage, text, Point(box.x, box.y),
						FONT_HERSHEY_COMPLEX_SMALL, 1.5, Scalar(0, 255, 0));

			}
		}
	}

	clock_t t_start = clock();
	vector<Tag> wrongFound = recognizer.process(taglist);
	clock_t t_stop = clock();
	double runtime_t = (double) (t_stop - t_start) / CLOCKS_PER_SEC;

	int false_valid = 0;

	for (int i = 0; i < wrongFound.size(); i++) {
		Tag tag = wrongFound[i];

		if (tag.isValid()) {
			false_valid++;
			if (testconfig::TEST_EXPORT_RECOGNIZER_RESULT_SUBIMAGES) {

							string img_name = tmp_failed.string() + "/Box"
									+ to_string(i + 1) + ".jpeg";
							imwrite(img_name, tag.getOrigSubImage());
							string img_name = tmp_failed.string() + "/Box_canny"
									+ to_string(i + 1) + ".jpeg";
							imwrite(img_name, tag.getCannySubImage());
						}
						if (testconfig::TEST_EXPORT_RECOGNIZER_RESULT_BIGIMAGES
								&& testconfig::TEST_EXPORT_RECOGNIZER_SHOW_FAILED) {

							Rect box = tag.getBox();
							line(notFoundImage, Point(box.x, box.y),
									Point(box.x + box.width, box.y), Scalar(255, 0, 0));
							line(notFoundImage, Point(box.x, box.y),
									Point(box.x, box.y + box.height), Scalar(255, 0, 0));
							line(notFoundImage, Point(box.x + box.width, box.y),
									Point(box.x + box.width, box.y + box.height),
									Scalar(0, 255, 0));
							line(notFoundImage, Point(box.x, box.y + box.height),
									Point(box.x + box.width, box.y + box.height),
									Scalar(0, 255, 0));
							string text = "" + boost::lexical_cast<std::string>(i + 1);
							cv::putText(notFoundImage, text, Point(box.x, box.y),
									FONT_HERSHEY_COMPLEX_SMALL, 1.5, Scalar(255, 0, 0));

						}
		}

	}

//	int matchedTags = foundTags - taglist.size();
	cout << "Ergebnisse fr Test " << filename.string() << endl;
	cout << "	Es wurden " << found.size()
			<< "  richtige Tags  an den Recognizer bergeben" << endl;
	cout << " Laufzeit insgesamt : " << runtime_f << "ms " << endl;
	cout << " Laufzeit pro Tag : " << runtime_f / found.size() << "ms " << endl;
	cout << false_found << " Tags wurden nicht erkannt" << endl;
	cout << to_string(100 - (false_found * 100 / found.size()))
			<< "% Erkennungsrate" << endl;

	cout << "Ergebnisse fr Test " << filename.string() << endl;
	cout << "	Es wurden " << taglist.size()
			<< "  falsche Tags  an den Recognizer bergeben" << endl;
	cout << " Laufzeit insgesamt : " << runtime_t << "ms " << endl;
	cout << " Laufzeit pro Tag : " << runtime_t / taglist.size() << "ms "
			<< endl;
	cout << false_valid << " Tags wurden falsch erkannt" << endl;
	cout << to_string(100 - (false_valid * 100 / taglist.size()))
			<< "% Erkennungsrate" << endl;

	out << "Ergebnisse " << filename.stem()
			<< ";" << found.size()
			<< ";"<< runtime_f
			<< ";" << to_string(runtime_f / found.size()) << ";"
			<< false_found << ";"
			<< to_string(100 - (false_found * 100 / found.size())) << ";"
			<< taglist.size() << ";" << runtime_t << ";"
			<< to_string(runtime_t / taglist.size()) << ";" << false_valid
			<< ";" << to_string(100 - (false_valid * 100 / taglist.size())) << endl;

	if (testconfig::TEST_EXPORT_ROI_RESULT_BIGIMAGES) {
		imwrite(te_dir_tmp.string() + "/results.jpeg", notFoundImage);
	}

}

void iterateRecognizerROIFolder(path p, path te_dir, ostream &out_file,
		string configfile = "") {

	path te_dir_tmp;

	if (configfile.size() > 0) {
		path c(configfile);
		out_file << "Tests  mit " << c.stem().string() << "" << endl;
	}

	if (exists(p))    // does p actually exist?
			{
		if (is_directory(p))      // is p a directory?
				{

			typedef vector<path> vec;             // store paths,
			vec v;                                // so we can sort them later

			copy(directory_iterator(p), directory_iterator(), back_inserter(v));

			sort(v.begin(), v.end());         // sort, since directory iteration
											  // is not ordered on some file systems

			for (vec::const_iterator it(v.begin()); it != v.end(); ++it) {
				try {

					path filename = *it;

					te_dir_tmp = path(
							te_dir.string() + filename.stem().string());

					create_directory(te_dir_tmp);

					path imagename(filename.string() + "/original.png");

					if (!exists(imagename)) {

						imagename = path(filename.string() + "/original.jpeg");
						if (!exists(imagename)) {
							imagename = path(
									filename.string() + "/original.jpg");
							if (!exists(imagename)) {
								cerr << "Die Datei wurde nicht gefunden: "
										<< imagename.string();
								break;
							}
						}
					}
					Converter conv = Converter();
					//cout << "Start converter on" << imagename.string() << endl;

					cv::Mat gray_image = conv.process(imagename.string());

					excecuteRecognizerTest(gray_image, filename, imagename,
							te_dir_tmp, out_file, configfile);

				} catch (int e) {
					cout << "An exception occurred. Exception Nr. " << e
							<< '\n';
				}

			}
		}

		else {
			cout << p
					<< " exists, but is neither a regular file nor a directory\n";
		}
	} else
		cout << p << " does not exist\n";
}

TEST(RecognizerTest,TestLocatedTags) {

	path p(testconfig::TEST_RECOGNIZER_PATH); // p reads clearer than argv[1] in the following code
	path p_conf(testconfig::TEST_RECOGNIZER_CONFIGS_PATH);

	path te_dir, tmp_success, tmp_failed, tmp_notfound, te_dir_tmp;
	Rect rec;
	Mat sub_image;
	//if results shall be exported, generate exportfolder

	//path p_ex(testconfig::TEST_RECOGNIZER_RESULT_PATH); // p reads clearer than argv[1] in the following code

	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%F-%I-%M-%S", timeinfo);

	te_dir = path(
			testconfig::TEST_RECOGNIZER_RESULT_PATH + "results_" + buffer
					+ "/");

	create_directory(te_dir);

	//create result csv-file
	boost::iostreams::stream_buffer<boost::iostreams::file_sink> buf(
			te_dir.string() + "results.csv");
	std::ostream out_file(&buf);

	out_file
			<< "Testbild; richtig uebergebene Tag; Laufzeit insgesamt /s; Laufzeit pro Tag/s; nicht erkannte Tags; Erkennungsrate;falsche uebergebene Tags; Laufzeit insgesamt /s; Laufzeit pro Tag /s; falsch erkannte Tags; Erkennungsrate;"
			<< endl;
	if (testconfig::TEST_RECOGNIZER_USE_TESTCONFIGS) {
		path te_dir_tmp;

		if (exists(p_conf))    // does p actually exist?
				{
			if (is_directory(p_conf))      // is p a directory?
					{

				typedef vector<path> vec;             // store paths,
				vec v;                              // so we can sort them later

				copy(directory_iterator(p_conf), directory_iterator(),
						back_inserter(v));

				sort(v.begin(), v.end());     // sort, since directory iteration
											  // is not ordered on some file systems

				for (vec::const_iterator it(v.begin()); it != v.end(); ++it) {
					try {

						path filename = *it;

						te_dir_tmp = path(
								te_dir.string() + filename.stem().string()
										+ "/");
						create_directory(te_dir_tmp);

						iterateRecognizerROIFolder(p, te_dir_tmp, out_file,
								filename.string());

					} catch (int e) {
						cout << "An exception occurred. Exception Nr. " << e
								<< '\n';
					}
				}
			}
		}

	} else {

		iterateRecognizerROIFolder(p, te_dir, out_file);
	}

}
