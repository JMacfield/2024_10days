#pragma once
#include "Model.h"
#include "AnimationModel.h"
#include "TextureManager.h"
#include "Camera.h"
#include "GameObject/Player.h"
#include "VectorCalculation.h"

#include "Collision.h"
#include "OBB.h"

class Missile
{
public:
	//追尾時間
	static const uint32_t kTrackingTime = 180 * 2;

	void Initialize(const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(Camera camera);

	//	void OnCollision(Collider* collider) override;

	//	const Vector3 GetWorldPosition() const override;

	//const WorldTransform& GetWorldTransform() const override { return worldTransform_; };

	const bool GetIsDead() const { return isDead_; };

	OBB GetCollision() { return collision_; }

	void SetCollision();

private:

	Player* player_ = nullptr;

	// モデル
	std::unique_ptr<Model> model_ = nullptr;
	// テクスチャ
	uint32_t modelHandle_ = 0;

	//ワールドトランスフォーム
	WorldTransform worldTransform_{0.0f,0.0f,0.0f};

	//速度
	Vector3 velocity_{};

	//媒介変数
	float trackingParameter_ = 0.0f;

	//追尾
	bool isTrackingComplete_ = false;

	//追尾タイマー
	uint32_t trackingTimer_ = 0;

	//死亡フラグ
	bool isDead_ = false;

	//オーディオハンドル
	uint32_t audioHandle_ = 0;

	//プレイヤーにはじかれたかどうか
	bool isRepelled_ = false;

	// ミサイル当たり判定用
	OBB collision_;
};


