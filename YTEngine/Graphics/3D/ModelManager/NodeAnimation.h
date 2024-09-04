#pragma once
#include <vector>
#include "KeyFrame.h"
#include "Vector3.h"

template <typename tValue>
struct AnimationCurve {
	std::vector<KeyFrame<tValue>> keyFrames;
};

struct NodeAnimation {
	AnimationCurve<Vector3> translate;
	AnimationCurve<Quaternion> rotate;
	AnimationCurve<Vector3> scale;
};