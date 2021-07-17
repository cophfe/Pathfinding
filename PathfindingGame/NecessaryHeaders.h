#pragma once
#define PHYSICS_TIME_STEP (1.0f/120)
#define PHYSICS_UNIT_SCALE 100
//third party
#include "raylib/raylib.h"
#include "box2d/box2d.h"
//standard
#include <vector>
#include <string>
#include <stdlib.h>
#include <forward_list>
//maths
#include "mlib.h"
#include "Matrix3.h"
#include "Vector2.h"
#include "Vector3.h"

using namespace mlib;