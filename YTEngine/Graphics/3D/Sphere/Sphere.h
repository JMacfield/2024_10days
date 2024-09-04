#pragma once

#include "DirectXCommon.h"
#include "ConvertLog.h"
#include <DirectXTex.h>

#include "Vector4.h"
#include "Matrix4x4.h"
#include "Matrix4x4Calculation.h"
#include "Transform.h"
#include "VertexData.h"
#include "Material.h"
#include "TransformationMatrix.h"
#include "DirectionalLight.h"
#include <SphereShape.h>

class Sphere {
public:
	
	//コンストラクタ
	Sphere();

	//初期化
	void Initialize();

	
	//更新処理
	//最終手段。ここにImGui入れる
	void Update();


	//描画
	//左上、右上、左下、右下
	void Draw(SphereShape sphereCondtion, Transform transform,Matrix4x4 viewMatrix,Matrix4x4 projectionMatrix, Vector4 color);


	//解放
	void Release();

	//デストラクタ
	~Sphere();


private:

	
	//Resource作成の関数化
	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);

	//頂点バッファビューを作成する
	void GenerateVertexBufferView();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

	




private:

	//DirectX内にある情報を取り入れる
	DirectXCommon* directXSetup_ = nullptr;



	//関数用
	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	D3D12_RESOURCE_DESC vertexResourceDesc_{};

	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSphere_;

	//初期化
	//頂点データ
	ComPtr<ID3D12Resource> vertexResourceSphere_ = nullptr;
	VertexData* vertexDataSphere_ = nullptr;


	//constにする意味はあったのだろうか
	uint32_t descriptorSizeSRV_=0u;
	uint32_t descriptorSizeRTV_=0u;
	uint32_t descriptorSizeDSV_=0u;


	//マテリアル用のリソースを作る
	ComPtr<ID3D12Resource> materialResourceSphere_ = nullptr;
	Material* materialData_ = nullptr;

	//球を描画するとき用のTransformationMatrix用のリソースを作る。
	//Matrix4x4 1つ分サイズを用意する
	ComPtr<ID3D12Resource> transformationMatrixResourceSphere_ = nullptr;
	TransformationMatrix* transformationMatrixDataSphere_ = nullptr;

	//Lighting用
	ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;





	//分割数
	const int32_t SUBDIVISION_ = 16;

};