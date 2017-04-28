#include "helper_points.h"


cv::Point GetMeanPt(const std::vector<cv::Point> &pts)
{
	assert(!pts.empty());
	cv::Point p = pts[0];
	const int num = int(pts.size());
	for (int i = 1; i < num; i++) {
		p.x += pts[i].x;
		p.y += pts[i].y;
	}
	p.x /= num;
	p.y /= num;
	return p;
}

void AverageSmoothPts(const std::vector<std::vector<cv::Point>> &src, std::vector<cv::Point> &dst)
{
	const int num = int(src.size());
	float weights[] = { 1.f, .8f, .64f, .512f, .4096f, .32768f, .262144f };
	assert(num <= sizeof(weights) / sizeof(float));
	float sumwei = weights[0];
	const int len = int(dst.size());
	dst = src[num - 1];
	const cv::Point mean0 = GetMeanPt(src[num - 1]);
	for (int n = 1; n < num; n++)
	{
		sumwei += weights[n];
		const std::vector<cv::Point> &curpts = src[num - 1 - n];
		const cv::Point mean_cur = GetMeanPt(curpts);
		for (int i = 0; i < len; i++) {
			dst[i].x += (int)(curpts[i].x + mean0.x - mean_cur.x) * weights[n];
			dst[i].y += (int)(curpts[i].y + mean0.y - mean_cur.y) * weights[n];
		}
	}

	for (int i = 0; i < len; i++) {
		dst[i].x /= sumwei;
		dst[i].y /= sumwei;
	}
}
