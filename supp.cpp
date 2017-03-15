//
//  supp.cpp
//  Opencv-Test
//
//  Created by luxy on 17/3/14.
//  Copyright © 2017年 luxy. All rights reserved.
//

#include "supp.hpp"


void getCornersFromRect(const cv::RotatedRect *rect, std::vector<trackedCorner> &trackedCorners){
    if (trackedCorners.size() != 4) {
        std::cout << "[ERROR] Size error in getCornersFromRect!" << std::endl;
    }
    cv::Point2f corners[4];
    rect->points(corners);
    for (int i = 0; i < 4; i++) {
        trackedCorners[i].corner = corners[i];
    }
}

void matchCorrespondingCorners(const std::vector<trackedCorner> &cornerPreFrame, std::vector<trackedCorner> &cornerThisFrame){
    if (cornerPreFrame.size() != 4 || cornerThisFrame.size() != 4) {
        std::cout << "[ERROR] Size error in matchCorrespondingCorners!" << std::endl;
    }
    //
    bool isMatched[4];
    isMatched[0] = isMatched[1] = isMatched[2] = isMatched[3] = false;
    // find nearest corner
    for (int i = 0; i < 4; i++) {
        cv::Point2f corner = cornerThisFrame[i].corner;
        double minDis = 9999;
        int minIdx = -1;
        for (int j = 0; j < 4; j++) {
            if (isMatched[j]) {
                continue;
            }
            double thisDis = cv::norm(corner - cornerPreFrame[j].corner);
            if (thisDis < minDis) {
                minDis = thisDis;
                minIdx = j;
            }
        }
        cornerThisFrame[i].mark = cornerPreFrame[minIdx].mark;
        isMatched[minIdx] = true;
    }
}

std::string generateCornersJSON(const std::vector<trackedCorner> &corners){
    std::string res = "{ ";
    for (int i = 0; i < corners.size(); i++) {
        if (i != 0) {
            res.append(", ");
        }
        res.append("\"" + corners[i].mark + "\":{\"x\":" + std::to_string(corners[i].corner.x)
                   + ", \"y\":" + std::to_string(corners[i].corner.y) + "}");
    }
    res.append(" }");
    return res;
}


