#pragma once
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>
#include <array>
#include <memory>
#include <list>
#include <random>

#include <DirectXTeX.h>
#include "Matrix4x4.h"
#include <Material.h>
#include <TransformationMatrix.h>
#include <DirectionalLight.h>
#include "MaterialData.h"
#include "ModelData.h"


#include "Vector4.h"
#include "Matrix4x4Calculation.h"
#include <VertexData.h>

#include <VectorCalculation.h>
#include <d3dx12.h>



#include "Camera.h"
#include "Transform.h"
#include "Particle.h"
#include <AccelerationField.h>
#include "ModelManager.h"

struct Emitter {
	//エミッタのTransform;
	Transform transform;
	//発生数
	uint32_t count;
	//発生頻度
	float frequency;
	//頻度用時刻
	float frequencyTime;
};

class Particle3D {
public:

	//コンストラクタ
	Particle3D();

	//初期化
	static Particle3D* Create(uint32_t modelHandle);


private:

#pragma region パーティクルの設定で使う関数

	//パーティクルの初期化をする関数
	Particle MakeNewParticle(std::mt19937& randomEngine);

	//Emitterで発生させる
	std::list<Particle> Emission(const Emitter& emmitter, std::mt19937& randomEngine);

#pragma endregion

public:

	void Update(Camera& camera);

	//通常の描画
	//void Draw();

	//テクスチャを上書きをする描画
	void Draw(uint32_t textureHandle,Camera& camera);


	//デストラクタ
	~Particle3D();




public:
	//透明度の変更
	void SetColor(Vector4 color) {
		this->materialColor_ = color;
	}

	void SetTransparency(float transparency) {
		this->materialColor_.w = transparency;
	}
	
	bool IsBillBordMode(bool isBillBordMode) {
		this->isBillBordMode_ = isBillBordMode;
	}


#pragma region エミッタの中の設定
	

	#pragma region SRT
	//Scale
	void SetScale(Vector3 scale) {
		this->emitter_.transform.scale = scale;
	}
	
	//Rotate
	void SetRotate(Vector3 rotate) {
		this->emitter_.transform.rotate = rotate;
	}
	Vector3 GetRotate() {
		return emitter_.transform.rotate;
	}

	//Translate
	void SetTranslate(Vector3 translate) {
		this->emitter_.transform.translate = translate;
	}
	Vector3 GetTranslate() {
		return emitter_.transform.translate;
	}

	#pragma endregion

	//発生数
	void SetCount(uint32_t count) {
		this->emitter_.count = count;
	}
	//発生頻度
	void SetFrequency(float frequency){
		this->emitter_.frequency = frequency;
	}
	//発生頻度を設定
	void SetFrequencyTime(float frequencyTime){
		this->emitter_.frequencyTime = frequencyTime;
	}


	void SetField(bool isSetField) {
		this->isSetField_ = isSetField;
	}
	void SetAccelerationField(AccelerationField accelerationField) {
		this->accelerationField_ = accelerationField;
	}

#pragma endregion

#pragma region Lightingの設定
	void SetLighting(bool enableLighting) {
		this->isEnableLighting_ = enableLighting;
	}
	//方向
	void SetDirection(Vector3 direction) {
		this->lightingDirection_ = direction;
	}

#pragma endregion

private:
	
	std::list<ModelData> modelInformationList_;

	//頂点リソースを作る
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	std::vector<VertexData> vertices_{};

	//表示する数
	int32_t instanceCount_ = 1;


	//マテリアル用のリソースを作る
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	//色関係のメンバ変数
	Vector4 materialColor_ = { 1.0f,1.0f,1.0f,1.0f };


	//Lighting用
	ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;
	//色
	Vector4 directionalLightColor_ = { 1.0f,1.0f,1.0f,1.0f };
	float directionalLightIntensity_ = 3.0f;

	//基本はtrueで
	bool isEnableLighting_ = true;
	//方向
	Vector3 lightingDirection_ = {0.0f,-1.0f,0.0f};


	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_ = {};
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_ = {};

	ComPtr<ID3D12Resource>instancingResource_ = nullptr;


	static const int32_t MAX_INSTANCE_NUMBER_ = 100;
	//描画すべきインスタンス数
	uint32_t numInstance_ = 0;

	int InstancingIndex_;

	//パーティクル
	std::list<Particle>particles_;
	ParticleForGPU* instancingData_ = nullptr;

	//ビルボード
	bool isBillBordMode_ = true;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;


	//エミッタの設定
	Emitter emitter_ = {};
	const float DELTA_TIME = 1.0f / 60.0f;

	//フィールド
	bool isSetField_ = false;
	AccelerationField accelerationField_ = {};

};
