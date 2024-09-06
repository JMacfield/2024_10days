#pragma once

#include "Model.h"
#include "AnimationModel.h"
#include "TextureManager.h"
#include "Camera.h"

class Player {
public: // -- 公開 メンバ関数 -- //

	Player();
	~Player();
	void Init();
	void Update();
	void Draw(Camera camera);
	void Debug();

	void Move(Vector3 direction);
	
	WorldTransform GetWorld() { return worldTransform_; }

	// 隕石の落下スピード取得
	float GetSpeed() const { return vel_.y; }

	// 線形補間を利用して速度を取得
	float GetSpeedForEaseInOutQuad(float t);

private: // -- 非公開 メンバ変数 -- //

	// モデル
	std::unique_ptr<Model> model_ = nullptr;
	// テクスチャ
	uint32_t modelHandle_ = 0;
	// ワールド座標
	WorldTransform worldTransform_ = {};

	// 移動量
	Vector3 vel_;
	// 加速度
	float acc;

	// 重力加速度
	const float kGravityPower_ = 0.49f;

	// 移動限界距離( -x ~ x, -z ~ z)
	const Vector2 kLimitArea_ = { 4.0f,4.0f };

	float t;
	float easeT;

};

