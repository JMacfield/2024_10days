#pragma once

#include "Matrix4x4.h"
#include "Vector3.h"
#include "DirectXCommon.h"

struct CameraMatrixData 
{
	Matrix4x4 viewMatrix_;
	
	Matrix4x4 projectionMatrix_;
	Matrix4x4 orthographicMatrix_;
};

struct CameraForGPU 
{
	Vector3 worldPosition;
};


struct Camera 
{
public:
	void Initialize();

	void Update();

	void Transfer();

public:

	ComPtr<ID3D12Resource> bufferResource_;

	float fov_ = 0.45f;
	
	float aspectRatio_ = 0.0f;

	float nearClip_ = 0.1f;
	float farClip_ = 1000.0f;

	Vector3 rotate_ = { 0.0f,0.0f,0.0f };
	
	Vector3 translate_ = { 0.0f,0.0f,0.0f };

	//アフィン行列
	Matrix4x4 worldMatrix_ = {};
	//ビュー行列
	Matrix4x4 viewMatrix_ = {};
	//射影行列
	Matrix4x4 projectionMatrix_ = {};
	//正射影行列
	Matrix4x4 orthographicMatrix_{};

	CameraMatrixData* cameraMatrixData_ = nullptr;

private:

	Vector3 scale_ = { 1.0f,1.0f,1.0f };
};