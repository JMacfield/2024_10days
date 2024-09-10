#include "EnemyNormal.h"
#include "GameObject/Enemy/Randam/Randam.h"
#include "GameObject/Enemy/Enemy.h"
#include "GameObject/Player.h"

void EnemyNormal::Initialize(Enemy* pEnemy)
{
	worldTransform_ = pEnemy->GetWorld();
	attackTime_ = Random::GetRandomInt(kMinAttackInterval, kMaxAttackInterval);
}

void EnemyNormal::Update(Enemy* pEnemy)
{

	//worldTransform_.translate_.x += 5.0f;
	//攻撃処理
	if (++attackTimer_ > attackTime_ && !pEnemy->GetIsSlow())
	{
		uint32_t attackNum = 0;
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


	//移動限界座標
	const float kMoveLimitX = 47;
	const float kMoveLimitZ = 47;
	worldTransform_.translate_.x = std::max<float>(worldTransform_.translate_.x, -kMoveLimitX);
	worldTransform_.translate_.x = std::min<float>(worldTransform_.translate_.x, +kMoveLimitX);
	worldTransform_.translate_.z = std::max<float>(worldTransform_.translate_.z, -kMoveLimitZ);
	worldTransform_.translate_.z = std::min<float>(worldTransform_.translate_.z, +kMoveLimitZ);
}