#pragma once
#include "Model.h"
#include "AnimationModel.h"
#include "TextureManager.h"
#include "Camera.h"
#include "GameObject/Player.h"
#include "VectorCalculation.h"

#include "Collision.h"
#include "OBB.h"

class Beam
{
public:
	//レーザーの寿命
	static const uint32_t kLifeTime = 60 * 10;

	void Initialize();

	void Update();

	void Draw(const Camera& camera);

	/*void OnCollision(Collider* collider) override;

	const Vector3 GetWorldPosition() const override;

	const WorldTransform& GetWorldTransform() const override { return worldTransform_; };*/

	const bool GetIsDead() const { return isDead_; };

private:
	//モデル
	std::unique_ptr<Model> model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_{};

	//死亡フラグ
	bool isDead_ = false;

	//タイマー
	uint32_t lifeTimer_ = 0;

	//OBB
	OBB obbSize{};

	//目標スケール
	Vector3 targetScale_{ 1.5f,1.5f,50.0f };
};

