#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <wiringPi.h>
using namespace std;
void ISR(void);
static volatile bool isready = false;
void getposfrompin(int&, int&);
int main() {
	wiringPiSetup();
	for (int i = 1;  i < 13; i++) {
		pinMode(i, INPUT);
		pullUpDnControl(i, PUD_DOWN);
	}
	wiringPiISR(0, INT_EDGE_RISING, &ISR);	
	cv::Mat Image;	
	cv::VideoCapture cap = cv::VideoCapture(0);
	if (!cap.read(Image)) {
		cout << "Couldn't connect to the camera. Please check the connection to the camera and that it is properly installed" << endl;
	}
	int width;
	int height;
	while (true){		
		cv::namedWindow("image");
		getposfrompin(width, height);
		cv::circle(Image, cv::Point2d(width, height), 40, cv::Scalar(128));
		cv::imshow("image", Image);
		cap.read(Image);
		char key = (char) cv::waitKey(1);
		if (key == 27)
			break;
	}
	cap.release();
	return 0;
}

void ISR(void) {
	isready = true;
}
void getposfrompin(int& x, int& y) {
	x = 0;
	y = 0;
	for(int k=0;k<2;k++){
		while (!isready);   // wait for the data bus to become ready (A rising edge will signal the ready state)
		isready = false;
		bool id = digitalRead(1) == HIGH ? TRUE : FALSE;	
		switch (id)
		{
		case TRUE:
			for (int i = 2; i < 13; i++) {
				x |= (digitalRead(i) == HIGH ? TRUE : FALSE) << i - 2;
			}		
			break;
		case FALSE:
			for (int i = 2; i < 13; i++) {
				y |= (digitalRead(i) == HIGH ? TRUE : FALSE) << i - 2;
			}		
			break;
		default:
			break;
		}
	}
}