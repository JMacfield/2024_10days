#pragma once
#include "GameObject/Enemy/state/IEnemyState.h"
#include "VectorCalculation.h"

class EnemyMissile : public IEnemyState
{
public:
	//待機時間
	static const uint32_t kWaitTime = 60;
	//ミサイル発射間隔
	static const uint32_t kFireInterval = 40;
	//硬直時間
	static const uint32_t kRecoveryTime = 60 * 2;

	void Initialize(Enemy* named) override;

	void Update(Enemy* named) override;


	//void OnCollision(Collider* collider) override;

	const WorldTransform& GetWorldTransform() const override { return worldTransform_; };

private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_{};

	//クォータニオン
//	Quaternion destinationQuaternion_{ 0.0f,0.0f,0.0f,1.0f };

	//待機タイマー
	uint32_t waitTimer_ = 0;

	//ミサイル攻撃のフラグ
	bool isAttack_ = false;

	//ミサイルの発射タイマー
	uint32_t fireTimer_ = 0;

	//発射カウント
	uint32_t fireCount_ = 0;

	//硬直フラグ
	bool isRecovery_ = false;

	//硬直時間
	uint32_t recoveryTimer_ = 0;

	//オーディオハンドル
	uint32_t audioHandle_ = 0;
};

