#pragma once
#include "Vector4.h"
#include "Vector2.h"
#include "Vector3.h"

//頂点データの拡張
struct VertexData {
	Vector4 position;
	Vector2 texCoord;
	Vector3 normal;
};