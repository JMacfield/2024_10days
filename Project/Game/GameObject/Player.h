#pragma once

#include "Model.h"
#include "AnimationModel.h"
#include "TextureManager.h"
#include "Camera.h"

#include "OBB.h"
#include "Collision.h"

// 重力加速度
const float kGravityPower_ = 0.098f;
// 移動限界距離( -x ~ x, -z ~ z)
const Vector2 kLimitArea_ = { 4.0f,4.0f };
// フレームレート(仮)
const float kFlamerate = 60.0f;


class Player {

public: // --静的 メンバ関数 -- //

	// インタンス取得
	static Player* GetInstance();

public: // -- 公開 メンバ関数 -- //

	Player();
	~Player();
	void Init();
	void Update();
	void Draw(Camera camera);
	void Debug();
	void Final();

	void Move(Vector3 direction);
	
	WorldTransform GetWorld() { return worldTransform_; }
	OBB GetCollision() { return collision_; }

	void SetCollision();

	// 被弾時の減速処理
	void ResiveSpeedDoun(float power);

	// 隕石の落下スピード取得
	float GetSpeed() const { return vel_.y; }

	// 落下スピードの段階を取得
	float GetNormalT() { return normalT; }

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

	// 線形補間時間(0.0f ~ 1.0f)
	float normalT;
	// イージング時間(0.0f ~ 1.0f)
	float easeT;
	// 衝突時の減算量を保持(0.0f ~ 1.0f)
	float reverseT;

	// ゲーム時間(単位は秒)
	float inGameTime;

	// プレイヤークラスのインスタンス
	static Player* instance;

	// プレイヤークラスの当たり判定用
	OBB collision_;

};

