#pragma once
#define PHYSICS_TIME_STEP (1.0f/120) // how long a physics time step is. physics is iterated 120 times per second. since the physics is really fast compared to the rest this is acceptable
#define PHYSICS_UNIT_SCALE 100 //used to convert between opengl rendering and box2d physics 100 pixels = 1 meter
#ifdef _DEBUG
#define DRAW_DEBUG 1
#endif
//third party
#include "raylib/raylib.h"
#include "box2d/box2d.h"
//standard
#include <vector>
#include <string>
#include <stdlib.h>
#include <forward_list>
#include <map>
#include <unordered_map>
//maths
#include "mlib.h"
#include "Matrix3.h"
#include "Vector2.h"
#include "Vector3.h"

using namespace mlib;