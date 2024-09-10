#include "EnemyNormal.h"
#include "GameObject/Enemy/Randam/Randam.h"
#include "GameObject/Enemy/Enemy.h"
#include "GameObject/Player.h"

void NamedEnemyStateNormal::Initialize(Enemy* pEnemy)
{
	worldTransform_ = pEnemy->GetWorld();
	attackTime_ = Random::GetRandomInt(kMinAttackInterval, kMaxAttackInterval);
}

void NamedEnemyStateNormal::Update(Enemy* pEnemy)
{
	//攻撃処理
	if (++attackTimer_ > attackTime_)
	{
		uint32_t attackNum = 1;
		IEnemyState* newState = nullptr;
		switch (attackNum)
		{

		case 0:
			newState = new EnemyMissile();
			newState->Initialize(pEnemy);
			break;

		}
		pEnemy->ChangeState(newState);
	}

	//ノックバックの処理
	if (isKnockBack_)
	{
		if (++knockBackTimer_ >= knockBackTime_)
		{
			isKnockBack_ = false;
		}

		worldTransform_.translate_ += knockBackVelocity_;
	}

	//移動限界座標
	const float kMoveLimitX = 47;
	const float kMoveLimitZ = 47;
	worldTransform_.translate_.x = std::max<float>(worldTransform_.translate_.x, -kMoveLimitX);
	worldTransform_.translate_.x = std::min<float>(worldTransform_.translate_.x, +kMoveLimitX);
	worldTransform_.translate_.z = std::max<float>(worldTransform_.translate_.z, -kMoveLimitZ);
	worldTransform_.translate_.z = std::min<float>(worldTransform_.translate_.z, +kMoveLimitZ);
}



//void EnemyStateNormal::OnCollision(Collider* collider)
//{
//	if (collider->GetCollisionAttribute() == kCollisionAttributeWeapon)
//	{
//		//ノックバックの速度を決める
//		const Player* player = GameObjectManager::GetInstance()->GetGameObject<Player>("Player");
//		knockBackVelocity_ = player->GetVelocity();
//		//if (player->GetComboIndex() == 3)
//		//{
//		//	Vector3 sub = worldTransform_.translation_ - player->GetWorldPosition();
//		//	sub = Mathseries::Normalize(sub);
//		//	sub.y = 0.0f;
//		//	const float kKnockBackSpeed = 2.0f;
//		//	knockBackVelocity_ = sub * kKnockBackSpeed;
//		//}
//
//		////ノックバックのフラグを立てる
//		//isKnockBack_ = true;
//
//		////ノックバックのタイマーを設定
//		//knockBackTimer_ = 0;
//		//knockBackTime_ = player->GetAttackTotalTime() - player->GetAttackParameter();
//	}
//}