#pragma once
#include <vector>

#include "DirectXCommon.h"
#include "Matrix4x4.h"
#include <VertexInfluence.h>
#include <span>
#include <WellForGPU.h>
#include <Skeleton.h>
#include <ModelData.h>
#include <WorldTransform.h>
#include <Camera.h>


struct SkinCluster {

	//SkinClusterを作る
	void Create(const Skeleton& skeleton, const ModelData& modelData);

	//SkinClusterの更新
	void Update(const Skeleton& skeleton);

	
	//SkinClusterStruct skinClusterStruct_ = {};
	std::vector<Matrix4x4> inverseBindPoseMatrices;

	//Influence
	//頂点に対して影響を与えるパラメータ群
	ComPtr<ID3D12Resource> influenceResource_;
	D3D12_VERTEX_BUFFER_VIEW influenceBufferView_;
	std::span<VertexInfluence>mappedInfluence_;





	ComPtr<ID3D12Resource>paletteResource_;
	std::span<WellForGPU> mappedPalette_;
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> paletteSrvHandle_;
	uint32_t srvIndex_;
	
	Skeleton skeleton_;
	


};



