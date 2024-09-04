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

	void Move(Vector3 direction);
	
	WorldTransform GetWorld() { return worldTransform_; }

private: // -- 非公開 メンバ変数 -- //

	// モデル
	std::unique_ptr<Model> model_ = nullptr;
	// テクスチャ
	uint32_t modelHandle_ = 0;
	// ワールド座標
	WorldTransform worldTransform_ = {};

	// 移動量
	Vector3 vel_;
	// 重力加速度
	const float kGravityPower_ = 0.49f;

	// 移動限界距離( -x ~ x, -z ~ z)
	const Vector2 kLimitArea_ = { 4.0f,4.0f };

};

