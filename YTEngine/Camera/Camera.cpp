#include "Camera.h"
#include <Matrix4x4Calculation.h>
#include "WinApp.h"

#include "Camera.h"
#include <Matrix4x4Calculation.h>
#include "DirectXCommon.h"

void Camera::Initialize() 
{
	//Resource作成
	bufferResource_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(CameraMatrixData)).Get();

	aspectRatio_ = float(WinApp::GetInstance()->GetClientWidth()) / float(WinApp::GetInstance()->GetClientHeight());

	//初期
	scale_ = { 1.0f, 1.0f,1.0f };
	rotate_ = { 0.0f, 0.0f, 0.0f };
	translate_ = { 0.0f, 0.0f, -9.8f };

	worldMatrix_ = MakeAffineMatrix(scale_, rotate_, translate_);
	
	viewMatrix_ = Inverse(worldMatrix_);
	
	//射影を計算
	projectionMatrix_ = MakePerspectiveFovMatrix(fov_, aspectRatio_, nearClip_, farClip_);
	
	//正射影行列(正規化)を計算
	orthographicMatrix_ = MakeOrthographicMatrix(0, 0, float(WinApp::GetInstance()->GetClientWidth()), float(WinApp::GetInstance()->GetClientHeight()), 0.0f, 100.0f);
}

void Camera::Update() 
{
	worldMatrix_ = MakeAffineMatrix(scale_, rotate_, translate_);

	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(fov_, aspectRatio_, nearClip_, farClip_);
	
	orthographicMatrix_ = MakeOrthographicMatrix(0, 0, float(WinApp::GetInstance()->GetClientWidth()), float(WinApp::GetInstance()->GetClientHeight()), 0.0f, 100.0f);

	Transfer();
}

void Camera::Transfer() 
{
	bufferResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraMatrixData_));
	
	cameraMatrixData_->viewMatrix_ = viewMatrix_;
	cameraMatrixData_->projectionMatrix_ = projectionMatrix_;
	cameraMatrixData_->orthographicMatrix_ = orthographicMatrix_;
	
	bufferResource_->Unmap(0, nullptr);
}