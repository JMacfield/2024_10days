#pragma once

#include "Vector3.h"

struct OBB {
	Vector3 center;
	Vector3 orientation[3];
	Vector3 size;
};