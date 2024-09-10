#pragma once
#include <stdInt.h>

namespace OtherCode{

	// 桁数取得 (UI用)
	int32_t GetDigits(int32_t num);

	// 指数補間
	float ExponentialInterpolation(float a, float b, float t, float k);

};


