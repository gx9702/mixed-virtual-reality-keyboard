//
//  main.cpp
//  Opencv-Test
//
//  Created by luxy on 17/3/14.
//  Copyright © 2017年 luxy. All rights reserved.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <thread>
#include <algorithm>
#include <iostream>
#include <vector>
#include <math.h>
#include <WS2tcpip.h>

#include "supp.h"
#include "helper_points.h"

using namespace cv;
using namespace std;

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

std::queue<std::string> msgQueue;

#define MSG_LEN 2000

const string trackbarWindowName = "Trackbars";
int H_MIN = 0;
int H_MAX = 255;
int S_MIN = 0;
int S_MAX = 255;
int V_MIN = 0;
int V_MAX = 47;

void on_trackbar(int, void*)
{//This function gets called whenever a
 // trackbar position is changed


}
void createTrackbars() {

	namedWindow(trackbarWindowName, 0);
	//create memory to store trackbar name on window

	/*char TrackbarName[50];
	sprintf(TrackbarName, "H_MIN", H_MIN);
	sprintf(TrackbarName, "H_MAX", H_MAX);
	sprintf(TrackbarName, "S_MIN", S_MIN);
	sprintf(TrackbarName, "S_MAX", S_MAX);
	sprintf(TrackbarName, "V_MIN", V_MIN);
	sprintf(TrackbarName, "V_MAX", V_MAX);*/

	createTrackbar("H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar);
	createTrackbar("H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar);
	createTrackbar("S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar);
	createTrackbar("S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar);
	createTrackbar("V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar);
	createTrackbar("V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar);
}

int socketSendData(std::string serverIP) {
	WSADATA wsa;
	SOCKET s;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}
	printf("Initialised.\n");


	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
		return 1;
	}
	cout << "Socket created " << endl;

	struct sockaddr_in server;
	char message[MSG_LEN];

	//server.sin_addr.s_addr = inet_addr(serverIP.c_str());
	inet_pton(AF_INET, serverIP.c_str(), &server.sin_addr);
	server.sin_family = PF_INET;
	server.sin_port = htons(5567);
	//Connect to remote server
	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return -1;
	}
	cout << "Connected" << endl;
	// keep communicating with server
	while (1)
	{
		if (msgQueue.empty()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
			continue;
		}
		string msg = msgQueue.front();
		msgQueue.pop();
		memcpy(message, msg.c_str(), MSG_LEN);
		//Send some data
		if (send(s, message, strlen(message), 0) < 0)
		{
			cout << "Send failed" << endl;
			break;
		}
	}
	return 0;
}


int main(int argc, char* argv[]) {
	thread socketThread(socketSendData, "127.0.0.1");

	int fontFace2 = cv::FONT_HERSHEY_DUPLEX;
	int fontFace3 = cv::FONT_HERSHEY_TRIPLEX;

	VideoCapture cap(0);
	if (!cap.isOpened()) {
		cout << "Cannot open the video cam" << endl;
		return -1;
	}

	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of gggggggframes of the video
	int dFPS = cap.get(CV_CAP_PROP_FPS);
	cout << dFPS << endl;

	cout << "Frame size : " << dWidth << " x " << dHeight << endl;

	//namedWindow("real_time_frame", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
	//namedWindow("gaussian_smooting", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
	//namedWindow("erode_dialate", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
	//namedWindow("median_filter", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
	//namedWindow("threshold_binary_image", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
	//namedWindow("find_contours", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
	//namedWindow("min_rotated_bounding_rect", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
	//namedWindow("rect_4_corners", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

	namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
												// corners tracking variables
	vector<TrackedCorner> cornersPreFrame(4);
	vector<TrackedCorner> cornersThisFrame(4);
	bool firstFrame = true;

	// initial area size settings
	double kbWidth = 0, kbLength = 0, kbArea = 0;
	int initCnt = 0, initSetLimit = 50;

	// keyboard information judge
	KeyboardInfo kbPreFrame, kbThisFrame;

	// remove skin Mat
	Mat hsv(Size(dWidth, dHeight), CV_8UC3);
	Mat bw(Size(dWidth, dHeight), CV_8UC1);
	Mat gray(Size(dWidth, dHeight), CV_8UC1);

	vector<vector<Point>> cache;
	int cnt = 0, len_ave = 7;

	//createTrackbars();

	while (1) {
		bool switchFrame = true;

		Mat frame;

		if (!cap.read(frame)) { //if not success, break loop
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}
		//imshow("real_time_frame", frame);


		Size kSize;
		kSize.height = 3;
		kSize.width = 3;
		double sigma = 0.3*(3 / 2 - 1) + 0.8;
		GaussianBlur(frame, frame, kSize, sigma, 0.0, 4);
		//imshow("gaussian_smooting", frame);

		//cvtColor(frame, gray, CV_RGB2GRAY);
		
		cvtColor(frame, hsv, CV_RGB2HSV);
		inRange(hsv, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), bw);
		//imshow("threshold_binary_image", bw);

		Mat Erode(Size(dWidth, dHeight), CV_8UC1);
		cv::erode(bw, Erode, cv::Mat(), cv::Point(-1, -1));

		Mat Dialate(Size(dWidth, dHeight), CV_8UC1);
		cv::dilate(Erode, Dialate, cv::Mat(), cv::Point(-1, -1), 2);
		//imshow("erode_dialate", Dialate);
		////Mat grayFrame;
		Mat blurredFrame;
		medianBlur(Dialate, blurredFrame, 25);
		//imshow("tmp", blurredFrame);
		//imshow("median_filter", blurredFrame);

		Mat mask;
		threshold(blurredFrame, mask, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

		std::vector<std::vector<cv::Point>> contours(10);
		std::vector<cv::Vec4i> hierarchy(10);
		cv::findContours(mask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

		int biggestContourIdx = -1;
		float biggestContourArea = 0;
		cv::Mat drawing(frame);
		for (int i = 0; i< contours.size(); i++) {
			drawContours(drawing, contours, i, Scalar(0,255,255), 2, 8, hierarchy, 0, cv::Point());
			float ctArea = cv::contourArea(contours[i]);
			if (ctArea > biggestContourArea) {
				biggestContourArea = ctArea;
				biggestContourIdx = i;
			}
		}
		//imshow("find_contours", drawing);
		if (biggestContourIdx == -1) {
			cout << "No Contour Found!" << endl;
			continue;
		}

		// compute the rotated bounding rect of the biggest contour! (this is the part that does what you want/need)
		cv::RotatedRect boundingBox = cv::minAreaRect(contours[biggestContourIdx]);
		// one thing to remark: this will compute the OUTER boundary box,
		// so maybe you have to erode dilate if you want something between the ragged lines

		// get 4 corners from rect
		getCornersFromRect(&boundingBox, cornersThisFrame);

		////// smooth video with weighted average
		//vector<Point> cornerPoint;
		//for (int i = 0; i < 4; i++) {
		//	cornerPoint.push_back(Point(cornersThisFrame[i].corner.x, cornersThisFrame[i].corner.y));
		//}
		//cache.push_back(cornerPoint);
		//cnt++;
		//if (cnt > len_ave)
		//	cache.erase(cache.begin());
		//AverageSmoothPts(cache, cornerPoint);
		//for (int i = 0; i < 4; i++) {
		//	cornersThisFrame[i].corner.x = cornerPoint[i].x;
		//	cornersThisFrame[i].corner.y = cornerPoint[i].y;
		//}

		if (firstFrame) {
			cornersThisFrame[0].mark = "A";
			cornersThisFrame[1].mark = "B";
			cornersThisFrame[2].mark = "C";
			cornersThisFrame[3].mark = "D";
			kbThisFrame = getRectInfoFromCorners(cornersThisFrame);
			kbPreFrame = getRectInfoFromCorners(cornersThisFrame);
			firstFrame = false;
		}
		else {
			// match 4 corners to previous frame's 4 corners
			matchCorrespondingCorners(cornersPreFrame, cornersThisFrame);
			kbThisFrame = getRectInfoFromCorners(cornersThisFrame);
		}

		// mark the frame
		// mark corners
		for (int i = 0; i < 4; i++) {
			std::string cornerMark = cornersThisFrame[i].mark;
			cv::Point2f tmpCorner(cornersThisFrame[i].corner);
			cv::putText(drawing, cornerMark + " (" + std::to_string((int)tmpCorner.x) + "," + std::to_string((int)tmpCorner.y) + ")", tmpCorner, fontFace3, 1, Scalar(0, 0, 255));
			cv::circle(drawing, cornersThisFrame[i].corner, 5, cv::Scalar(48, 48, 225), 5, 8, 0);
		}

		cv::line(drawing, cornersThisFrame[0].corner, cornersThisFrame[1].corner, cv::Scalar(50, 205, 50), 2, 8, 0);
		cv::line(drawing, cornersThisFrame[1].corner, cornersThisFrame[2].corner, cv::Scalar(50, 205, 50), 2, 8, 0);
		cv::line(drawing, cornersThisFrame[2].corner, cornersThisFrame[3].corner, cv::Scalar(50, 205, 50), 2, 8, 0);
		cv::line(drawing, cornersThisFrame[3].corner, cornersThisFrame[0].corner, cv::Scalar(50, 205, 50), 2, 8, 0);

		// initialize rectangle width, height and area
		if (++initCnt < initSetLimit) {
			//cv::putText(drawing, "Initializing ... " + std::to_string(initSetLimit - initCnt),
			//	Point2f(50, 50), fontFace2, 1.5, cv::Scalar(50, 205, 50));
			kbWidth += kbThisFrame.width;
			kbLength += kbThisFrame.length;
		}
		else if (initCnt == initSetLimit) {
			kbWidth = kbWidth / initSetLimit;
			kbLength = kbLength / initSetLimit;
			kbArea = kbWidth * kbLength;
			initCnt++;
		}
		else {
			//cv::putText(drawing, "Area change: " + std::to_string((kbThisFrame.area - kbArea) / kbArea),
			//	Point2f(50, 50), fontFace2, 1.5, cv::Scalar(50, 205, 50));
			// meaningful frame
			// send transform into to socket
			if (kbThisFrame.area < kbArea*0.5 || kbThisFrame.area > kbArea*1.5) {
				switchFrame = false;
			}
			else {
				TransformRect transRect = getTransBetweenNearFrames(kbPreFrame, kbThisFrame);
				string jsonData = generateTransformJSON(transRect);
				cout << transRect.posX << ", " << transRect.posY << endl;
				// cout << transRect.rotZ << endl;
				// string jsonData = generateCornersJSON(cornersThisFrame);
				msgQueue.push(jsonData);
				cout << "msgQueue Size: " << msgQueue.size() << endl;
			}
		}

		imshow("MyVideo", frame); //show the frame in "MyVideo" window

									//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		if (waitKey(30) == 27) {
			cout << "esc key is pressed by user" << endl;
			break;
		}
		if (switchFrame) {
			cornersPreFrame = cornersThisFrame;
			cornersThisFrame = vector<TrackedCorner>(4);
			kbPreFrame = kbThisFrame;
			kbThisFrame = KeyboardInfo();
		}
	}
	std::terminate();
	return 0;
}
