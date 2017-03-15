//
//  supp.hpp
//  Opencv-Test
//
//  Created by luxy on 17/3/14.
//  Copyright © 2017年 luxy. All rights reserved.
//

#ifndef supp_hpp
#define supp_hpp

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <queue>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

struct trackedCorner{
    std::string mark;
    cv::Point2f corner;
};

void getCornersFromRect(const cv::RotatedRect *rect, std::vector<trackedCorner> &trackedCorners);
void matchCorrespondingCorners(const std::vector<trackedCorner> &cornerPreFrame, std::vector<trackedCorner> &cornerThisFrame);
std::string generateCornersJSON(const std::vector<trackedCorner> &corners);

#endif /* supp_hpp */
