#pragma once

#include "Model.h"
#include "AnimationModel.h"
#include "TextureManager.h"
#include "Camera.h"
#include "GameObject/Player.h"
#include "GameObject/Enemy/Missile/Missile.h"
#include "GameObject/Enemy/state/IEnemyState.h"
#include "GameObject/Enemy/state/EnemyNormal.h"
#include "GameObject/Enemy/state/EnemyMissile.h"

class Enemy {
public: 

	Enemy();
	~Enemy();
	void Initialize();
	void Update();
	void Draw(Camera camera);

	void ChangeState(IEnemyState* newState);

	const std::list<std::unique_ptr<Missile>>& GetMissiles() const { return missiles_; };

	void AddMissile(Missile* missile) { missiles_.push_back(std::unique_ptr<Missile>(missile)); };

	const bool GetIsAttack() const { return isAttack_; };

	void SetIsAttack(const bool isAttack) { isAttack_ = isAttack; };

	const float GetDamage() const { return damage_; };

	void SetDamage(const float damage) { damage_ = damage; };

	const float GetHP() const { return hp_; };

	//Model* GetModel() { return model_; };

	void SetIsSlow(bool isSlow) { isSlow_ = isSlow; };

	const bool GetIsSlow() { return isSlow_; };

	WorldTransform GetWorld() { return worldTransform_; }

private:
	//状態
	IEnemyState* state_ = nullptr;

	//ミサイル
	std::list<std::unique_ptr<Missile>> missiles_{};

	//HP
	Vector2 hpBarSize_{ 480.0f,16.0f };
	const float kMaxHP = 800.0f;
	float hp_ = kMaxHP;

	//アクティブフラグ
	bool isActive_ = false;

	//攻撃フラグ
	bool isAttack_ = false;

	//ダメージ
	float damage_ = 0.0f;

	//当たり判定のフラグ
	bool onCollision_ = false;
	bool preOnCollision_ = false;

	//スロウ状態のフラグ
	static const int kSlowTime = 60 * 5;
	bool isSlow_ = false;
	int slowTimer_ = 0;
	// -- Player -- //
	std::unique_ptr<Player> player_;
	// モデル
	std::unique_ptr<Model> model_ = nullptr;
	// テクスチャ
	uint32_t modelHandle_ = 0;
	// ワールド座標
	WorldTransform worldTransform_ = {};

};

