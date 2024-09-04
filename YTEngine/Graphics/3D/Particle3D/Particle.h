#pragma once
#include <Transform.h>
#include "Vector4.h"
#include <Matrix4x4.h>

//CPU側
struct Particle {
	Transform transform;
	Vector3  velocity;
	Vector4 color;
	float lifeTime;
	float currentTime;
};

struct ParticleForGPU {
	Matrix4x4  World;
	Vector4 color;
};
