//
//  supp.hpp
//  Opencv-Test
//
//  Created by luxy on 17/3/14.
//  Copyright © 2017年 luxy. All rights reserved.
//

#ifndef supp_h
#define supp_h

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp> 
#include <iostream>
#include <queue>
#include <vector>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <math.h>

#define PI 3.14159
template <typename T> class Vector2D
{
private:
	T x;
	T y;

public:
	explicit Vector2D(const T& x = 0, const T& y = 0) : x(x), y(y) {}
	Vector2D(const Vector2D<T>& src) : x(src.x), y(src.y) {}
	virtual ~Vector2D() {}

	// Accessors
	inline T X() const { return x; }
	inline T Y() const { return y; }
	inline T X(const T& x) { this->x = x; }
	inline T Y(const T& y) { this->y = y; }

	// Vector arithmetic
	inline Vector2D<T> operator-() const
	{
		return Vector2D<T>(-x, -y);
	}

	inline Vector2D<T> operator+() const
	{
		return Vector2D<T>(+x, +y);
	}

	inline Vector2D<T> operator+(const Vector2D<T>& v) const
	{
		return Vector2D<T>(x + v.x, y + v.y);
	}

	inline Vector2D<T> operator-(const Vector2D<T>& v) const
	{
		return Vector2D<T>(x - v.x, y - v.y);
	}

	inline Vector2D<T> operator*(const T& s) const
	{
		return Vector2D<T>(x*s, y*s);
	}

	// Dot product
	inline T operator*(const Vector2D<T>& v) const
	{
		return x*v.x + y*v.y;
	}

	// l-2 norm
	inline T norm() const { return sqrt(x*x + y*y); }

	// inner angle (radians)
	static T angle(const Vector2D<T>& v1, const Vector2D<T>& v2) {
		return acos((v1 * v2) / (v1.norm() * v2.norm()));
	}
};

struct TrackedCorner {
	std::string mark;
	cv::Point2f corner;
	TrackedCorner() : mark(""), corner(cv::Point2f(0, 0)) {}
};

struct KeyboardInfo {
	std::vector<TrackedCorner> corners;
	double width;
	double length;
	cv::Point2f centerGravity;
	double area;
	KeyboardInfo() : width(0), length(0), centerGravity(cv::Point2f(0, 0)), area(0) {}
};

struct TransformRect {
	double posX, posY, posZ;
	double rotX, rotY, rotZ;
	TransformRect() : posX(0), posY(0), posZ(0), rotX(0), rotY(0), rotZ(0) {}
};


void getCornersFromRect(const cv::RotatedRect *rect, std::vector<TrackedCorner> &trackedCorners);
void matchCorrespondingCorners(const std::vector<TrackedCorner> &cornerPreFrame, std::vector<TrackedCorner> &cornerThisFrame);
std::string generateCornersJSON(const std::vector<TrackedCorner> &corners);
std::string generateTransformJSON(const TransformRect transRect);
KeyboardInfo getRectInfoFromCorners(const std::vector<TrackedCorner> &corners);
TransformRect getTransBetweenNearFrames(const KeyboardInfo &kbPreFrame, const KeyboardInfo &kbThisFrame);
double getPolarAngle(const Vector2D<double> &v);
int getRotationDirection(double fromPolarAngle, double toPolarAngle);

#endif /* supp_hpp */

/* test

cv::Point2i p1(3, 5);
cv::Point2i p2(3, 10);
cv::Point2i p3(10, 5);
cv::Point2i vec = p1 - p2;
cv::Point2i changedP3 = p3 + vec;
cout << vec.x << ", " << vec.y << endl;
cout << changedP3.x << ", " << changedP3.y << endl;

Vector2D<double> p1_1(p1.x, p1.y);
Vector2D<double> p2_1(p2.x, p2.y);
Vector2D<double> p3_1(p3.x, p3.y);

double rad = Vector2D<double>::angle(p2_1-p1_1, p3_1-p1_1);
double deg = rad * 180.0 / M_PI;

std::cout << "rad = " << rad << "\tdeg = " << deg << std::endl;


// 0, -5
// 10, 0
// rad = 1.5708	deg = 90

code*/
#pragma once
