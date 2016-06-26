#ifndef __cv_Utility__H__
#define __cv_Utility__H__
#include "cv_header.h"

void resize_roi(int width, int height, cv::Rect &roi, float ratio);//ratio>1放大，ratio<1缩小
void move_roi(int width, int height, cv::Rect &roi, float ratio_x, float ratio_y);//ratio<0向左上

#endif
