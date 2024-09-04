#pragma once
#include <Vector3.h>
#include <Quaternion.h>

template <typename tValue>

struct KeyFrame {
	//キーフレームの値
	tValue value;
	//キーフレームの時刻(秒)
	float time;
};



//Vector3
using KeyFrameVector3 = KeyFrame<Vector3>;
//Quaternion
using KeyFrameQuaternion = KeyFrame<Quaternion>;

