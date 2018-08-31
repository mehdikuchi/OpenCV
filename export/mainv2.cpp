#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
using namespace std;
void getposfrompin(int&, int&);
bool detect(int);
int main() {
	int i;
	wiringPiSetup();

	for (i = 1; i < 13; ++i) {
		pinMode(i, INPUT);
		pullUpDnControl(i, PUD_DOWN);
	}
	pinMode(0, OUTPUT);
	digitalWrite(0, LOW);   //means it has not yet read the bus


	string datain;
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
		char key = (char) cv::waitKey(10);
		if (key == 27)
			break;
	}
	cap.release();
	return 0;
}


void getposfrompin(int& x, int& y) {
	
	while (!detect(0));
	bool id = digitalRead(1) == HIGH ? TRUE : FALSE;
	int x = 0;
	int y = 0;
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
	digitalWrite(0, HIGH);      //The reading of the bus is finished sor the controller should pull the pin0 low and high it again
}


bool detect(int pin) {
	bool ch;
	ch = digitalRead(pin) == LOW ? TRUE : FALSE;
	if (ch) {
		delay(4);
		ch = digitalRead(pin) == HIGH ? TRUE : FALSE;
		if (ch)
			return TRUE;
	}
	return FALSE;
}