#include "Missile.h"
#include "GameObject/Enemy/Enemy.h"
#include "ModelManager.h"
#include "../../Player.h"
#include "VectorCalculation.h"

void Missile::Initialize(const Vector3& position, const Vector3& velocity)
{
	// モデル設定
	modelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/AssignmentModel/AnimatedCube", "AnimatedCube.gltf");
	model_.reset(Model::Create(modelHandle_));

	//速度の初期化
	velocity_ = velocity;

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translate_ = position;
	worldTransform_.scale_ = { 10.6f,10.6f,10.6f };

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
	Vector3 targetPosition = player_.GetWorld().translate_;
	Vector3 sub = targetPosition - worldTransform_.translate_;

	if (!isRepelled_)
	{
		//距離を計算
		//float distance = Length(sub);

		//正規化
		sub = Normalize(sub);
		
		velocity_ = Normalize(velocity_);
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
		velocity_ = Slerp(velocity_, sub, trackingParameter_);
		const float kSpeed = 0.6f;
		velocity_ *= kSpeed;
	}

	//追捕終了していなかったら
	if (!isTrackingComplete_ && !isRepelled_)
	{
		velocity_ = Slerp(velocity_, sub, trackingParameter_);
		const float kSpeed = 0.6f;
		velocity_ *= kSpeed;
	}

	//移動処理
	worldTransform_.translate_ += velocity_;

	//回転処理
	worldTransform_.rotate_.x += 0.2f;
	worldTransform_.rotate_.y += 0.2f;

	//ワールドトランスフォームの更新
	worldTransform_.Update();

	////フィールド外に出たら死亡フラグを立てる
	//if (worldTransform_.translate_.x <= -1000.0f || worldTransform_.translate_.x >= 1000.0f || worldTransform_.translate_.y <= 1.0f || worldTransform_.translate_.z <= -1000.0f || worldTransform_.translate_.z >= 1000.0f)
	//{
	//	isDead_ = true;
	//}
}

void Missile::Draw( Camera camera)
{
	model_->Draw(worldTransform_, camera);
}

//void Missile::OnCollision(Collider* collider)
//{
//	if (collider->GetCollisionAttribute() == kCollisionAttributeWeapon && !isDead_)
//	{
//		SetCollisionAttribute(kCollisionAttributeWeapon);
//		SetCollisionMask(kCollisionMaskWeapon);
//		isRepelled_ = true;
//		const float kSpeed = 0.6f;
//		Vector3 targetPosition = GameObjectManager::GetInstance()->GetGameObject<NamedEnemy>("NamedEnemy")->GetWorldPosition();
//		Vector3 sub = targetPosition - worldTransform_.translation_;
//		sub = Mathseries::Normalize(sub);
//		velocity_ = sub * kSpeed;
//	}
//
//	if (isRepelled_)
//	{
//		if (collider->GetCollisionAttribute() & kCollisionAttributeEnemy)
//		{
//			isDead_ = true;
//		}
//	}
//	else
//	{
//		if (collider->GetCollisionAttribute() & kCollisionAttributePlayer)
//		{
//			isDead_ = true;
//		}
//	}
//}
//
//const Vector3 Missile::GetWorldPosition() const
//{
//	Vector3 pos{};
//	pos.x = worldTransform_.matWorld_.m[3][0];
//	pos.y = worldTransform_.matWorld_.m[3][1];
//	pos.z = worldTransform_.matWorld_.m[3][2];
//	return pos;
//}
