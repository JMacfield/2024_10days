#pragma once
#include <Matrix4x4.h>

struct WellForGPU {
	//位置用
	Matrix4x4 skeletonSpaceMatrix;
	//法線用
	Matrix4x4 skeletonSpaceIncerseTransposeMatrix;
};