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

	// -- 床 仮置き -- //
	planeModelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/AssignmentModel/plane", "plane.gltf");
	planeModel_.reset(Model::Create(planeModelHandle_));
	planeModelWorldTransform_.Initialize();
	planeModelWorldTransform_.scale_ = { 32.0f,1.0f,32.0f };

	// -- Player 初期化 -- //
	player_ = std::make_unique<Player>();
	player_->Init();

	// カメラ
	camera_.Initialize();
	camera_.rotate_= {1.57f,0.0f,0.0f};
	camera_.translate_ = {0.0f,100.0f,0.0f};

}

/// <summary>
/// 更新
/// </summary>
void GameScene::Update(GameManager* gameManager) {
	gameManager;

	// 入力
	XINPUT_STATE joyState;

	// カメラ 更新
	camera_.translate_.y = player_->GetWorld().translate_.y + 25.0f;
	camera_.Update();
	
	// コントローラーを接続していなければ早期リターン
	if (!Input::GetInstance()->GetJoystickState(joyState)) {
		return;
	}

	// 移動量取得
	Vector3 move = { (float)joyState.Gamepad.sThumbLX / SHRT_MAX,0.0f,(float)joyState.Gamepad.sThumbLY / SHRT_MAX };
	if (move.x >= -0.3f && move.x <= 0.3f) {
		move.x = 0.0f;
	}
	else {
		move.x *= 0.5f;
	}
	move.y = 0.0f;
	if (move.z >= -0.3f && move.z <= 0.3f) {
		move.z = 0.0f;
	}
	else {
		move.z *= 0.5f;
	}
	player_->Move(move);

#ifdef _DEBUG
	
	// 再起動処理 //LBで反応する
	if (Input::GetInstance()->IsPushLeft(joyState)) {
		player_->Init();
	}

#endif
	
	// -- Player 更新 -- //
	player_->Update();

	// -- 床 -- //
	planeModelWorldTransform_.Update();



}

void GameScene::Draw() {
	
	// -- 床 描画-- //
	planeModel_->Draw(planeModelWorldTransform_, camera_);

	// -- Player 描画 -- //
	player_->Draw(camera_);

}

GameScene::~GameScene() {
	
}