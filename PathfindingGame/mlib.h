#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "raylib/raylib.h"

namespace mlib
{
	const float pi = 3.14159265f;
	const float rad2Deg = 180 / pi;
	const float deg2Rad = pi / 180;

	static float lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
	}
}