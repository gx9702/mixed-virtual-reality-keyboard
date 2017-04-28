//
//  supp.cpp
//  Opencv-Test
//
//  Created by luxy on 17/3/14.
//  Copyright © 2017年 luxy. All rights reserved.
//

#include "supp.h"

void getCornersFromRect(const cv::RotatedRect *rect, std::vector<TrackedCorner> &trackedCorners) {
	if (trackedCorners.size() != 4) {
		std::cout << "[ERROR] Size error in getCornersFromRect!" << std::endl;
		return;
	}
	cv::Point2f corners[4];
	rect->points(corners);
	for (int i = 0; i < 4; i++) {
		trackedCorners[i].corner = corners[i];
	}
}

void matchCorrespondingCorners(const std::vector<TrackedCorner> &cornerPreFrame, std::vector<TrackedCorner> &cornerThisFrame) {
	if (cornerPreFrame.size() != 4 || cornerThisFrame.size() != 4) {
		std::cout << "[ERROR] Size error in matchCorrespondingCorners!" << std::endl;
		return;
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

std::string generateCornersJSON(const std::vector<TrackedCorner> &corners) {
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

std::string generateTransformJSON(const TransformRect transRect) {
	std::string res = "";
	//res.append("{\"posX\":" + std::to_string(transRect.posX) + ", \"posY\":" + std::to_string(transRect.posY) + "}");
	res.append(std::to_string(transRect.posX) + "," + std::to_string(transRect.posY) + "," + std::to_string(transRect.rotZ));
	return res;
}


KeyboardInfo getRectInfoFromCorners(const std::vector<TrackedCorner> &corners) {
	KeyboardInfo kbInfo;
	if (corners.size() != 4) {
		std::cout << "[ERROR] Size error in getRectInfoFromCorners!" << std::endl;
		return kbInfo;
	}
	// get corners
	kbInfo.corners = std::vector<TrackedCorner>(corners);
	// get width and length
	double edge1 = cv::norm(corners[0].corner - corners[1].corner);
	double edge2 = cv::norm(corners[1].corner - corners[2].corner);
	double edge3 = cv::norm(corners[2].corner - corners[0].corner);
	std::vector<double> sortList{ edge1, edge2, edge3 };
	std::sort(sortList.begin(), sortList.end());
	kbInfo.width = sortList[0];
	kbInfo.length = sortList[1];

	// get area size
	kbInfo.area = kbInfo.width * kbInfo.length;

	// get gravity center
	int gcX = (int)(corners[0].corner.x + corners[1].corner.x + corners[2].corner.x + corners[3].corner.x) >> 2;
	int gcY = (int)(corners[0].corner.y + corners[1].corner.y + corners[2].corner.y + corners[3].corner.y) >> 2;
	kbInfo.centerGravity = cv::Point2f(gcX, gcY);
	return kbInfo;
}


TransformRect getTransBetweenNearFrames(const KeyboardInfo &kbPreFrame, const KeyboardInfo &kbThisFrame) {
	TransformRect transRes;
	if (kbThisFrame.corners.size() != 4 || kbPreFrame.corners.size() != 4) {
		std::cout << "[ERROR] Size error in getTransBetweenNearFrames!" << std::endl;
		return transRes;
	}
	// find corresponding points
	cv::Point2f cornerAThisFrame, cornerAPreFrame;
	for (int i = 0; i < 4; i++) {
		if (kbThisFrame.corners[i].mark == "A") {
			cornerAThisFrame = kbThisFrame.corners[i].corner;
		}
		if (kbPreFrame.corners[i].mark == "A") {
			cornerAPreFrame = kbPreFrame.corners[i].corner;
		}
	}

	// calculate gravity center move vector
	cv::Point2f transVec = kbThisFrame.centerGravity - kbPreFrame.centerGravity;
	cv::Point2f cornerAPreFramePosed = cornerAPreFrame + transVec;

	// calculate the angle
	Vector2D<double> pCenter(kbThisFrame.centerGravity.x, kbThisFrame.centerGravity.y);
	Vector2D<double> pA(cornerAThisFrame.x, cornerAThisFrame.y);
	Vector2D<double> pA_(cornerAPreFramePosed.x, cornerAPreFramePosed.y);

	Vector2D<double> vA = pA - pCenter;
	Vector2D<double> vA_ = pA_ - pCenter;

	int rotDirection = getRotationDirection(getPolarAngle(vA_), getPolarAngle(vA));
	if (rotDirection == 0) {
		std::cout << "[ERROR] calculation error in getRotationDirection" << std::endl;
		return transRes;
	}

	double rad = Vector2D<double>::angle(vA, vA_);
	double deg = rad * 180.0 / PI;
	deg = deg > 180 ? (360 - deg) : deg;

	transRes.posX = transVec.x;
	transRes.posY = transVec.y;
	transRes.rotZ = -rotDirection * deg;

	// meaningless shaking
	if (abs(transVec.x) <= 1 && abs(transVec.y) <= 1)
	{
		transRes.posX = 0;
		transRes.posY = 0;
		transRes.rotZ = 0;
	}

	return transRes;
}

double getPolarAngle(const Vector2D<double> &v) {
	return atan2(v.Y(), v.X()) * 180 / PI;
}

int getRotationDirection(double fromPolarAngle, double toPolarAngle) {
	if (fromPolarAngle >= 0 && toPolarAngle >= 0) {
		if (fromPolarAngle >= toPolarAngle) {
			return 1;
		}
		else {
			return -1;
		}
	}
	if (fromPolarAngle <= 0 && toPolarAngle <= 0) {
		return -getRotationDirection(-fromPolarAngle, -toPolarAngle);
	}
	if (fromPolarAngle >= 0 && toPolarAngle <= 0) {
		if (fromPolarAngle < 90) {
			return 1;
		}
		else {
			return -1;
		}
	}
	if (fromPolarAngle <= 0 && toPolarAngle >= 0) {
		return -getRotationDirection(-fromPolarAngle, -toPolarAngle);
	}
	return 0;
}
