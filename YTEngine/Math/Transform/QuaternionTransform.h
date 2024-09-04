#pragma once
#include <Vector3.h>
#include <Quaternion.h>

struct QuaternionTransform {
	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;
};