#pragma once
#include <stdInt.h>
#include "../Math/Vector/Vector3.h"

namespace OtherCode{

	// 桁数取得 (UI用)
	int32_t GetDigits(int32_t num);

	// 指数補間
	float ExponentialInterpolation(float a, float b, float t, float k);
	Vector3 ExponentialInterpolation(Vector3 a, Vector3 b, float t, float k);

};


