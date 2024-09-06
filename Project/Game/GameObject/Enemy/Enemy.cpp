#include "Enemy.h"

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Initialize() {

	// モデル設定
	modelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/AssignmentModel/AnimatedCube", "AnimatedCube.gltf");
	model_.reset(Model::Create(modelHandle_));

	// ワールド座標 初期化
	worldTransform_.Initialize();
	worldTransform_.translate_.y = 3.0f;

	worldTransform_.scale_ = {5.0f,5.0f,5.0f};

}

void Enemy::Update() {

	// ワールド座標 更新
	worldTransform_.Update();

}

void Enemy::Draw(Camera camera) {

	//  描画
	model_->Draw(worldTransform_, camera);
}
