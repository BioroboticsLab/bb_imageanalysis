/*
 * Testgenerator.cpp
 *
 *  Created on: 30.07.2014
 *      Author: mareikeziese
 */

#include "Testgenerator.h"



namespace decoder {

Testgenerator::Testgenerator() {
	// TODO Auto-generated constructor stub

}

Testgenerator::~Testgenerator() {
	// TODO Auto-generated destructor stub
}

} /* namespace decoder */

decoder::BBList tags;
path directory;
string modified_image;
string tags_file;


int redrawWindow(string windowName, Mat img){
		destroyAllWindows();
	  //Create a window
	     namedWindow("My Window",WINDOW_NORMAL);



	      //set the callback function for any mouse event
	     setMouseCallback("My Window", CallBackFunc, &img);

	      //show the image
	     imshow("My Window", img);

	      // Wait until user press some key
	     int keyst = waitKey(0);

	     // press a to refresh window
	     if (keyst == 97) {
	    	 cout << "Complete Testgeneration" <<endl;
	     	    	 cout << "Generate Image" <<endl;
	     	         imwrite(modified_image, img);
	     	     	 cout << "Generate Tags" <<endl;
	     	     	std::ofstream ofs(tags_file);
	     	     	boost::archive::text_oarchive oa(ofs);
	     	     	oa & tags;
	     	     	//for ( unsigned int i = 0; i < tags.size(); i++) {
	     	     	//	oa << tags[i];
	     	     	//}



	    	//press ESC to abort generation;
	     }else if(keyst == 27) {
	    	 cerr << "Aborting Testgeneration";
	     }else{
	    	 redrawWindow(windowName, img);
	     }

	     return 0;
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{

	Mat* img = (Mat*)userdata;

     if  ( event == cv::EVENT_LBUTTONDOWN )
     {

          cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
          Point center(x,y);
          tags.AddPoint(center);
          circle(*img, center,20,Scalar(255,0,0), 3);

          string text = "Box " + boost::lexical_cast<std::string>(tags.size());
          		cv::putText(*img, text, center, FONT_HERSHEY_COMPLEX_SMALL, 1.5,
          				Scalar(0, 255, 0));

         //redrawWindow("My Window", *img);
     }

}





int main(int argc, char** argv)
{

     // Read image from file
	char* image_name = argv[1];

	path file(image_name);

	string testname = file.stem().string();

	directory = path(config::TEST_DATA_EXPORT_PATH+ testname);

	if(is_directory(directory)){
		cerr << "Ein Test für diese Eingabe besteht bereits" << endl;
		return 0;
	}
	create_directory(directory);

	path copyfile(directory.string() + "/original" + file.extension().string());

	copy_file(file, copyfile);
	modified_image = directory.string() + "/modified" + file.extension().string();
	tags_file = directory.string() + "/tags.txt";








     Mat img = imread(image_name);

      //if fail to read the image
     if ( img.empty() )
     {
          cout << "Error loading the image" << endl;
          return -1;
     }

     tags= decoder::BBList();
     int response = redrawWindow("My Window", img);
/*
      //Create a window
     namedWindow("My Window", 1);

      //set the callback function for any mouse event
     setMouseCallback("My Window", CallBackFunc, &img);

      //show the image
     imshow("My Window", img);

      // Wait until user press some key
     waitKey(0);*/

      return response;

}
