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

	// -- 目印 -- //
	landmarkModelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/AssignmentModel/cube", "cube.gltf");
	for (int32_t i = 0; i < 60; i++) {
		
		// 新規モデルを追加
		Model* newModel = Model::Create(landmarkModelHandle_);
		// 配列にモデルを追加
		landmarkModel_.push_back(newModel);
		
		// 座標を初期化
		WorldTransform newWorld;
		newWorld.Initialize();
		newWorld.translate_.y = i * 1000.0f;
		newWorld.translate_.z = 5.0f;
		
		// 新規座標を追加
		landmarkWorldTransform_.push_back(newWorld);
	}

	// -- Player 初期化 -- //
	player_ = std::make_unique<Player>();
	player_->Init();

	// -- カメラ 初期化 -- //
	camera_.Initialize();
	camera_.rotate_= {1.57f,0.0f,0.0f};
	camera_.translate_ = {0.0f,player_->GetWorld().translate_.y,0.0f};

	// -- UI 初期化＆ロード -- //

	TextureManager* textureManager = TextureManager::GetInstance();// マネージャー 取得
	std::string directrypath = "Resources/AssignmentTexture/UI/";// パスを指定
	std::string format = ".png";// 拡張子を指定

	// 数字テクスチャを読みこむ
	for (int32_t i = 0; i < 10; i++) {
		std::string number = std::to_string(i);
		numberTexHandle_[i] = textureManager->LoadTexture(directrypath + number + format);
	}

	// 文字テクスチャ
	stringTexHandle_["km"] = textureManager->LoadTexture(directrypath + "km" + format);
	stringTexHandle_["dot"] = textureManager->LoadTexture(directrypath + "dot" + format);
	
	// 速度のテクスチャを設定
	for (int32_t i = 0; i < speedUI_.size(); i++) {
		speedUI_[i] = Sprite::Create(numberTexHandle_[0], Vector2(i * 32.0f, 5.0f));
		speedUI_[i]->SetScale(Vector2(1.0f /8.0f, 1.0f / 8.0f));
	}
	// 4文字目を"."に7文字目をkmのテクスチャに変更
	speedUI_[3]->SetTexture(stringTexHandle_["dot"]);
	speedUI_[6]->SetTexture(stringTexHandle_["km"]);
	speedUI_[6]->SetPosition(Vector2(246.0f, 5.0f));

	

}

/// <summary>
/// 更新
/// </summary>
void GameScene::Update(GameManager* gameManager) {
	gameManager;

	// 入力
	XINPUT_STATE joyState;

	// -- Player 更新 -- //
	player_->Update();

	// カメラ 更新
	camera_.Update();
	camera_.translate_.y = player_->GetWorld().translate_.y + 25.0f;
	camera_.translate_.y += player_->GetSpeed();

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
	
	// -- 床 更新 -- //
	planeModelWorldTransform_.Update();

	// -- 目印 更新 -- //
	for (WorldTransform world : landmarkWorldTransform_) {
		world.Update();
	}

	// -- UI 更新 -- //

	// 速度に応じて画像を変更
	float speed = player_->GetSpeed() * 100.0f;
	int32_t viewSpeed = std::abs((int32_t)speed);
	
	speedUI_[0]->SetTexture(numberTexHandle_[(viewSpeed / 10000)]);
	viewSpeed = viewSpeed % 10000;
	speedUI_[1]->SetTexture(numberTexHandle_[(viewSpeed / 1000)]);
	viewSpeed = viewSpeed % 1000;
	speedUI_[2]->SetTexture(numberTexHandle_[(viewSpeed / 100)]);
	viewSpeed = viewSpeed % 100;
	speedUI_[4]->SetTexture(numberTexHandle_[(viewSpeed / 10)]);
	viewSpeed = viewSpeed % 10;
	speedUI_[5]->SetTexture(numberTexHandle_[(viewSpeed)]);


}

void GameScene::Draw() {
	
	// -- 床 描画-- //
	planeModel_->Draw(planeModelWorldTransform_, camera_);

	// -- 目印 描画 -- //
	for (int32_t i = 0; i < landmarkModel_.size(); i++) {
		Model* model = landmarkModel_.at(i);
		model->Draw(landmarkWorldTransform_.at(i),camera_);
	}

	// -- Player 描画 -- //
	player_->Draw(camera_);

	// -- テクスチャ 更新 -- // 
	for (int32_t i = 0; i < speedUI_.size(); i++) {
		speedUI_[i]->Draw();
	}

}

GameScene::~GameScene() {
	

	// 目印 解放
	landmarkModel_.clear();

	// Sprite 解放 
	for (int32_t i = 0; i < speedUI_.size(); i++) {
		delete speedUI_[i];
	}

}