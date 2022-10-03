#include "utils.h"
#include <math.h>
#include <stdio.h>

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
{
	if ((click_x >= area_center_x && click_x <= area_center_x + area_width) &&
		(click_y >= area_center_y && click_y <= area_center_y + area_height))
		return 1;
	return 0;
}

int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y)
{
	CP_Vector circle = { circle.x = circle_center_x, circle.y = circle_center_y };
	CP_Vector click = { click.x = click_x, click.y = click_y};
	if (CP_Vector_Distance(circle, click) <= diameter * 0.5f)
		return 1;
	return 0;
}

CP_Vector AngleToVector(float radian_angle)
{
	CP_Vector ret;
	ret.x = cos(radian_angle);
	ret.y = sin(radian_angle);
	return ret;
}