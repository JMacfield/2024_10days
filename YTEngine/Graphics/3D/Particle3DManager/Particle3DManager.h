#pragma once
#include <unordered_map>
#include <string>
#include <list>
#include <random>

#include "DirectXCommon.h"
#include "MaterialData.h"
#include <Particle.h>
#include "VertexData.h"
#include <Camera.h>
#include <AccelerationField.h>
#include <DirectionalLight.h>
#include "Transform.h"

struct ParticleEmitterStruct {
	//エミッタのTransform;
	Transform transform;
	//発生数
	uint32_t count;
	//発生頻度
	float frequency;
	//頻度用時刻
	float frequencyTime;
};

struct ParticleGrounp {
	//マテリアルデータ
	MaterialData materialData;
	//テクスチャ用SRVインデックス
	uint32_t textureSrvindex;

	//パーティクルのリスト
	std::list<Particle>particles;

	//インスタンシングデータ用SRVインデックス
	uint32_t instancingSrvIndex;
	//インスタンシングリソース
	ComPtr<ID3D12Resource>instancingResource;
	//インスタンス数
	uint32_t instanceNumber;
	//インスタンシングデータに書き込むためのポインタ
	ParticleForGPU* instancingData;

	ParticleEmitterStruct emitter_ = {};
};


class Particle3DManager final {
private:
	//コンストラクタ
	Particle3DManager() = default;

	//デストラクタ
	~Particle3DManager() = default;

public:
	//インスタンス
	static Particle3DManager* GetInstance();

	//コピーコンストラクタ禁止
	Particle3DManager(const Particle3DManager& particle3DManager) = delete;

	//代入演算子を無効にする
	Particle3DManager& operator=(const Particle3DManager& particle3DManager) = delete;


public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 放出
	/// </summary>
	/// <param name="名前"></param>
	/// <param name="テクスチャハンドル"></param>
	/// <param name="座標"></param>
	/// <param name="数"></param>
	void Emit(const std::string name,uint32_t textureHandle, const Vector3& position, uint32_t count);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="カメラ"></param>
	void Draw(Camera& camera);

	/// <summary>
	/// 指定したものを消す
	/// </summary>
	/// <param name="名前"></param>
	void DeleteElement(const std::string name);

private:
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera"></param>
	void Update(Camera& camera);

	void CreateParticleGroup(const std::string name, uint32_t textureHandle, Vector3 position,uint32_t count);


	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="ランダムエンジン"></param>
	/// <param name="座標"></param>
	/// <returns></returns>
	Particle MakeNewParticle(std::mt19937& randomEngine,Vector3 position);

	//エミッタ
	std::list<Particle> Emission(Vector3 podition, uint32_t count, std::mt19937& randomEngine);


private:
	//ユーザーが付けるグループ名をキーとしてグループを持てるようにする
	std::unordered_map<std::string, ParticleGrounp>particleGroup_;



	//C++でいうsrandみたいなやつ
	//ランダムエンジンの初期化
	 //C++でいうsrandみたいなやつ
	std::random_device seedGenerator;
	std::mt19937 rand_ = {};

	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	std::vector<VertexData> vertices_{};

	//マテリアル用のリソースを作る
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	//色関係のメンバ変数
	Vector4 materialColor_ = { 1.0f,1.0f,1.0f,1.0f };
	//基本はtrueで
	bool isEnableLighting_ = true;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	ComPtr<ID3D12Resource>instancingResource_ = nullptr;

	//Lighting用
	ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;
	//色
	Vector4 directionalLightColor_ = { 1.0f,1.0f,1.0f,1.0f };
	float directionalLightIntensity_ = 3.0f;

	
	//方向
	Vector3 lightingDirection_ = { 0.0f,-1.0f,0.0f };


	const uint32_t  MAX_INSTANCE_NUMBER_ = 100;

	//描画すべきインスタンス数
	uint32_t numInstance_ = 0;
	bool isBillBordMode_ = true;

	//エミッタの設定
	//Emitter emitter_ = {};
	const float DELTA_TIME = 1.0f / 60.0f;

	//フィールド
	bool isSetField_ = true;
	AccelerationField accelerationField_ = {};


	//パーティクル
	ParticleForGPU* instancingData_ = nullptr;
};

