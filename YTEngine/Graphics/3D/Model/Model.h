#pragma once
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>
#include <array>
#include <memory>
#include <DirectXTex.h>
#include <d3dx12.h>

#include "Matrix4x4.h"
#include "Vector4.h"
#include "TransformationMatrix.h"
#include "Matrix4x4Calculation.h"
#include "VertexData.h"


#include "MaterialData.h"
#include "ModelData.h"
#include "DirectionalLight.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "ModelManager.h"
#include <PointLight.h>
#include <SpotLight.h>

#include "Skeleton.h"
#include <SkinCluster.h>
#include "LightingType.h"

class Model {
public:

	//コンストラクタ
	Model()=default;

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="modelHandle">モデルハンドル</param>
	/// <returns></returns>
	static Model* Create(uint32_t modelHandle);


	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform">ワールドトランスフォーム</param>
	/// <param name="camera">カメラ</param>
	void Draw(WorldTransform& worldTransform, Camera& camera);



	/// <summary>
	/// デストラクタ
	/// </summary>
	~Model()=default;




public:

	//透明度の変更
	void SetColor(Vector4 color) {
		this->materialColor_ = color;
	}

	void SetTransparency(float transparency) {
		this->materialColor_.w = transparency;
	}


	//アクセッサのまとめ
	void SetBlendMode(int32_t blendmode) {
		blendModeNumber_ = blendmode;
	}

#pragma region Lightingの設定
	void SetLighting(bool enableLighting) {
		this->selectLighting_ = enableLighting;
	}
	//方向
	void SetDirectionalLightDirection(Vector3 direction) {
		this->lightingDirection_ = direction;
	}
	//Intensity
	void SetDirectionalLightIntensity(float intensity) {
		this->directionalLightIntensity_ = intensity;
	}

#pragma endregion

#pragma region PointLightの設定
	void SetPointLightColor(Vector4 color) {
		this->pointLightData_.color = color;
	}
	void SetPointLightPosition(Vector3 position) {
		this->pointLightData_.position = position;
	}
	void SetPointLightIntensity(float intensity) {
		this->pointLightData_.intensity = intensity;
	}
	void SetPointLightRadius(float radius) {
		this->pointLightData_.radius = radius;
	}
	void SetPointLightDecay(float decay) {
		this->pointLightData_.decay = decay;
	}

#pragma endregion

#pragma region SpotLightの設定
	//色
	void SetSpotLightColor(Vector4 color) {
		this->spotLightData_.color = color;
	}

	//位置
	void SetSpotLightPosition(Vector3 position) {
		this->spotLightData_.position = position;
	}

	//輝度
	void SetSpotLightIntensity(float intensity) {
		this->spotLightData_.intensity = intensity;
	}
	
	//方向
	void SetSpotLightDirection(Vector3 direction) {
		this->spotLightData_.direction = direction;
	}

	//届く距離
	void SetSpotLightDistance(float distance) {
		this->spotLightData_.distance = distance;
	}

	//減衰率
	void SetSpotLightDecay(float decay) {
		this->spotLightData_.decay = decay;
	}
	//Falloffの開始の角度の設定
	void SetCosFalloffStart(float cos) {
		this->spotLightData_.cosFallowoffStart = cos;
	}
	//余弦
	void SetSpotLightCosAngle(float cosAngle) {
		this->spotLightData_.cosAngle = cosAngle;
	}

#pragma endregion


	Matrix4x4 GetAnimationLocalMatrix() {
		return animationLocalMatrix_;
	}






private:
	struct Material {
		Vector4 color;
		int32_t lightingKinds;
		float padding[3];
		Matrix4x4 uvTransform;
		float shininess;
	};

	struct SkinningEnable {
		int32_t isSkinning;
	};

private:
	//頂点リソースを作る
	//頂点バッファビューを作成する
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	//インデックス
	ComPtr<ID3D12Resource> indexResource_ = nullptr;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

#pragma region なくしたい


	//マテリアル用のリソースを作る
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	Material* materialData_ = nullptr;
	//色関係のメンバ変数
	Vector4 materialColor_ = { 1.0f,1.0f,1.0f,1.0f };
	//Ligtingをするかどうか
	//基本はtrueで
	int32_t selectLighting_ = Directional;

	//DirectionalLight
	ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;
	//方向
	Vector3 lightingDirection_ = {0.0f,-1.0f,0.0f};
	//Lightの色
	Vector4 lightColor_ = { 1.0f,1.0f,1.0f,1.0f };
	float directionalLightIntensity_ = 1.0f;
	float shininess_ = 100.0f;

	//PointLight
	ComPtr<ID3D12Resource> pointLightResource_ = nullptr;
	PointLight* pointLightMapData_ = {};
	PointLight pointLightData_ = {};


	//SpotLight
	ComPtr<ID3D12Resource> spotLightResource_ = nullptr;
	SpotLight* spotLightMapData_ = {};
	SpotLight spotLightData_ = {};



#pragma endregion

	//PixelShaderにカメラの座標を送る為の変数
	ComPtr<ID3D12Resource> cameraResource_ = nullptr;
	CameraForGPU* cameraForGPU_ = {};

	//Skinningするかどうか
	ComPtr<ID3D12Resource> skinningResource_ = nullptr;
	SkinningEnable* skinningData_ = {};
	SkinningEnable isSkinning_ = {};

	//アニメーションを再生するときに使う時間
	float animationTime_ = 0.0f;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//モデルハンドル
	uint32_t modelHandle_ = 0u;
	ModelData modelData_ = {};

	//アニメーションのローカル座標
	//後々シェーダーで渡す
	Matrix4x4 animationLocalMatrix_ = {};

	//デフォルトはα加算
	int32_t blendModeNumber_ = 1;


	bool isAnimation_ = false;




};