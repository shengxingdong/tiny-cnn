#ifndef __cv_Utility__H__
#define __cv_Utility__H__
#include "cv_header.h"

void resize_roi(int width, int height, cv::Rect &roi, float ratio);//ratio>1�Ŵ�ratio<1��С
void move_roi(int width, int height, cv::Rect &roi, float ratio_x, float ratio_y);//ratio<0������

#endif
