/*
 * LocalizerTest.cpp
 *
 *  Created on: 31.07.2014
 *      Author: mareikeziese
 */
#include "LocalizerTest.h"

class LocalizerTest: public ::testing::Test {

private:
public:
	void SetUp() {
	}

	/**
	 * function that is called immediately after a test
	 */
	void TearDown() {
	}

};
/**
 * loads Point of a prepared test-case
 *
 * @param string filename
 * @return
 */
BBList loadTags(string filename) {

	std::ifstream ifs(filename);
	boost::archive::text_iarchive ia(ifs);
	// read class state from archive
	BBList tags;
	ia & tags;
	return tags;

}

TEST(LocalizerTest,TestLocatedTags) {

	path p(testconfig::TEST_ROI_PATH); // p reads clearer than argv[1] in the following code

	path te_dir, tmp_success, tmp_failed, tmp_notfound, te_dir_tmp;
	Rect rec;
	Mat sub_image;
	//if results shall be exported, generate exportfolder

	path p_ex(testconfig::TEST_ROI_RESULT_PATH); // p reads clearer than argv[1] in the following code

	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%F-%I-%M-%S", timeinfo);

	te_dir = path(testconfig::TEST_ROI_RESULT_PATH + "results_" + buffer + "/");

	create_directory(te_dir);

	if (exists(p))    // does p actually exist?
			{
		if (is_regular_file(p))        // is p a regular file?
			cout << p << " size is " << file_size(p) << '\n';

		else if (is_directory(p))      // is p a directory?
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

					if (testconfig::TEST_EXPORT_ROI_RESULT_SUBIMAGES) {

						tmp_success = path(te_dir_tmp.string() + "/success");
						tmp_failed = path(te_dir_tmp.string() + "/failed");
						tmp_notfound = path(te_dir_tmp.string() + "/notfound");

						create_directory(tmp_success);
						create_directory(tmp_failed);
						create_directory(tmp_notfound);
					}

					//cout << "Execute on " << filename.string() << endl;

					BBList tags = loadTags(filename.string() + "/tags.txt");

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
					Mat notFoundImage;

					Localizer localizer = Localizer();
					//cout << "Start localizer" << endl;

					vector<BoundingBox> bb = localizer.process(gray_image);
					if (testconfig::TEST_EXPORT_ROI_RESULT_BIGIMAGES) {
						notFoundImage = imread(imagename.string());
						string sobel_image = te_dir_tmp.string()
								+ "/sobel.jpeg";
						string blob_image = te_dir_tmp.string() + "/blob.jpeg";
						string canny_image = te_dir_tmp.string()
								+ "/canny.jpeg";

						imwrite(sobel_image, localizer.getSobel());
						imwrite(blob_image, localizer.getBlob());
						imwrite(canny_image, localizer.getCannyMap());
					}

					vector<int> notFound = vector<int>();

					int rightTags = tags.size();
					int foundTags = bb.size();

					for (int i = 0; i <= tags.size(); i++) {
						Point p = tags.getPoint(i);
						bool tagFound = false;

						for (int j = 0; j <= bb.size(); j++) {
							if (bb[j].isPossibleCenter(p,
									testconfig::TEST_EXPORT_ROI_TOLERANCE)) {
								tagFound = true;
								if (testconfig::TEST_EXPORT_ROI_RESULT_SUBIMAGES) {
									string img_name = tmp_success.string()
											+ "/Box" + to_string(i + 1)
											+ ".jpeg";
									imwrite(img_name, bb[j].sub_image_orig_);
								}
								if (testconfig::TEST_EXPORT_ROI_RESULT_BIGIMAGES
										&& testconfig::TEST_EXPORT_ROI_SHOW_SUCCESS) {
									Rect box = bb[j].box_;
									line(notFoundImage, Point(box.x, box.y),
											Point(box.x + box.width, box.y),
											Scalar(0, 255, 0));
									line(notFoundImage, Point(box.x, box.y),
											Point(box.x, box.y + box.height),
											Scalar(0, 255, 0));
									line(notFoundImage,
											Point(box.x + box.width, box.y),
											Point(box.x + box.width,
													box.y + box.height),
											Scalar(0, 255, 0));
									line(notFoundImage,
											Point(box.x, box.y + box.height),
											Point(box.x + box.width,
													box.y + box.height),
											Scalar(0, 255, 0));
									string text = ""
											+ boost::lexical_cast<std::string>(
													i + 1);
									cv::putText(notFoundImage, text, p,
											FONT_HERSHEY_COMPLEX_SMALL, 1.5,
											Scalar(0, 255, 0));

								}
								bb.erase(bb.begin() + j);
								break;
							}
						}
						if (!tagFound) {
							if (testconfig::TEST_EXPORT_ROI_RESULT_SUBIMAGES) {
								int imagesize = config::LOCALIZER_MAXTAGSIZE
										- 30;

								rec = Rect(p.x - (imagesize / 2),
										p.y - (imagesize / 2), imagesize,
										imagesize);
								//if rectangle is outside the possible image-coordinates => resize rectangle
								if ((rec.x + rec.width) > gray_image.cols) {
									rec.x -= abs(
											rec.x + rec.width
													- gray_image.cols);
								}

								if ((rec.y + rec.height) > gray_image.rows) {
									rec.y -= abs(
											rec.y + rec.height
													- gray_image.rows);
								}

								if (rec.x < 0) {
									rec.x = 0;
								}

								if (rec.y < 0) {
									rec.y = 0;
								}

								string img_name = tmp_notfound.string() + "/"
										+ to_string(rec.x) + "_"
										+ to_string(rec.y) + "__"
										+ to_string(rec.width) + "_"
										+ to_string(rec.height) + "__" + "__"
										+ to_string(p.x) + "_" + to_string(p.y)
										+ "__Box" + to_string(i + 1) + ".jpeg";
								sub_image = Mat(gray_image, rec);
								imwrite(img_name, sub_image);
							}
							if (testconfig::TEST_EXPORT_ROI_RESULT_BIGIMAGES
									&& testconfig::TEST_EXPORT_ROI_SHOW_NOTFOUND) {
								circle(notFoundImage, p, 20, Scalar(0, 0, 255),
										3);
								string text = ""
										+ boost::lexical_cast<std::string>(
												i + 1);
								cv::putText(notFoundImage, text, p,
										FONT_HERSHEY_COMPLEX_SMALL, 1.5,
										Scalar(0, 0, 255));
							}

							notFound.push_back(i);

						}
						//EXPECT_TRUE(tagFound) << "Box " << i << " wurde nicht gefunden" << endl;
					}
					if (testconfig::TEST_EXPORT_ROI_RESULT_SUBIMAGES
							|| testconfig::TEST_EXPORT_ROI_RESULT_BIGIMAGES) {
						for (int j = 0; j <= bb.size(); j++) {

//generate subimage
							if (testconfig::TEST_EXPORT_ROI_RESULT_SUBIMAGES) {
								string img_name_f = tmp_failed.string() + "/"
										+ to_string(bb[j].box_.x) + "_"
										+ to_string(bb[j].box_.y) + "__"
										+ to_string(bb[j].box_.width) + "_"
										+ to_string(bb[j].box_.height)
										+ ".jpeg";

								imwrite(img_name_f, bb[j].sub_image_orig_);
							}
							if (testconfig::TEST_EXPORT_ROI_RESULT_BIGIMAGES
									&& testconfig::TEST_EXPORT_ROI_SHOW_FAILED) {
								Rect box = bb[j].box_;
								line(notFoundImage, Point(box.x, box.y),
										Point(box.x + box.width, box.y),
										Scalar(255, 0, 0));
								line(notFoundImage, Point(box.x, box.y),
										Point(box.x, box.y + box.height),
										Scalar(255, 0, 0));
								line(notFoundImage,
										Point(box.x + box.width, box.y),
										Point(box.x + box.width,
												box.y + box.height),
										Scalar(255, 0, 0));
								line(notFoundImage,
										Point(box.x, box.y + box.height),
										Point(box.x + box.width,
												box.y + box.height),
										Scalar(255, 0, 0));

							}
						}

					}
					if (testconfig::TEST_EXPORT_ROI_RESULT_BIGIMAGES) {
						imwrite(te_dir_tmp.string() + "/results.jpeg",
								notFoundImage);
					}
					int matchedTags = foundTags - bb.size();
					cout << "Ergebnisse f�r Test " << filename.string() << endl;
					cout << "	Es wurden " << foundTags << " gefunden" << endl;
					cout << "	Es wurden " << rightTags
							<< " Tags  per Hand markiert" << endl;
					cout << "	davon wurden " << endl;

					cout << matchedTags << " 		richtig erkannt ("<< to_string(matchedTags*100/rightTags)<<" % der markierten Tags)" << endl;
					cout << rightTags - matchedTags << " 		nicht erkannt"
							<< endl;
					/*cout << "[";
					 for (std::vector<int>::const_iterator i = notFound.begin();
					 i != notFound.end(); ++i)
					 cout << *i << "; ";
					 cout << "]" << endl;*/
					;
					cout << bb.size() << " 		falsch erkannt ("<< to_string(bb.size()*100/foundTags)<<" % der gefundenen Tags)" << endl;

				} catch (int e) {
					cout << "An exception occurred. Exception Nr. " << e
							<< '\n';
				}

			}
		}

		else
			cout << p
					<< " exists, but is neither a regular file nor a directory\n";
	} else
		cout << p << " does not exist\n";
}

