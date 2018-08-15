#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
using namespace std;

int main() {
	string datain;
	cv::Mat Image;	
	cv::VideoCapture cap = cv::VideoCapture(0);
	if (!cap.read(Image)) {
		cout << "couldn't connect to the cam" << endl;
	}
	auto width = Image.cols;
	auto height = Image.rows;
	while (true){		
		cv::namedWindow("image");
		cv::imshow("image", Image);
		cap.read(Image);
		char key = (char) cv::waitKey(10);
		if (key == 27)
			break;
	}
	cap.release();


	return 0;
}