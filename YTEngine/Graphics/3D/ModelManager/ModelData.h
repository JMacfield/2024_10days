#pragma once
#include <vector>
#include <VertexData.h>
#include "MaterialData.h"
#include <Node.h>
#include <map>
#include "JoinWeightData.h"

struct ModelData {
	std::map<std::string, JointWeightData> skinClusterData;
	std::vector<VertexData> vertices;
	std::vector <uint32_t>indices;
	MaterialData material;
	Node rootNode;
};