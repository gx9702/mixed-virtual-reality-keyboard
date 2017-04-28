#ifndef helper_points_h
#define helper_points_h

#include "supp.h"

using namespace cv;
using namespace std;
Point GetMeanPt(const std::vector<Point> &pts);

void AverageSmoothPts(const vector<vector<Point> > &src, vector<Point> &dst);

#endif

#pragma once
