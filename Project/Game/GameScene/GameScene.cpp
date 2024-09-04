#include "GameScene/GameScene.h"
#include <imgui.h>
#include <Input.h>
#include "GlobalVariables.h"

#include "ModelManager.h"
#include "AnimationManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameScene::GameScene() {

}

/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize() {
	
	modelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/AssignmentModel/simpleSkin", "simpleSkin.gltf");
	planeModelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/AssignmentModel/plane", "plane.gltf");


	for (int i = 0; i < SIMPLE_SKIN_AMOUNT_; ++i) {
		skeleton_[i].Create(ModelManager::GetInstance()->GetModelData(modelHandle).rootNode);
		skinCluster_[i].Create(skeleton_[i], ModelManager::GetInstance()->GetModelData(modelHandle));
		animationHande_ = AnimationManager::GetInstance()->LoadFile("Resources/AssignmentModel/simpleSkin", "simpleSkin.gltf");

		simpleModel_[i].reset(AnimationModel::Create(modelHandle));
		worldTransform_[i].Initialize();
	}

	worldTransform_[0].translate_.x=0.0f;
	worldTransform_[0].translate_.y = 0.5f;
	worldTransform_[0].translate_.z = 0.0f;
	worldTransform_[1].translate_.y=-1.0f;
	worldTransform_[1].translate_.y = -1.0f;
	worldTransform_[1].translate_.z = -1.0f;

	//sneakWalk
	//Walk
	humanModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/AssignmentModel/human", "walk.gltf");
	humanAnimationModel_ = AnimationManager::GetInstance()->LoadFile("Resources/AssignmentModel/human", "walk.gltf");
	
	for (int i = 0; i < WALK_HUMAN_AMOUNT_; ++i) {
		human_[i].reset(AnimationModel::Create(humanModelHandle));
		humanWorldTransform_[i].Initialize();
		humanAnimationTime_[i] = 0;
		humanSkeleton_[i].Create(ModelManager::GetInstance()->GetModelData(humanModelHandle).rootNode);
		humanSkinCluster_[i].Create(humanSkeleton_[i], ModelManager::GetInstance()->GetModelData(humanModelHandle));
		humanWorldTransform_[i].translate_.x = 2.0f;

	}

	humanWorldTransform_[0].translate_.y = 0.5f;
	humanWorldTransform_[1].translate_.y = -2.0f;

	// -- 床 仮置き -- //
	planeModel_.reset(Model::Create(planeModelHandle_));
	
	planeModelWorldTransform_.Initialize();
	planeModelWorldTransform_.scale_ = { 32.0f,1.0f,32.0f };

	//球
	//uint32_t noneModelHandle = ModelManager::GetInstance()->LoadModelFile("Resources/CG3/Sphere", "Sphere.obj");
	//noneAnimationModel_.reset(Model::Create(noneModelHandle));
	noneAnimationWorldTransform_.Initialize();
	const float SPHERE_SCALE = 0.5f;
	noneAnimationWorldTransform_.scale_ = { SPHERE_SCALE,SPHERE_SCALE,SPHERE_SCALE };
	noneAnimationWorldTransform_.translate_.x = -2.0f;
	noneAnimationWorldTransform_.translate_.y = -1.0f;

	// カメラ
	camera_.Initialize();
	camera_.rotate_= {1.57f,0.0f,0.0f};
	camera_.translate_ = {0.0f,100.0f,0.0f};

	for (int i = 0; i < SIMPLE_SKIN_AMOUNT_; ++i) {
		worldTransform_[i].rotate_.y = 3.1415f;
	}

	// -- Player 初期化 -- //
	player_ = std::make_unique<Player>();
	player_->Init();

}

/// <summary>
/// 更新
/// </summary>
void GameScene::Update(GameManager* gameManager) {
	gameManager;

	XINPUT_STATE joyState;

	camera_.translate_.y = player_->GetWorld().translate_.y + 25.0f;
	camera_.Update();

	animationTime_[0] += 1.0f / 60.0f;
	animationTime_[1] += 2.0f / 60.0f;
	
	humanAnimationTime_[0] += 1.0f / 60.0f;
	humanAnimationTime_[1] += 3.0f / 60.0f;

	AnimationManager::GetInstance()->ApplyAnimation(humanSkeleton_[0], humanAnimationModel_, humanModelHandle, humanAnimationTime_[0]);
	
	if (!Input::GetInstance()->GetJoystickState(joyState)) {
		return;
	}

	Vector3 move = { (float)joyState.Gamepad.sThumbLX / SHRT_MAX,0.0f,0.0f };
	move.x = move.x * 0.5f;

	humanWorldTransform_->translate_ = Add(humanWorldTransform_[0].translate_, move);

	if (move.x > 0.1f) {
		humanWorldTransform_->rotate_.y = 1.4f;
	}
	else if (move.x < -0.1f) {
		humanWorldTransform_->rotate_.y = -1.4f;
	}

	if (humanWorldTransform_->translate_.x < -3.36f) {
		humanWorldTransform_->translate_.x = -3.3599999f;
	}
	else if (humanWorldTransform_->translate_.x > 3.36f) {
		humanWorldTransform_->translate_.x = 3.3599999f;
	}
	
	for (int i = 0; i < SIMPLE_SKIN_AMOUNT_; ++i) {
		skeleton_[i].Update();
	}
	for (int i = 0; i < WALK_HUMAN_AMOUNT_; ++i) {
		humanSkeleton_[i].Update();
	}
	
	for (int i = 0; i < SIMPLE_SKIN_AMOUNT_; ++i) {
		skinCluster_[i].Update(skeleton_[i]);
	}
	for (int i = 0; i < WALK_HUMAN_AMOUNT_; ++i) {
		humanSkinCluster_[i].Update(humanSkeleton_[i]);
	}

#ifdef _DEBUG
	
	if (Input::GetInstance()->IsPushLeft(joyState)) {
		player_->Init();
	}

#endif
	for (int i = 0; i < WALK_HUMAN_AMOUNT_; ++i) {
		humanWorldTransform_[i].Update();
	}

	// -- Player 更新 -- //
	player_->Update();

	// -- 床 -- //
	planeModelWorldTransform_.Update();
}

void GameScene::Draw() {
	human_[0]->Draw(humanWorldTransform_[0], camera_, humanSkinCluster_[0]);

	// -- 床 描画-- //
	planeModel_->Draw(planeModelWorldTransform_, camera_);

	// -- Player 描画 -- //
	player_->Draw(camera_);

}

GameScene::~GameScene() {
	
}