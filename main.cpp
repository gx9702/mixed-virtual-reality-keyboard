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
#include <iostream>
#include <vector>

#include "supp.hpp"

// susie test

using namespace cv;
using namespace std;

std::queue<std::string> msgQueue;

#define MSG_LEN 2000

int socketSendData(std::string serverIP){
    int sock;
    struct sockaddr_in server;
    char message[MSG_LEN];
    //Create socket
    sock = socket(PF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    cout<< "Socket created " << endl;;
    server.sin_addr.s_addr = inet_addr(serverIP.c_str());
    server.sin_family = PF_INET;
    server.sin_port = htons(5567);
    //Connect to remote server
    if (connect(sock, (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return -1;
    }
    cout << "Connected" << endl;
    // keep communicating with server
    while(1)
    {
        if (msgQueue.empty()) {
            std::this_thread::sleep_for (std::chrono::milliseconds(30));
            continue;
        }
        string msg = msgQueue.front();
        msgQueue.pop();
        memcpy(message, msg.c_str(), MSG_LEN);
        //Send some data
        if(send(sock, message, strlen(message)-1, 0) < 0)
        {
            cout << "Send failed" << endl;;
            break;
        }
    }
    close(sock);
    return 0;
}


int main(int argc, char* argv[]){
    //thread socketThread(socketSendData, "127.0.0.1");
    
    VideoCapture cap(0);
    if (!cap.isOpened()){
        cout << "Cannot open the video cam" << endl;
        return -1;
    }
    
    double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
    
    cout << "Frame size : " << dWidth << " x " << dHeight << endl;
    
    namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
    
    vector<trackedCorner> cornersPreFrame(4);
    vector<trackedCorner> cornersThisFrame(4);
    
    bool firstFrame = true;
    
    while (1){
        Mat frame;
        
        if (!cap.read(frame)){ //if not success, break loop
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }
        
        Mat grayFrame;
        cvtColor(frame, grayFrame, CV_BGR2GRAY);
        
        Mat blurredFrame;
        medianBlur(grayFrame, blurredFrame, 25);
        
        
        Mat mask;
        threshold(blurredFrame, mask, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
        
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(mask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        
        int biggestContourIdx = -1;
        float biggestContourArea = 0;
        cv::Mat drawing(frame);
        for( int i = 0; i< contours.size(); i++ ){
            // drawContours(drawing, contours, i, Scalar(0,255,255), 2, 8, hierarchy, 0, cv::Point());
            float ctArea= cv::contourArea(contours[i]);
            if(ctArea > biggestContourArea){
                biggestContourArea = ctArea;
                biggestContourIdx = i;
            }
        }
        
        if (biggestContourIdx == -1) {
            cout << "No Contour Found!" << endl;
            break;
        }
        
        // compute the rotated bounding rect of the biggest contour! (this is the part that does what you want/need)
        cv::RotatedRect boundingBox = cv::minAreaRect(contours[biggestContourIdx]);
        // one thing to remark: this will compute the OUTER boundary box,
        // so maybe you have to erode dilate if you want something between the ragged lines
        
        // get 4 corners from rect
        getCornersFromRect(&boundingBox, cornersThisFrame);
        if (firstFrame) {
            cornersThisFrame[0].mark = "0";
            cornersThisFrame[1].mark = "1";
            cornersThisFrame[2].mark = "2";
            cornersThisFrame[3].mark = "3";
            firstFrame = false;
        }else{
            // match 4 corners to previous frame's 4 corners
            matchCorrespondingCorners(cornersPreFrame, cornersThisFrame);
        }
        
        string jsonData = generateCornersJSON(cornersThisFrame);
        // msgQueue.push(jsonData);
        // cout << "msgQueue Size: " << msgQueue.size() << endl;
        
        for (int i = 0; i < 4; i++) {
            cv::putText(drawing, cornersThisFrame[i].mark, cornersThisFrame[i].corner, FONT_HERSHEY_PLAIN, 2,  Scalar(0,0,255));
            cv::circle(drawing, cornersThisFrame[i].corner, 5, cv::Scalar(48,48,225), 5, 8 ,0);
        }
        
        cv::line(drawing, cornersThisFrame[0].corner, cornersThisFrame[1].corner, cv::Scalar(50,205,50), 2, 8 ,0);
        cv::line(drawing, cornersThisFrame[1].corner, cornersThisFrame[2].corner, cv::Scalar(50,205,50), 2, 8 ,0);
        cv::line(drawing, cornersThisFrame[2].corner, cornersThisFrame[3].corner, cv::Scalar(50,205,50), 2, 8 ,0);
        cv::line(drawing, cornersThisFrame[3].corner, cornersThisFrame[0].corner, cv::Scalar(50,205,50), 2, 8 ,0);
        
        // if no contour found
        if(biggestContourIdx < 0){
            std::cout << "no contour found" << std::endl;
            return 1;
        }
        
        imshow("MyVideo", drawing); //show the frame in "MyVideo" window
        
        //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        if (waitKey(30) == 27){
            cout << "esc key is pressed by user" << endl;
            break;
        }
        
        // switch frame
        cornersPreFrame = cornersThisFrame;
        cornersThisFrame = vector<trackedCorner>(4);
    }
    return 0;
}