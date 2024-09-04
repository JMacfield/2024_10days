#pragma once
#include <Vector3.h>
#include <AABB.h>

struct AccelerationField {
	Vector3 acceleration;
	AABB area;
};