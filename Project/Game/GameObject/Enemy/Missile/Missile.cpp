#include "Missile.h"
#include "GameObject/Enemy/Enemy.h"
#include "ModelManager.h"
#include "../../Player.h"
#include "VectorCalculation.h"
#include "GameObject/Enemy/Randam/Randam.h"

void Missile::Initialize(const Vector3& position, const Vector3& velocity)
{
	// モデル設定
	modelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/AssignmentModel/missile", "missile.gltf");
	model_.reset(Model::Create(modelHandle_));

	//速度の初期化
	velocity_ = velocity;

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translate_ = { missileSpornPoint_ = Random::GetRandomFloat(kMinSpornRange, kMaxSpornRange),position.y,missileSpornPoint_ = Random::GetRandomFloat(kMinSpornRange, kMaxSpornRange)};
	worldTransform_.rotate_.x = 135.1f;
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };

	player_ = Player::GetInstance();


	/*AABB aabb = {
		.min{-worldTransform_.scale_.x,-worldTransform_.scale_.y,-worldTransform_.scale_.z,},
		.max{worldTransform_.scale_.x,worldTransform_.scale_.y,worldTransform_.scale_.z}
	};
	SetAABB(aabb);
	SetCollisionAttribute(kCollisionAttributeMissile);
	SetCollisionMask(kCollisionMaskMissile);
	SetCollisionPrimitive(kCollisionPrimitiveAABB);*/

}

void Missile::Update()
{

	//目標への差分ベクトルを計算
	Vector3 targetPosition = player_->GetWorld().translate_;
	Vector3 sub = targetPosition - worldTransform_.translate_;

	if (!isRepelled_)
	{
		//距離を計算
		float distance = Length(sub);

		const float kSpeed = 5.6f;

		//正規化
		sub = Normalize(sub);
		
		velocity_ = Normalize(velocity_);
		velocity_ = sub * kSpeed;

		if (distance <= 500.0f)
		{
			isRepelled_ = true;
		}
	}

	//媒介変数の更新
	if (trackingParameter_ < 0.1f) {
		trackingParameter_ += 1.0f / (60.0f * 1.0f);
	}

	//追尾タイマーを進める
	if (trackingTimer_ != kTrackingTime)
	{
		trackingTimer_++;
	}

	//一定時間追尾するようにする
	if (trackingTimer_ >= kTrackingTime)
	{
		isTrackingComplete_ = true;
	}

	//追捕終了していなかったら
	if (isTrackingComplete_ == true)
	{
		velocity_ = Slerp(sub, velocity_, trackingParameter_);
		const float kSpeed = 1.0f;
		velocity_ *= kSpeed;
	}

	//移動処理
	worldTransform_.translate_ += velocity_;

	//回転処理


	//ワールドトランスフォームの更新
	worldTransform_.Update();

	SetCollision();

	if (isInvincible_ == false) {
		// 当たり判定
		if (IsCollision(player_->GetInstance()->GetCollision(), collision_)) {
			//player_->MinusHP(1);
			player_->ResiveSpeedDoun(0.02f);
			isDead_ = true;
			isInvincible_ = true;

			if (invincibleTime_ > 60) {
				isInvincible_ = false;
				invincibleTimer_ = 0;
				player_->ResiveSpeedDoun(0.0f);
			}
		}
	}

	if (isInvincible_ == true) {
		invincibleTimer_++;
	}

	if (player_->GetHP() == 0) {
		player_->SetAlive(false);
	}

	//フィールド外に出たら死亡フラグを立てる
	if  (worldTransform_.translate_.y >= player_->GetWorld().translate_.y + 50.0f)
	{
		isDead_ = true;
		player_->MinusHP(1);
	}
}

void Missile::Draw(Camera camera)
{
	model_->Draw(worldTransform_, camera);
}

void Missile::SetCollision() {
	collision_.center = worldTransform_.GetWorldPosition();
	GetOrientations(MakeRotateXYZMatrix(worldTransform_.rotate_.x, worldTransform_.rotate_.y, worldTransform_.rotate_.z), collision_.orientation);
	collision_.size = { 2.0f,2.0f,2.0f };
}