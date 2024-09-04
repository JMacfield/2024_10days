#pragma once
#include <QuaternionTransform.h>
#include "Matrix4x4.h"
#include "Node.h"

#include <string>
#include <vector>
#include <optional>


struct Joint {
	//Transform情報
	QuaternionTransform transform;
	//LocalMatrix
	Matrix4x4 localMatrix;
	//SkeletonSpaceでの変換行列
	Matrix4x4 skeletonSpaceMatrix;
	//名前
	std::string name;
	//子JointのIndexのリスト。いなければ空。
	std::vector<int32_t> cheldren;
	//自信のIndex
	int32_t index;
	//親JointのIndex。いなければnull
	//optional...ある型にbool値で無効か有効かの情報を追加した型
	std::optional<int32_t>parent;

};


int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);