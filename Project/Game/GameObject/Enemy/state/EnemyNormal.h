#pragma once
#include "IEnemyState.h"
#include "VectorCalculation.h"
#include "Quaternion.h"

class EnemyNormal : public IEnemyState
{
public:
	//攻撃までの最小時間
	static const uint32_t kMinAttackInterval = 60 * 2;
	//攻撃までの最大時間
	static const uint32_t kMaxAttackInterval = 60 * 4;

	void Initialize(Enemy* pEnemy) override;

	void Update(Enemy* pEnemy) override;

	const WorldTransform& GetWorldTransform() const override { return worldTransform_; };

private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_{};

	//クォータニオン
	Quaternion destinationQuaternion_{ 0.0f,0.0f,0.0f,1.0f };

	//攻撃用のタイマー
	uint32_t attackTimer_ = 0;
	uint32_t attackTime_ = 0;
};

