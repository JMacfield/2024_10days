#include "Matrix4x4Calculation.h"



//コタンジェント
float Cot(float theta) {
	return (1.0f / tan(theta));
}


//単位行列を作成する
//斜めの1が並ぶやつ
Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 result = {
		result.m[0][0] = 1.0f,
		result.m[0][1] = 0.0f,
		result.m[0][2] = 0.0f,
		result.m[0][3] = 0.0f,

		result.m[1][0] = 0.0f,
		result.m[1][1] = 1.0f,
		result.m[1][2] = 0.0f,
		result.m[1][3] = 0.0f,

		result.m[2][0] = 0.0f,
		result.m[2][1] = 0.0f,
		result.m[2][2] = 1.0f,
		result.m[2][3] = 0.0f,

		result.m[3][0] = 0.0f,
		result.m[3][1] = 0.0f,
		result.m[3][2] = 0.0f,
		result.m[3][3] = 1.0f,
	};

	return result;
}




//乗算
Matrix4x4 Multiply(const Matrix4x4 m1, const Matrix4x4 m2) {
	Matrix4x4 result = {};
	result.m[0][0] = (m1.m[0][0] * m2.m[0][0]) + (m1.m[0][1] * m2.m[1][0]) + (m1.m[0][2] * m2.m[2][0]) + (m1.m[0][3] * m2.m[3][0]);
	result.m[0][1] = (m1.m[0][0] * m2.m[0][1]) + (m1.m[0][1] * m2.m[1][1]) + (m1.m[0][2] * m2.m[2][1]) + (m1.m[0][3] * m2.m[3][1]);
	result.m[0][2] = (m1.m[0][0] * m2.m[0][2]) + (m1.m[0][1] * m2.m[1][2]) + (m1.m[0][2] * m2.m[2][2]) + (m1.m[0][3] * m2.m[3][2]);
	result.m[0][3] = (m1.m[0][0] * m2.m[0][3]) + (m1.m[0][1] * m2.m[1][3]) + (m1.m[0][2] * m2.m[2][3]) + (m1.m[0][3] * m2.m[3][3]);

	result.m[1][0] = (m1.m[1][0] * m2.m[0][0]) + (m1.m[1][1] * m2.m[1][0]) + (m1.m[1][2] * m2.m[2][0]) + (m1.m[1][3] * m2.m[3][0]);
	result.m[1][1] = (m1.m[1][0] * m2.m[0][1]) + (m1.m[1][1] * m2.m[1][1]) + (m1.m[1][2] * m2.m[2][1]) + (m1.m[1][3] * m2.m[3][1]);
	result.m[1][2] = (m1.m[1][0] * m2.m[0][2]) + (m1.m[1][1] * m2.m[1][2]) + (m1.m[1][2] * m2.m[2][2]) + (m1.m[1][3] * m2.m[3][2]);
	result.m[1][3] = (m1.m[1][0] * m2.m[0][3]) + (m1.m[1][1] * m2.m[1][3]) + (m1.m[1][2] * m2.m[2][3]) + (m1.m[1][3] * m2.m[3][3]);

	result.m[2][0] = (m1.m[2][0] * m2.m[0][0]) + (m1.m[2][1] * m2.m[1][0]) + (m1.m[2][2] * m2.m[2][0]) + (m1.m[2][3] * m2.m[3][0]);
	result.m[2][1] = (m1.m[2][0] * m2.m[0][1]) + (m1.m[2][1] * m2.m[1][1]) + (m1.m[2][2] * m2.m[2][1]) + (m1.m[2][3] * m2.m[3][1]);
	result.m[2][2] = (m1.m[2][0] * m2.m[0][2]) + (m1.m[2][1] * m2.m[1][2]) + (m1.m[2][2] * m2.m[2][2]) + (m1.m[2][3] * m2.m[3][2]);
	result.m[2][3] = (m1.m[2][0] * m2.m[0][3]) + (m1.m[2][1] * m2.m[1][3]) + (m1.m[2][2] * m2.m[2][3]) + (m1.m[2][3] * m2.m[3][3]);

	result.m[3][0] = (m1.m[3][0] * m2.m[0][0]) + (m1.m[3][1] * m2.m[1][0]) + (m1.m[3][2] * m2.m[2][0]) + (m1.m[3][3] * m2.m[3][0]);
	result.m[3][1] = (m1.m[3][0] * m2.m[0][1]) + (m1.m[3][1] * m2.m[1][1]) + (m1.m[3][2] * m2.m[2][1]) + (m1.m[3][3] * m2.m[3][1]);
	result.m[3][2] = (m1.m[3][0] * m2.m[0][2]) + (m1.m[3][1] * m2.m[1][2]) + (m1.m[3][2] * m2.m[2][2]) + (m1.m[3][3] * m2.m[3][2]);
	result.m[3][3] = (m1.m[3][0] * m2.m[0][3]) + (m1.m[3][1] * m2.m[1][3]) + (m1.m[3][2] * m2.m[2][3]) + (m1.m[3][3] * m2.m[3][3]);


	return result;


}



//Scale
//拡縮
Matrix4x4 MakeScaleMatrix(const Vector3 scale) {
	Matrix4x4 result = {};
	result.m[0][0] = scale.x;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = scale.y;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = scale.z;
	result.m[2][3] = 0.0f;


	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;


	return result;
}


//Rotate
#pragma region XYZの個別の回転

Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result = {};

	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = std::sin(radian);
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = -(std::sin(radian));
	result.m[2][2] = std::cos(radian);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result = {};

	result.m[0][0] = std::cos(radian);
	result.m[0][1] = 0.0f;
	result.m[0][2] = -(std::sin(radian));
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = std::sin(radian);
	result.m[2][1] = 0.0f;
	result.m[2][2] = std::cos(radian);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result = {};

	result.m[0][0] = std::cos(radian);
	result.m[0][1] = std::sin(radian);
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = -(std::sin(radian));
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

#pragma endregion 

//AllRotate
//回転
Matrix4x4 MakeRotateXYZMatrix(float radianX, float radianY, float radianZ) {
	Matrix4x4 result;

	Matrix4x4 rotateMatrixX = MakeRotateXMatrix(radianX);
	Matrix4x4 rotateMatrixY = MakeRotateYMatrix(radianY);
	Matrix4x4 rotateMatrixZ = MakeRotateZMatrix(radianZ);



	result = Multiply(rotateMatrixX, Multiply(rotateMatrixY, rotateMatrixZ));

	return result;

}



//Translate
//移動
Matrix4x4 MakeTranslateMatrix(Vector3 translate) {
	Matrix4x4 result = {};
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;


	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1.0f;


	return result;
}



//AffineMatrix
//SRTの融合
//アフィン行列
Matrix4x4 MakeAffineMatrix(const Vector3 scale, const Vector3 rotate, const Vector3 translate) {
	Matrix4x4 result;

	//S
	Matrix4x4 scaleMatrix;
	scaleMatrix = MakeScaleMatrix(scale);

	//R
	Matrix4x4 rotateMatrix;
	rotateMatrix = MakeRotateXYZMatrix(rotate.x, rotate.y, rotate.z);

	Matrix4x4 translateMatrix;
	translateMatrix = MakeTranslateMatrix(translate);


	result = Multiply(scaleMatrix, Multiply(rotateMatrix, translateMatrix));

	return result;
}


//逆行列
Matrix4x4 Inverse(const Matrix4x4 m) {
	float MatrixFormula;
	MatrixFormula =
		+(m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3])
		+ (m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1])
		+ (m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2])

		- (m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1])
		- (m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3])
		- (m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2])

		- (m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3])
		- (m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1])
		- (m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2])

		+ (m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1])
		+ (m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3])
		+ (m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2])

		+ (m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3])
		+ (m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1])
		+ (m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2])


		- (m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1])
		- (m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3])
		- (m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2])

		- (m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0])
		- (m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0])
		- (m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0])

		+ (m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0])
		+ (m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0])
		+ (m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]);

	Matrix4x4 result = {};



	result.m[0][0] = (1 / MatrixFormula) * (
		+(m.m[1][1] * m.m[2][2] * m.m[3][3])
		+ (m.m[1][2] * m.m[2][3] * m.m[3][1])
		+ (m.m[1][3] * m.m[2][1] * m.m[3][2])
		- (m.m[1][3] * m.m[2][2] * m.m[3][1])
		- (m.m[1][2] * m.m[2][1] * m.m[3][3])
		- (m.m[1][1] * m.m[2][3] * m.m[3][2]));

	result.m[0][1] = (1 / MatrixFormula) * (
		-(m.m[0][1] * m.m[2][2] * m.m[3][3])
		- (m.m[0][2] * m.m[2][3] * m.m[3][1])
		- (m.m[0][3] * m.m[2][1] * m.m[3][2])
		+ (m.m[0][3] * m.m[2][2] * m.m[3][1])
		+ (m.m[0][2] * m.m[2][1] * m.m[3][3])
		+ (m.m[0][1] * m.m[2][3] * m.m[3][2]));


	result.m[0][2] = (1 / MatrixFormula) * (
		+(m.m[0][1] * m.m[1][2] * m.m[3][3])
		+ (m.m[0][2] * m.m[1][3] * m.m[3][1])
		+ (m.m[0][3] * m.m[1][1] * m.m[3][2])
		- (m.m[0][3] * m.m[1][2] * m.m[3][1])
		- (m.m[0][2] * m.m[1][1] * m.m[3][3])
		- (m.m[0][1] * m.m[1][3] * m.m[3][2]));

	result.m[0][3] = (1 / MatrixFormula) * (
		-(m.m[0][1] * m.m[1][2] * m.m[2][3])
		- (m.m[0][2] * m.m[1][3] * m.m[2][1])
		- (m.m[0][3] * m.m[1][1] * m.m[2][2])
		+ (m.m[0][3] * m.m[1][2] * m.m[2][1])
		+ (m.m[0][2] * m.m[1][1] * m.m[2][3])
		+ (m.m[0][1] * m.m[1][3] * m.m[2][2]));



	result.m[1][0] = (1 / MatrixFormula) * (
		-(m.m[1][0] * m.m[2][2] * m.m[3][3])
		- (m.m[1][2] * m.m[2][3] * m.m[3][0])
		- (m.m[1][3] * m.m[2][0] * m.m[3][2])
		+ (m.m[1][3] * m.m[2][2] * m.m[3][0])
		+ (m.m[1][2] * m.m[2][0] * m.m[3][3])
		+ (m.m[1][0] * m.m[2][3] * m.m[3][2]));

	result.m[1][1] = (1 / MatrixFormula) * (
		+(m.m[0][0] * m.m[2][2] * m.m[3][3])
		+ (m.m[0][2] * m.m[2][3] * m.m[3][0])
		+ (m.m[0][3] * m.m[2][0] * m.m[3][2])
		- (m.m[0][3] * m.m[2][2] * m.m[3][0])
		- (m.m[0][2] * m.m[2][0] * m.m[3][3])
		- (m.m[0][0] * m.m[2][3] * m.m[3][2]));

	result.m[1][2] = (1 / MatrixFormula) * (
		-(m.m[0][0] * m.m[1][2] * m.m[3][3])
		- (m.m[0][2] * m.m[1][3] * m.m[3][0])
		- (m.m[0][3] * m.m[1][0] * m.m[3][2])
		+ (m.m[0][3] * m.m[1][2] * m.m[3][0])
		+ (m.m[0][2] * m.m[1][0] * m.m[3][3])
		+ (m.m[0][0] * m.m[1][3] * m.m[3][2]));

	result.m[1][3] = (1 / MatrixFormula) * (
		+(m.m[0][0] * m.m[1][2] * m.m[2][3])
		+ (m.m[0][2] * m.m[1][3] * m.m[2][0])
		+ (m.m[0][3] * m.m[1][0] * m.m[2][2])
		- (m.m[0][3] * m.m[1][2] * m.m[2][0])
		- (m.m[0][2] * m.m[1][0] * m.m[2][3])
		- (m.m[0][0] * m.m[1][3] * m.m[2][2]));



	result.m[2][0] = (1 / MatrixFormula) * (
		+(m.m[1][0] * m.m[2][1] * m.m[3][3])
		+ (m.m[1][1] * m.m[2][3] * m.m[3][0])
		+ (m.m[1][3] * m.m[2][0] * m.m[3][1])
		- (m.m[1][3] * m.m[2][1] * m.m[3][0])
		- (m.m[1][1] * m.m[2][0] * m.m[3][3])
		- (m.m[1][0] * m.m[2][3] * m.m[3][1]));

	result.m[2][1] = (1 / MatrixFormula) * (
		-(m.m[0][0] * m.m[2][1] * m.m[3][3])
		- (m.m[0][1] * m.m[2][3] * m.m[3][0])
		- (m.m[0][3] * m.m[2][0] * m.m[3][1])
		+ (m.m[0][3] * m.m[2][1] * m.m[3][0])
		+ (m.m[0][1] * m.m[2][0] * m.m[3][3])
		+ (m.m[0][0] * m.m[2][3] * m.m[3][1]));

	result.m[2][2] = (1 / MatrixFormula) * (
		+(m.m[0][0] * m.m[1][1] * m.m[3][3])
		+ (m.m[0][1] * m.m[1][3] * m.m[3][0])
		+ (m.m[0][3] * m.m[1][0] * m.m[3][1])
		- (m.m[0][3] * m.m[1][1] * m.m[3][0])
		- (m.m[0][1] * m.m[1][0] * m.m[3][3])
		- (m.m[0][0] * m.m[1][3] * m.m[3][1]));

	result.m[2][3] = (1 / MatrixFormula) * (
		-(m.m[0][0] * m.m[1][1] * m.m[2][3])
		- (m.m[0][1] * m.m[1][3] * m.m[2][0])
		- (m.m[0][3] * m.m[1][0] * m.m[2][1])
		+ (m.m[0][3] * m.m[1][1] * m.m[2][0])
		+ (m.m[0][1] * m.m[1][0] * m.m[2][3])
		+ (m.m[0][0] * m.m[1][3] * m.m[2][1]));


	result.m[3][0] = (1 / MatrixFormula) * (
		-(m.m[1][0] * m.m[2][1] * m.m[3][2])
		- (m.m[1][1] * m.m[2][2] * m.m[3][0])
		- (m.m[1][2] * m.m[2][0] * m.m[3][1])
		+ (m.m[1][2] * m.m[2][1] * m.m[3][0])
		+ (m.m[1][1] * m.m[2][0] * m.m[3][2])
		+ (m.m[1][0] * m.m[2][2] * m.m[3][1]));


	result.m[3][1] = (1 / MatrixFormula) * (
		+(m.m[0][0] * m.m[2][1] * m.m[3][2])
		+ (m.m[0][1] * m.m[2][2] * m.m[3][0])
		+ (m.m[0][2] * m.m[2][0] * m.m[3][1])
		- (m.m[0][2] * m.m[2][1] * m.m[3][0])
		- (m.m[0][1] * m.m[2][0] * m.m[3][2])
		- (m.m[0][0] * m.m[2][2] * m.m[3][1]));

	result.m[3][2] = (1 / MatrixFormula) * (
		-(m.m[0][0] * m.m[1][1] * m.m[3][2])
		- (m.m[0][1] * m.m[1][2] * m.m[3][0])
		- (m.m[0][2] * m.m[1][0] * m.m[3][1])
		+ (m.m[0][2] * m.m[1][1] * m.m[3][0])
		+ (m.m[0][1] * m.m[1][0] * m.m[3][2])
		+ (m.m[0][0] * m.m[1][2] * m.m[3][1]));

	result.m[3][3] = (1 / MatrixFormula) * (
		+(m.m[0][0] * m.m[1][1] * m.m[2][2])
		+ (m.m[0][1] * m.m[1][2] * m.m[2][0])
		+ (m.m[0][2] * m.m[1][0] * m.m[2][1])
		- (m.m[0][2] * m.m[1][1] * m.m[2][0])
		- (m.m[0][1] * m.m[1][0] * m.m[2][2])
		- (m.m[0][0] * m.m[1][2] * m.m[2][1]));


	return result;
}



//透視投影行列(正規化する)
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result = {};
	float theta = fovY / 2.0f;

	result.m[0][0] = (1.0f / aspectRatio) * Cot(theta);
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = Cot(theta);
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = farClip/ (farClip- nearClip);
	result.m[2][3] = 1;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = ( - nearClip * farClip) / (farClip - nearClip);
	result.m[3][3] = 0;

	return result;
}


//正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float neaCrlip, float farClip) {
	Matrix4x4 result = {};
	result.m[0][0] = 2.0f /( right - left);
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = 2.0f /( top - bottom);
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = (1 / farClip - neaCrlip);
	result.m[2][3] = 0;

	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = neaCrlip / (neaCrlip - farClip);
	result.m[3][3] = 1;

	return result;
}

//転置行列
Matrix4x4 MakeTransposeMatrix(const Matrix4x4 m) {
	Matrix4x4 result = {};

	result.m[0][0] = m.m[0][0];
	result.m[0][1] = m.m[1][0];
	result.m[0][2] = m.m[2][0];
	result.m[0][3] = m.m[3][0];

	result.m[1][0] = m.m[0][1];
	result.m[1][1] = m.m[1][1];
	result.m[1][2] = m.m[2][1];
	result.m[1][3] = m.m[3][1];

	result.m[2][0] = m.m[0][2];
	result.m[2][1] = m.m[1][2];
	result.m[2][2] = m.m[2][2];
	result.m[2][3] = m.m[3][2];

	result.m[3][0] = m.m[0][3];
	result.m[3][1] = m.m[1][3];
	result.m[3][2] = m.m[2][3];
	result.m[3][3] = m.m[3][3];


	return result;
}
