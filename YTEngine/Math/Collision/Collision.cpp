#include "Collision.h"



//AABBとPointの当たり判定
bool IsCollisionAABBAndPoint(const AABB& aabb, const Vector3& point){
#pragma region 手前
    //左上
    Vector3 frontLeftTop = { aabb.min.x,aabb .max.y,aabb .min.z};
    //右上
    Vector3 frontRightTop = { aabb.max.x,aabb.max.y,aabb.min.z };
    //右下
    Vector3 frontRightBottom= { aabb.max.x,aabb.min.y,aabb.min.z };
    //左下
    Vector3 frontLeftBottom = { aabb.min.x,aabb.min.y,aabb.min.z };
    
#pragma endregion

#pragma region 奥
    //左上
    Vector3 backLeftTop = { aabb.min.x,aabb.max.y,aabb.max.z };
    //右上
    Vector3 backRightTop = { aabb.max.x,aabb.max.y,aabb.max.z };
    //右下
    Vector3 backRightBottom = { aabb.max.x,aabb.min.y,aabb.max.z };
    //左下
    Vector3 backLeftBottom = { aabb.min.x,aabb.min.y,aabb.max.z };

#pragma endregion

    //横
    //縦
    //奥行
    if ((point.x >= aabb.min.x && point.x <= aabb.max.x) &&
        (point.y >= aabb.min.y && point.y <= aabb.max.y) &&
        (point.z >= aabb.min.z && point.z <= aabb.max.z)) {
        return true;
    }
    else {
        return false;
    }


}

bool IsCollision(const OBB& obb1, const OBB& obb2)
{

	// 分離軸テスト
	for (const auto& axis : obb1.orientation) {
		if (!TestAxis(axis, obb1, obb2)) {
			return false;
		}
	}

	for (const auto& axis : obb2.orientation) {
		if (!TestAxis(axis, obb1, obb2)) {
			return false;
		}
	}

	// OBB1の軸とOBB2の軸に垂直な軸をテスト
	for (const auto& axis : {
			Vector3{obb1.orientation[1].x * obb2.orientation[2].x - obb1.orientation[2].x * obb2.orientation[1].x,
					obb1.orientation[1].y * obb2.orientation[2].y - obb1.orientation[2].y * obb2.orientation[1].y,
					obb1.orientation[1].z * obb2.orientation[2].z - obb1.orientation[2].z * obb2.orientation[1].z},
			Vector3{obb1.orientation[2].x * obb2.orientation[0].x - obb1.orientation[0].x * obb2.orientation[2].x,
					obb1.orientation[2].y * obb2.orientation[0].y - obb1.orientation[0].y * obb2.orientation[2].y,
					obb1.orientation[2].z * obb2.orientation[0].z - obb1.orientation[0].z * obb2.orientation[2].z},
			Vector3{obb1.orientation[0].x * obb2.orientation[1].x - obb1.orientation[1].x * obb2.orientation[0].x,
					obb1.orientation[0].y * obb2.orientation[1].y - obb1.orientation[1].y * obb2.orientation[0].y,
					obb1.orientation[0].z * obb2.orientation[1].z - obb1.orientation[1].z * obb2.orientation[0].z} }) {
		if (!TestAxis(axis, obb1, obb2)) {
			return false;
		}
	}

	return true;
}

bool TestAxis(const Vector3& axis, const OBB& obb1, const OBB& obb2) {

	// OBBの射影を計算
	auto projection1 = OBBProjection(obb1, axis);
	auto projection2 = OBBProjection(obb2, axis);

	// 射影が重なっているかチェック
	return ProjectionOverlap(projection1, projection2);
}

std::pair<float, float> OBBProjection(const OBB& obb, const Vector3& axis) {

	float val = std::sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z); // 正規化
	float newAxis = 0.0f;
	newAxis = newAxis / val;

	// OBB上の頂点を取得
	std::array<Vector3, 8> vertices{};
	for (int i = 0; i < 8; ++i) {
		Vector3 sign = { (i & 1) ? 1.0f : -1.0f, (i & 2) ? 1.0f : -1.0f, (i & 4) ? 1.0f : -1.0f };
		vertices[i] = {
			obb.center.x + obb.orientation[0].x * sign.x * obb.size.x +
						  obb.orientation[1].x * sign.y * obb.size.y +
						  obb.orientation[2].x * sign.z * obb.size.z,
			obb.center.y + obb.orientation[0].y * sign.x * obb.size.x +
						  obb.orientation[1].y * sign.y * obb.size.y +
						  obb.orientation[2].y * sign.z * obb.size.z,
			obb.center.z + obb.orientation[0].z * sign.x * obb.size.x +
						  obb.orientation[1].z * sign.y * obb.size.y +
						  obb.orientation[2].z * sign.z * obb.size.z
		};
	}

	// 頂点を軸に射影
	std::array<float, 8> projections{};
	for (int i = 0; i < 8; ++i) {
		projections[i] = vertices[i].x * axis.x + vertices[i].y * axis.y + vertices[i].z * axis.z;
	}

	auto minmax = std::minmax_element(projections.begin(), projections.end());
	return std::make_pair(*minmax.first, *minmax.second);
}

bool ProjectionOverlap(const std::pair<float, float>& projection1, const std::pair<float, float>& projection2)
{
	return projection1.second >= projection2.first && projection2.second >= projection1.first;
}

Vector3 GetXAxis(const Matrix4x4& m) {
	return { m.m[0][0],m.m[0][1],m.m[0][2] };
}

Vector3 GetYAxis(const Matrix4x4& m) {
	return { m.m[1][0],m.m[1][1],m.m[1][2] };
}

Vector3 GetZAxis(const Matrix4x4& m) {
	return { m.m[2][0],m.m[2][1],m.m[2][2] };
}

void GetOrientations(const Matrix4x4& m, Vector3 orientations[3]) {
	orientations[0] = GetXAxis(m);
	orientations[1] = GetYAxis(m);
	orientations[2] = GetZAxis(m);
}

//bool IsCollision(const OBB& obb_1, const OBB& obb_2) {
//	const int32_t kAxis = 3;
//	for (size_t i = 0; i < kAxis; i++) {
//		// obb_1の軸
//		if (SeparationAxis(obb_1.orientation[i], obb_1, obb_2)) {
//			return false;
//		}
//		// obb_2の軸
//		if (SeparationAxis(obb_2.orientation[i], obb_1, obb_2)) {
//			return false;
//		}
//		for (size_t j = 0; j < kAxis; j++) {
//			Vector3 tmp = Cross(obb_1.orientation[i], obb_2.orientation[j]);
//			if (SeparationAxis(obb_1.orientation[i], obb_1, obb_2)) {
//				return false;
//			}
//		}
//	}
//	return true;
//}

bool SeparationAxis(const Vector3 axis, const OBB obb_1, const OBB obb_2) {// 分離軸
	Vector3 L = axis;
	// 頂点数
	const int32_t kIndex = 8;
	// 頂点格納用配列
	std::vector<Vector3> vertices_1;
	std::vector<Vector3> vertices_2;
	// 配列に頂点を代入
	OBBIndex(obb_1, vertices_1);
	OBBIndex(obb_2, vertices_2);
	// 距離を格納
	float min_1 = DotVector3(vertices_1[0], L);
	float max_1 = min_1;
	float min_2 = DotVector3(vertices_2[0], L);
	float max_2 = min_2;
	for (size_t i = 1; i < kIndex; i++) {
		float dot_1 = DotVector3(vertices_1[i], L);
		float dot_2 = DotVector3(vertices_2[i], L);
		// min/max比べる
		min_1 = (std::min)(min_1, dot_1);
		max_1 = (std::max)(max_1, dot_1);
		min_2 = (std::min)(min_2, dot_2);
		max_2 = (std::max)(max_2, dot_2);
	}
	float L1 = max_1 - min_1;
	float L2 = max_2 - min_2;
	float sumSpan = L1 + L2;
	float longSpan = (std::max)(max_1, max_2) - (std::min)(min_1, min_2);
	// 分離軸である
	if (sumSpan < longSpan) {
		return true;
	}
	return false;
}

void OBBIndex(const OBB& obb, std::vector<Vector3>& output_vertices) {
	std::vector<Vector3> vertices = {
		{obb.size},
		{+obb.size.x, -obb.size.y, -obb.size.z},
		{+obb.size.x, -obb.size.y, +obb.size.z},
		{-obb.size.x, -obb.size.y, +obb.size.z},
		{-obb.size.x, +obb.size.y, -obb.size.z},
		{+obb.size.x, +obb.size.y, -obb.size.z},
		{obb.size},
		{-obb.size.x, +obb.size.y, +obb.size.z},
	};

	Matrix4x4 rotateMatrix = SetRotate(obb.orientation);
	for (auto& vertex : vertices) {
		vertex = ColliTransform(vertex, rotateMatrix);
		vertex = Add(vertex, obb.center);
	}
	output_vertices = vertices;
}

Matrix4x4 SetRotate(const Vector3(&array)[3]) {
	Matrix4x4 rotateMatrix;
	rotateMatrix = MakeIdentity4x4();
	rotateMatrix.m[0][0] = array[0].x;
	rotateMatrix.m[0][1] = array[0].y;
	rotateMatrix.m[0][2] = array[0].z;
	rotateMatrix.m[1][0] = array[1].x;
	rotateMatrix.m[1][1] = array[1].y;
	rotateMatrix.m[1][2] = array[1].z;
	rotateMatrix.m[2][0] = array[2].x;
	rotateMatrix.m[2][1] = array[2].y;
	rotateMatrix.m[2][2] = array[2].z;
	return rotateMatrix;
}

Vector3 ColliTransform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result{ 0.0f, 0.0f, 0.0f };
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
		1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
		1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
		1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
		1.0f * matrix.m[3][3];

	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}