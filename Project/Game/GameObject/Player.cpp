#include "Player.h"

Player::Player(){}

Player::~Player(){}

void Player::Init(){

	// モデル設定
	modelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/AssignmentModel/comet", "comet.gltf");
	model_.reset(Model::Create(modelHandle_));
	
	// ワールド座標 初期化
	worldTransform_.Initialize();
	worldTransform_.translate_.y = 400.0f;

	// 移動量 初期化
	vel_ = { 0.0f,-kGravityPower_,0.0f };

}

void Player::Update(){
	
	// 落下
	if (worldTransform_.translate_.y > 0.0f) {
		vel_.y += (vel_.y / 45.0f);
		worldTransform_.translate_.y += vel_.y;
	}
	else if (worldTransform_.translate_.y < 0.0f) {
		worldTransform_.translate_.y = 0.0f;
	}


	// ワールド座標 更新
	worldTransform_.Update();

}

void Player::Draw(Camera camera){

	//  描画
	model_->Draw(worldTransform_,camera);
}
