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

	// -- 雲 -- //
	clowdModelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/AssignmentModel/clowd", "clowd.gltf");
	clowdModel_.reset(Model::Create(clowdModelHandle_));
	clowdModel_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.5f));
	clowdModel_->SetLighting(false);
	clowdModelWorldTransform_.Initialize();
	clowdModelWorldTransform_.scale_ = { 4.0f,8.0f,4.0f };
	clowdModelWorldTransform_.translate_ = { 0.0f,200.0f,0.0f };
	
	// -- Player 初期化 -- //
	player_ = Player::GetInstance();
	player_->Init();

	//enemy初期化
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();

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
	// 3文字目を"."に7文字目をkmのテクスチャに変更
	speedUI_[2]->SetTexture(stringTexHandle_["dot"]);
	speedUI_[6]->SetTexture(stringTexHandle_["km"]);
	speedUI_[6]->SetPosition(Vector2(246.0f, 5.0f));

	// -- 速度メーター 初期化 -- //

	// メーター段階
	materStep_ = 0;

	// 追加ディレクトリパス
	std::string directrypathMidle = "antenna/antenna";
	
	// 初期化
	for (int32_t i = 0; i < materUI_.size(); i++) {
		
		// 画像読み込み
		std::string number = std::to_string(i);

		materTex_[i] = textureManager->LoadTexture(directrypath + directrypathMidle + number + format);
		
		// スプライト生成
		materUI_[i] = Sprite::Create(materTex_[i], Vector2(32.0f, 24.0f));
		materUI_[i]->SetScale(Vector2(0.7f, 0.7f));
	}

	materUI_[1]->SetPosition({ materUI_[0]->GetPosition().x + (materUI_[0]->GetScale().x * 512.0f) ,materUI_[0]->GetPosition().y });
	materUI_[1]->SetAnchorPoint(Vector2(1.0f, 0.0f));

	// -- ゲーム進行状況 -- //
	gameBehavior_ = GameBehavior::kStart;

	clowdModel_->SetLighting(false);
}

/// <summary>
/// 更新
/// </summary>
void GameScene::Update(GameManager* gameManager) {
	
	switch (gameBehavior_)
	{
	case GameBehavior::kStart:

		this->StartUpdate(gameManager);
		
		break;
	case GameBehavior::kInGame:
		
		this->InGameUpdate(gameManager);
		
		break;
	case GameBehavior::kPerfectClear:
		
		this->PerfectUpdate(gameManager);
		
		break;
	case GameBehavior::kGameClear:
		
		this->ClearUpdate(gameManager);
		
		break;
	case GameBehavior::kGameOver:
		
		this->OverUpdate(gameManager);
		
		break;
	default:
		break;
	}

#ifdef _DEBUG

	ImGui::Begin("Clowd");
	ImGui::DragFloat3("Pos", &clowdModelWorldTransform_.translate_.x, 1.0f);
	ImGui::DragFloat3("Rot", &clowdModelWorldTransform_.rotate_.x, 0.01f);
	ImGui::DragFloat3("Scale", &clowdModelWorldTransform_.scale_.x, 0.1f);
	ImGui::End();

#endif

	// -- 床 更新 -- //
	planeModelWorldTransform_.Update();

	// -- 雲 更新 -- //
	clowdModelWorldTransform_.Update();


}

void GameScene::Draw() {
	
	// -- 床 描画-- //
	planeModel_->Draw(planeModelWorldTransform_, camera_);

	// -- 雲 描画 -- //
	clowdModel_->Draw(clowdModelWorldTransform_,camera_);

	// -- テクスチャ 描画 -- // 
	//enemy描画
	enemy_->Draw(camera_);

	// -- テクスチャ 更新 -- // 
	for (int32_t i = 0; i < speedUI_.size(); i++) {
		speedUI_[i]->Draw();
	}

	if (gameBehavior_ == GameBehavior::kInGame) {

		// -- Player 描画 -- //
		player_->Draw(camera_);

		// -- 速度メーター 描画 -- //
		for (int32_t i = 0; i < materUI_.size(); i++) {
			materUI_[i]->Draw();
		}

		// -- テクスチャ 描画 -- // 
		for (int32_t i = 0; i < speedUI_.size(); i++) {
			speedUI_[i]->Draw();
		}

	}

}

void GameScene::StartUpdate(GameManager* gameManager)
{
	// トランジションが終了したらInGameへ移行する
	if(!gameManager->IsStartTransition()){
		gameBehavior_ = GameBehavior::kInGame;
	}
}

void GameScene::InGameUpdate(GameManager* gameManager)
{
	gameManager;

	// 入力
	XINPUT_STATE joyState;

	// -- Player 更新 -- //
	player_->Update();

	//enemy更新
	enemy_->Update();

	// カメラ 更新
	camera_.Update();
	camera_.translate_.y = player_->GetWorld().translate_.y + 30.0f;
	camera_.translate_.y += player_->GetSpeed();

	// コントローラーを接続していなければ早期リターン
	if (!Input::GetInstance()->GetJoystickState(joyState)) {
		//return;
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




	// -- UI 更新 -- //

	// 速度に応じて画像を変更
	float speed = player_->GetSpeed() * 1000.0f;
	int32_t viewSpeed = std::abs((int32_t)speed);

	speedUI_[0]->SetTexture(numberTexHandle_[(viewSpeed / 10000)]);
	viewSpeed = viewSpeed % 10000;
	speedUI_[1]->SetTexture(numberTexHandle_[(viewSpeed / 1000)]);
	viewSpeed = viewSpeed % 1000;
	speedUI_[3]->SetTexture(numberTexHandle_[(viewSpeed / 100)]);
	viewSpeed = viewSpeed % 100;
	speedUI_[4]->SetTexture(numberTexHandle_[(viewSpeed / 10)]);
	viewSpeed = viewSpeed % 10;
	speedUI_[5]->SetTexture(numberTexHandle_[(viewSpeed)]);

	// -- 速度メーター更新 -- //
	materUI_[1]->SetScale(Vector2(float(1.0f - player_->GetNormalT()) * materUI_[0]->GetScale().x, materUI_[0]->GetScale().y));

	// ミサイルとの判定 プレイヤーとエネミーのOBB
	//IsCollision(player_->GetCollision(),);
}

void GameScene::PerfectUpdate(GameManager* gameManager)
{
	gameManager;
}

void GameScene::ClearUpdate(GameManager* gameManager)
{
	gameManager;
}

void GameScene::OverUpdate(GameManager* gameManager)
{
	gameManager;
}

GameScene::~GameScene() {
	
	player_->Final();

	// Sprite 解放 
	for (int32_t i = 0; i < speedUI_.size(); i++) {
		delete speedUI_[i];
	}

	// メーター 解放 
	for (int32_t i = 0; i < materUI_.size(); i++) {
		delete materUI_[i];
	}

}