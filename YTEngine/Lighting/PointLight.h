#pragma once
#include <Vector4.h>
#include <Vector3.h>

struct PointLight {
	//ライトの色
	Vector4 color;
	//ライトの位置
	Vector3 position;
	//輝度
	float intensity;

	//ライトに届く最大距離
	float radius;
	//減衰率
	float decay;
	float padding[2];

};
