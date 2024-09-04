#pragma once
#include "Vector4.h" 
#include "Vector3.h"

struct DirectionalLight {
	//ライトの色
	Vector4 color;
	//ライトの向き
	Vector3 direction;
	//輝度
	float intensity;
};
