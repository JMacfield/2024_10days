#include "WorldTransform.h"
#include "Matrix4x4Calculation.h"

#include "DirectXCommon.h"
#include <Camera.h>

void WorldTransform::Initialize() {
	//Resource作成
	bufferResource_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(WorldTransformData)).Get();

	//初期
	scale_ = { 1.0f, 1.0f,1.0f };
	rotate_ = { 0.0f, 0.0f, 0.0f };
	translate_ = { 0.0f, 0.0f, 0.0f };
}

void WorldTransform::Initialize(bool isUseGLTF, Matrix4x4 matrix4x4){
	this->isUseGLTF_ = isUseGLTF;
	this->rootNodeLocalMatrix_ = matrix4x4;

	//Resource作成
	bufferResource_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(WorldTransformData)).Get();

	//初期
	scale_ = { 1.0f, 1.0f,1.0f };
	rotate_ = { 0.0f, 0.0f, 0.0f };
	translate_ = { 0.0f, 0.0f, 0.0f };

}

void WorldTransform::Update() {
	//SRT合成
	worldMatrix_ = MakeAffineMatrix(scale_, rotate_, translate_);
	//逆転置行列
	//ワールド行列を逆転置にする
	Matrix4x4 worldInverseMatrix = Inverse(worldMatrix_);
	//転置にした
	worldInverseTransposeMatrix_ = MakeTransposeMatrix(worldInverseMatrix);

	//親があれば親のワールド行列を掛ける
	if (parent_) {
		worldMatrix_ = Multiply(worldMatrix_, parent_->worldMatrix_);
	}

	Transfer();
}

void WorldTransform::Update(Matrix4x4 animationLocalMatrix){
	//SRT合成
	worldMatrix_ = MakeAffineMatrix(scale_, rotate_, translate_);
	//逆転置行列
	//ワールド行列を逆転置にする
	Matrix4x4 worldInverseMatrix = Inverse(worldMatrix_);
	//転置にした
	worldInverseTransposeMatrix_ = MakeTransposeMatrix(worldInverseMatrix);

	//親があれば親のワールド行列を掛ける
	if (parent_) {
		worldMatrix_ = Multiply(worldMatrix_, parent_->worldMatrix_);
	}

	animationLocalMatrix_ = animationLocalMatrix;

	Transfer();
}



void WorldTransform::Transfer() {
	
	bufferResource_->Map(0, nullptr, reinterpret_cast<void**>(&tranceformationData_));

	//GPU側でWVP計算してくれるようにする
	//通常
	if (isUseGLTF_ == false) {
		tranceformationData_->world = worldMatrix_;
	}
	else {
		
		tranceformationData_->world = worldMatrix_;

	}

	tranceformationData_->normal = worldMatrix_;
	tranceformationData_->worldInverseTranspose = worldInverseTransposeMatrix_;
	bufferResource_->Unmap(0, nullptr);
}
