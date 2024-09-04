#pragma once
#include <cstdint>
#include <array>


const uint32_t NUM_MAX_INFLUENCE = 4;
struct VertexInfluence {
	std::array<float, NUM_MAX_INFLUENCE> weights;
	std::array<int32_t, NUM_MAX_INFLUENCE> jointIndices;
};