#pragma once

#include "Matrix4x4.h"
#include "Vector3.h"

#include "DirectXCommon.h"

struct WorldTransformData {
	Matrix4x4 world;
	Matrix4x4 normal;
	Matrix4x4 worldInverseTranspose;
};


struct WorldTransform {
public:
#pragma region メンバ関数

	//初期化
	void Initialize();

	/// <summary>
	/// glTFを使う時
	/// </summary>
	void Initialize(bool isUseGLTF,Matrix4x4 matrix4x4);

	//行列を計算・転送する
	void Update();

	void Update(Matrix4x4 animationLocalMatrix);

	//ペアレントの設定
	void SetParent(const WorldTransform* parent) {
		parent = parent_;
	}

	//glTF用
	void  SetRootNodeLocalMatrix(Matrix4x4 matrix4x4) {
		this->rootNodeLocalMatrix_ = matrix4x4;
	}

	Vector3 GetWorldPosition() { return { worldMatrix_.m[3][0],worldMatrix_.m[3][1],worldMatrix_.m[3][2] }; }

private:
	//転送
	void Transfer();


#pragma endregion

#pragma region メンバ変数


public:
	
	bool isUseGLTF_ = false;

	//スケール
	Vector3 scale_ = { 1.0f, 1.0f, 1.0f };
	//回転
	Vector3 rotate_ = { 0.0f, 0.0f, 0.0f };
	//座標
	Vector3 translate_ = { 0.0f, 0.0f, 0.0f };

	//glTFからローカル行列を受け取る
	Matrix4x4 rootNodeLocalMatrix_ = {};

	Matrix4x4 animationLocalMatrix_ = {};

	//定数バッファ
	ComPtr<ID3D12Resource> bufferResource_;

	WorldTransformData* tranceformationData_ = nullptr;

	//ワールド行列へ
	Matrix4x4 worldMatrix_ = {};
	//逆転置行列
	Matrix4x4 worldInverseTransposeMatrix_ = {};

	//親となるワールド変換へのポインタ
	const WorldTransform* parent_ = nullptr;

#pragma endregion

};

