#include "GameObject/Enemy/state/EnemyMissile.h"
#include "GameObject/Enemy/Enemy.h"

void EnemyMissile::Initialize(Enemy* pEnemy)
{
	worldTransform_ = pEnemy->GetWorld();
	//destinationQuaternion_ = worldTransform_.quaternion_;
}

void EnemyMissile::Update(Enemy* pEnemy)
{
	//worldTransform_.translate_.x += 5.0f;

	//待機処理
	if (!isAttack_)
	{
		if (++waitTimer_ > kWaitTime)
		{
			isAttack_ = true;
		}
	}

	//攻撃処理
	if (isAttack_ && !isRecovery_)
	{
		if (++fireTimer_ > kFireInterval)
		{
			fireTimer_ = 0;
			fireCount_++;
			Vector3 velocity[4];
			velocity[0] = { 1.2f,0.0f,1.2f };
			velocity[0] = TransformCalculation(velocity[0], worldTransform_.worldMatrix_);
			velocity[1] = { 1.2f,0.0f,-1.2f };
			velocity[1] = TransformCalculation(velocity[1], worldTransform_.worldMatrix_);

			Vector3 translation = pEnemy->GetWorld().translate_;
			Missile* newMissile1 = new Missile();
			newMissile1->Initialize(translation, velocity[0]);
			pEnemy->AddMissile(newMissile1);

			Missile* newMissile2 = new Missile();
			newMissile2->Initialize(translation, velocity[1]);
			pEnemy->AddMissile(newMissile2);

		}

		if (fireCount_ >= 3)
		{
			isRecovery_ = true;
		}
	}

	//硬直処理
	if (isRecovery_)
	{
		if (++recoveryTimer_ > kRecoveryTime)
		{
			IEnemyState* newState = new EnemyNormal();
			newState->Initialize(pEnemy);
			pEnemy->ChangeState(newState);
		}
	}

	//移動限界座標
	const float kMoveLimitX = 47;
	const float kMoveLimitZ = 47;
	worldTransform_.translate_.x = std::max<float>(worldTransform_.translate_.x, -kMoveLimitX);
	worldTransform_.translate_.x = std::min<float>(worldTransform_.translate_.x, +kMoveLimitX);
	worldTransform_.translate_.z = std::max<float>(worldTransform_.translate_.z, -kMoveLimitZ);
	worldTransform_.translate_.z = std::min<float>(worldTransform_.translate_.z, +kMoveLimitZ);
}



//void EnemyMissile::OnCollision(Collider* collider)
//{
//	if (collider->GetCollisionAttribute() == kCollisionAttributeWeapon)
//	{
//
//	}
//}