#pragma once
#include <cstdint>
#include <ModelData.h>
#include <Camera.h>
#include <SpotLight.h>
#include <PointLight.h>
#include <DirectionalLight.h>

#include  "LightingType.h"
struct WorldTransform;
struct Camera;
struct SkinCluster;

class AnimationModel{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	AnimationModel() {};

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <returns></returns>
	static AnimationModel* Create(uint32_t modelHandle);


	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	/// <param name="skinCluster"></param>
	void Draw(WorldTransform& worldTransform, Camera& camera, SkinCluster& skinCluster);


	/// <summary>
	/// デストラクタ
	/// </summary>
	~AnimationModel() {};





private:


private:
	struct Material {
		Vector4 color;
		int32_t lightingKinds;
		float padding[3];
		Matrix4x4 uvTransform;
		float shininess;
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
	Vector3 lightingDirection_ = { 0.0f,-1.0f,0.0f };
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




	//PixelShaderにカメラの座標を送る為の変数
	ComPtr<ID3D12Resource> cameraResource_ = nullptr;
	CameraForGPU* cameraForGPU_ = {};


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

