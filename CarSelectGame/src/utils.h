#pragma once
#include <stdlib.h>
#include <cprocessing.h>

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
#define RNG(min, max) (rand() % (max + 1 - min) + min)

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);
int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);
union CP_Vector AngleToVector(float radian_angle);