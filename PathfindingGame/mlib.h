#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include "raylib/raylib.h"
#include "box2d//box2d.h"

namespace mlib
{
	//useful consts
	constexpr float pi = 3.14159265f;
	constexpr float rad2Deg = 180 / pi;
	constexpr float deg2Rad = pi / 180;

	//custom lerp function
	static float lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
	}
}