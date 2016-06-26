#include "cv_utility.h"

void resize_roi(int width, int height, cv::Rect &roi, float ratio)
{
	int stx = roi.x;
	int sty = roi.y;
	int edx = roi.x + roi.width;
	int edy = roi.y + roi.height;

	ratio = ratio - 1;
	ratio = ratio / 2.0;
	stx -= roi.width * ratio;
	sty -= roi.height * ratio;
	edx += roi.width * ratio;
	edy += roi.height * ratio;

	if (stx < 0)stx = 0;
	if (sty < 0)sty = 0;
	if (edx >= width)edx = width - 1;
	if (edy >= height)edy = height - 1;

	roi.x = stx;
	roi.y = sty;
	roi.width = edx - stx;
	roi.height = edy - sty;
}

void move_roi(int width, int height, cv::Rect &roi, float ratio_x, float ratio_y)
{
	int stx = roi.x;
	int sty = roi.y;
	int edx = roi.x + roi.width;
	int edy = roi.y + roi.height;

	stx += roi.width * ratio_x;
	sty += roi.height * ratio_y;
	edx += roi.width * ratio_x;
	edy += roi.height * ratio_y;

	if (stx < 0)stx = 0;
	if (sty < 0)sty = 0;
	if (edx >= width)edx = width - 1;
	if (edy >= height)edy = height - 1;

	roi.x = stx;
	roi.y = sty;
	roi.width = edx - stx;
	roi.height = edy - sty;
}