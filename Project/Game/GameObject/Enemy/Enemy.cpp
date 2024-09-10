#include "Enemy.h"

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Initialize() {

	// モデル設定
	modelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/AssignmentModel/AnimatedCube", "AnimatedCube.gltf");
	model_.reset(Model::Create(modelHandle_));

	// ワールド座標 初期化
	worldTransform_.Initialize();
	worldTransform_.translate_.y = 300.0f;

	worldTransform_.scale_ = {1.0f,1.0f,1.0f};

}

void Enemy::Update() {

	// ワールド座標 更新
	worldTransform_.Update();

	//死亡フラグの立ったミサイルを削除
	missiles_.remove_if([](std::unique_ptr<Missile>& missile)
		{
			if (missile->GetIsDead())
			{
				missile.reset();
				return true;
			}
			return false;
		}
	);

	//ミサイルの更新
	for (const std::unique_ptr<Missile>& missile : missiles_)
	{
		missile->Update();
	}

}

void Enemy::Draw(Camera camera) {

	//  描画
	model_->Draw(worldTransform_, camera);

	//ミサイルの描画
	for (const std::unique_ptr<Missile>& missile : missiles_)
	{
		missile->Draw(camera);
	}
}


void Enemy::ChangeState(IEnemyState* newState)
{
	delete state_;
	state_ = newState;
}