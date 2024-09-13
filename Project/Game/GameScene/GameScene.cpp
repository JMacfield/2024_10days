#include "GameScene/GameScene.h"
#include <imgui.h>
#include <Input.h>
#include "GlobalVariables.h"

#include "ModelManager.h"
#include "AnimationManager.h"
#include "Other/Code/OtherCode.h"

#include "OverScene.h"
#include "ClearScene.h"
#include "PerfectScene.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameScene::GameScene() {

}

/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize() {

	// -- 天球  -- //
	skydomeModelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/AssignmentModel/skydome", "skydome.gltf");
	skydomeModel_.reset(Model::Create(skydomeModelHandle_));
	skydomeModel_->SetLighting(false);
	skydomeModelWorldTransform_.Initialize();
	skydomeModelWorldTransform_.scale_ = { 8192.0f,8192,8192 };


	// -- 床 仮置き -- //
	planeModelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/AssignmentModel/earth", "earth.gltf");
	planeModel_.reset(Model::Create(planeModelHandle_));
	planeModel_->SetLighting(false);
	planeModelWorldTransform_.Initialize();
	planeModelWorldTransform_.scale_ = { 512.0f,512.0f,512.0f };
	planeModelWorldTransform_.translate_ = { 0.0f,-256.0f,0.0f };


	// -- 雲 -- //
	clowdModelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/AssignmentModel/clowd", "clowd.gltf");
	clowdModel_.reset(Model::Create(clowdModelHandle_));
	clowdModel_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.5f));
	clowdModel_->SetLighting(false);
	clowdModelWorldTransform_.Initialize();
	clowdModelWorldTransform_.scale_ = { 4.0f,8.0f,4.0f };
	clowdModelWorldTransform_.translate_ = { 0.0f,200.0f,0.0f };

	// -- 隕石 -- //
	cometModelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/AssignmentModel/comet", "comet.gltf");
	cometModel_.reset(Model::Create(cometModelHandle_));
	cometWorldTransform_.Initialize();
	cometWorldTransform_.scale_ = { 512.0f,512.0f, 512.0f };
	cometWorldTransform_.translate_ = { 0.0f,60000.0f, 0.0f };

	// -- Player 初期化 -- //
	player_ = Player::GetInstance();
	player_->Init();

	//enemy初期化
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();

	Random::Initialize();

	// -- カメラ 初期化 -- //
	camera_.Initialize();
	camera_.rotate_ = { 1.57f,0.0f,0.0f };
	camera_.translate_ = { 0.0f,player_->GetWorld().translate_.y,0.0f };
	camera_.fov_ = 1.0f;
	cameraNormalT_ = 0.0f;

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
		speedUI_[i]->SetScale(Vector2(1.0f / 8.0f, 1.0f / 8.0f));
	}
	// 3文字目を"."に7文字目をkmのテクスチャに変更
	speedUI_[2]->SetTexture(stringTexHandle_["dot"]);
	speedUI_[6]->SetTexture(stringTexHandle_["km"]);
	speedUI_[6]->SetPosition(Vector2(246.0f, 5.0f));

	// -- 速度メーター 初期化 -- //

	hpUI_[0] = Sprite::Create(numberTexHandle_[5], Vector2(1130.0, 5.0f));
	hpUI_[1] = Sprite::Create(numberTexHandle_[1], Vector2(1100.0, 5.0f));
	hpUI_[0]->SetScale(Vector2(1.0f / 8.0f, 1.0f / 8.0f));
	hpUI_[1]->SetScale(Vector2(1.0f / 8.0f, 1.0f / 8.0f));

	// メーター段階
	materStep_ = 0;

	// 追加ディレクトリパス
	std::string directrypathMidle = "antenna/antenna";
	
	int32_t hpIconHandle = TextureManager::LoadTexture("Resources/AssignmentTexture/UI/mater/mater1.png");

	hpIcon_.reset(Sprite::Create(hpIconHandle, Vector2(1100, 50.0f)));
	hpIcon_->SetScale(Vector2(1.0f / 6.0f, 1.0f / 6.0f));

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

	// -- シェイク強度 -- //
	basePos = {};
	shakePos = {};
	shakePower = 32;
	shakeCount = 0;

	// -- ホワイトアウト -- //
	whiteOutTexHandle_ = textureManager->LoadTexture("Resources/AssignmentTexture/white2x2.png");
	whiteOutSprite.reset(Sprite::Create(whiteOutTexHandle_, Vector2(0.0f, 0.0f)));
	whiteOutSprite->SetScale(Vector2(1280.0f, 720.0f));
	whiteOutSprite->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	whiteOutNormalT_ = 0.0f;

	// -- 上下黒帯 -- //
	movieScreenTexHandle_ = textureManager->LoadTexture("Resources/AssignmentTexture/movieScreen.png");
	movieScreenSprite.reset(Sprite::Create(movieScreenTexHandle_, Vector2(640.0f, 360.0f)));
	movieScreenSprite->SetScale(Vector2(1.0f, 1.4f));
	movieScreenSprite->SetAnchorPoint(Vector2(0.5f, 0.5f));
	movieScreenNormalT_ = 0.0f;

	// インゲームBGM
	gameBGMHandle_ = Audio::GetInstance()->LoadMP3(L"Resources/Sounds/game.mp3");
	Audio::GetInstance()->ChangeVolume(gameBGMHandle_, 0.05f);
	isPerfect_ = false;

}

/// <summary>
/// 更新
/// </summary>
void GameScene::Update(GameManager* gameManager) {


	camera_.Update();

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

	/*ImGui::Begin("Clowd");
	ImGui::DragFloat3("Pos", &clowdModelWorldTransform_.translate_.x, 1.0f);
	ImGui::DragFloat3("Rot", &clowdModelWorldTransform_.rotate_.x, 0.01f);
	ImGui::DragFloat3("Scale", &clowdModelWorldTransform_.scale_.x, 0.1f);
	ImGui::End();

	ImGui::Begin("Camera");
	ImGui::DragFloat("Fov", &camera_.fov_, 0.01f);
	ImGui::End();*/

#endif

	// -- 床 更新 -- //
	planeModelWorldTransform_.Update();

	// -- 雲 更新 -- //
	clowdModelWorldTransform_.Update();

	// -- 隕石 更新 -- //
	cometWorldTransform_.Update();

	// -- 天球 更新 -- //
	skydomeModelWorldTransform_.Update();

	if (whiteOutNormalT_ >= 0.9f) {
		if (isPerfect_) {
			isAudioPlay_ = false;
			Audio::GetInstance()->StopMP3(gameBGMHandle_);
			gameManager->ChangeScene(new PerfectScene);
		}
		else {
			isAudioPlay_ = false;
			Audio::GetInstance()->StopMP3(gameBGMHandle_);
			gameManager->ChangeScene(new ClearScene);
			
		}
	}

	if (gameBehavior_ == GameBehavior::kGameOver&& gameManager->IsTransitioned()) {
		isAudioPlay_ = false;
		Audio::GetInstance()->StopMP3(gameBGMHandle_);
		gameManager->ChangeScene(new OverScene);
	}

}

void GameScene::Draw() {

	// -- 天球 描画 -- //
	skydomeModel_->Draw(skydomeModelWorldTransform_, camera_);

	// -- 床 描画-- //
	planeModel_->Draw(planeModelWorldTransform_, camera_);

	// -- 隕石 描画 -- //
	cometModel_->Draw(cometWorldTransform_, camera_);

	if (gameBehavior_ == GameBehavior::kInGame) {

		// -- 雲 描画 -- //
		//clowdModel_->Draw(clowdModelWorldTransform_, camera_);

		// -- テクスチャ 描画 -- // 
		//enemy描画
		enemy_->Draw(camera_);

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

		hpUI_[0]->Draw();
		hpUI_[1]->Draw();

		hpIcon_->Draw();
	}

	whiteOutSprite->Draw();

	movieScreenSprite->Draw();

}

void GameScene::StartUpdate(GameManager* gameManager)
{
	// トランジションが終了したらInGameへ移行する
	if (!gameManager->IsStartTransition()) {
		gameBehavior_ = GameBehavior::kInGame;
	}
}

void GameScene::InGameUpdate(GameManager* gameManager)
{
	gameManager;

	if (isAudioPlay_ == true) {
		Audio::GetInstance()->PlayMP3(gameBGMHandle_, true);
	}

	if (isAudioPlay_ == false) {
		Audio::GetInstance()->StopMP3(gameBGMHandle_);
	}

	// 入力
	XINPUT_STATE joyState;

	// -- Player 更新 -- //
	player_->Update();

	//enemy更新
	enemy_->Update();

	// -- カメラ 更新 -- //
	camera_.translate_.y = player_->GetWorld().translate_.y + 30.0f;
	camera_.translate_.y += player_->GetSpeed();
	// 視野角 調整
	if (player_->GetNormalT() > 0.0f) {
		camera_.fov_ = OtherCode::ExponentialInterpolation(1.0f, 0.3f, (player_->GetNormalT() - 0.0f) * 1.0f, 1.0f);
	}

	// 隕石座標
	cometWorldTransform_.translate_.y = player_->GetWorld().translate_.y + (30.0f + (cometWorldTransform_.scale_.y * 0.5f));
	cometWorldTransform_.translate_.y += player_->GetSpeed();

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

	if (Input::GetInstance()->IsPushKey(DIK_W)) move.z += 0.3f;
	if (Input::GetInstance()->IsPushKey(DIK_S)) move.z -= 0.3f;
	if (Input::GetInstance()->IsPushKey(DIK_A)) move.x -= 0.3f;
	if (Input::GetInstance()->IsPushKey(DIK_D)) move.x += 0.3f;

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

	int health = player_->GetHP();
	int32_t hHealth = std::abs((int32_t)health);

	hpUI_[1]->SetTexture(numberTexHandle_[(hHealth/10)]);
	hHealth = hHealth % 10;
	hpUI_[0]->SetTexture(numberTexHandle_[(hHealth)]);

	// -- 速度メーター更新 -- //
	materUI_[1]->SetScale(Vector2(float(1.0f - player_->GetNormalT()) * materUI_[0]->GetScale().x, materUI_[0]->GetScale().y));

	if (player_->GetNormalT() >= 1.0f&& player_->GetWorld().translate_.y <= 1000.0f) {
		gameBehavior_ = GameBehavior::kPerfectClear;
	}
	// ミサイルとの判定 プレイヤーとエネミーのOBB
	//IsCollision(player_->GetCollision(),);

	// ゴールライン
	if (player_->GetWorld().translate_.y <= 1000.0f) {

		// 一定範囲内 かつ 速度が最大であれば
		if (player_->GetWorld().translate_.y >= 900.0f && player_->GetNormalT() >= 1.0f) {
			// 完全クリア演出へ移行
			gameBehavior_ = GameBehavior::kPerfectClear;
		}

		// 速度が最大じゃない場合
		else if (player_->GetWorld().translate_.y < 900.0f && player_->GetNormalT() < 1.0f) {
			// 通常クリア演出へ移行
			gameBehavior_ = GameBehavior::kGameClear;
		}

	}

	if (player_->GetHP() == 0) {
		// 敗北演出へ移行
		gameBehavior_ = GameBehavior::kGameOver;
	}
}

void GameScene::PerfectUpdate(GameManager* gameManager)
{
	isPerfect_ = true;
	gameManager;
	if (movieScreenNormalT_ < 1.0f) {

		movieScreenNormalT_ += 1.0f / (60.0f * 0.5f);
		if (movieScreenNormalT_ > 1.0f) {
			movieScreenNormalT_ = 1.0f;
		}

		Vector2 newScale = { 1.0f,1.0f };
		newScale.y = OtherCode::ExponentialInterpolation(1.4f, 1.0f, movieScreenNormalT_, 1.0f);
		movieScreenSprite->SetScale(newScale);
	}
	else {

		// time加算処理
		if (cameraNormalT_ < 1.0f) {
			// フレームレート * ゲーム時間(3分)で補間
			cameraNormalT_ += 1.0f / (kFlamerate * 1.0f);

		}

		// カメラ座標
		camera_.translate_.y = OtherCode::ExponentialInterpolation(camera_.translate_.y, 1000.0f, cameraNormalT_, 1.0f);
		camera_.translate_.z = OtherCode::ExponentialInterpolation(0.0f, -1500.0f, cameraNormalT_, 1.0f);
		camera_.rotate_.x = OtherCode::ExponentialInterpolation(1.57f, 0.5f, cameraNormalT_, 1.0f);

		// カメラ座標を保持
		basePos = camera_.translate_;


		if (cameraNormalT_ >= 1.0f) {
			if (cometWorldTransform_.translate_.y >= cometWorldTransform_.scale_.y * 0.5f) {
				cometWorldTransform_.translate_.y += player_->GetSpeed() * 0.5f;
			}

			// シェイク処理
			if (shakePower > 0) {
				shakePos.x = float(rand() % shakePower) - float(shakePower / 2);
				shakePos.y = float(rand() % shakePower) - float(shakePower / 2);

				camera_.translate_.x = basePos.x + shakePos.x;
				camera_.translate_.y = basePos.y + shakePos.y;
			}

			shakeCount++;
			if (shakeCount > 6) {
				shakePower -= 1;
				shakeCount = 0;
			}

			if (shakePower > 30) { return; }

			if (whiteOutNormalT_ < 1.0f) {
				whiteOutNormalT_ += 1.0f / (60.0f * 5.5f);
				if (whiteOutNormalT_ >= 1.0f) {
					whiteOutNormalT_ = 1.0f;
				}
			}

			Vector4 newColor = { 1.0f,1.0f,1.0f,0.0f };
			newColor.w = OtherCode::ExponentialInterpolation(newColor.w, 1.0f, whiteOutNormalT_, 1.1f);
			whiteOutSprite->SetColor(newColor);
		}

	}

}

void GameScene::ClearUpdate(GameManager* gameManager)
{
	// 隕石の落下
	if (cometWorldTransform_.translate_.y >= cometWorldTransform_.scale_.y * -0.25f) {
		cometWorldTransform_.translate_.y += player_->GetSpeed() * 0.4f;
		
	}

	cometWorldTransform_.rotate_.y += 0.02f;

	if (movieScreenNormalT_ < 1.0f) {

		movieScreenNormalT_ += 1.0f / (60.0f * 3.0f);
		if (movieScreenNormalT_ > 1.0f) {
			movieScreenNormalT_ = 1.0f;
		}

		Vector2 newScale = { 1.0f,1.0f };
		newScale.y = OtherCode::ExponentialInterpolation(1.4f, 1.0f, movieScreenNormalT_, 1.0f);
		movieScreenSprite->SetScale(newScale);

		// 視野角 調整
		camera_.fov_ = OtherCode::ExponentialInterpolation(0.3f, 1.4f, movieScreenNormalT_, 1.0f);

		camera_.translate_.y = OtherCode::ExponentialInterpolation(camera_.translate_.y, 1000.0f, movieScreenNormalT_, 1.0f);

		// カメラ座標を保持
		basePos = camera_.translate_;

	}
	else
	{

		if (whiteOutNormalT_ < 1.0f) {

			whiteOutNormalT_ += 1.0f / (60.0f * 7.0f);

			Vector4 newColor = { 1.0f,1.0f,1.0f,0.0f };
			newColor.w = OtherCode::ExponentialInterpolation(newColor.w, 1.0f, whiteOutNormalT_, 1.1f);
			whiteOutSprite->SetColor(newColor);

		}

	}

	gameManager;
}

void GameScene::OverUpdate(GameManager* gameManager)
{
	if (movieScreenNormalT_ == 0.0f) {
		// 隕石が宇宙に消えていく
		cometWorldTransform_.translate_.y = player_->GetWorld().translate_.y;
		movieScreenNormalT_ += 1.0f / (60.0f * 0.5f);
	}
	else if (movieScreenNormalT_ < 1.0f) {

		movieScreenNormalT_ += 1.0f / (60.0f * 0.5f);
		if (movieScreenNormalT_ > 1.0f) {
			movieScreenNormalT_ = 1.0f;
		}

		// 視野角 調整
		camera_.fov_ = OtherCode::ExponentialInterpolation(camera_.fov_, 1.4f, movieScreenNormalT_, 1.0f);

		Vector2 newScale = { 1.0f,1.0f };
		newScale.y = OtherCode::ExponentialInterpolation(1.4f, 1.0f, movieScreenNormalT_, 1.0f);
		movieScreenSprite->SetScale(newScale);
	}

	else {

		// 隕石が宇宙に消えていく
		cometWorldTransform_.translate_.y += 7.5f;
		if (cometWorldTransform_.scale_.x > 0.1f) {
			cometWorldTransform_.scale_.x -= 0.03f;
			cometWorldTransform_.scale_.y -= 0.03f;
			cometWorldTransform_.scale_.z -= 0.03f;
		}
		cometWorldTransform_.rotate_.y -= 0.01f;
		cometWorldTransform_.rotate_.z -= 0.01f;

		// time加算処理
		if (cameraNormalT_ < 1.0f) {
			// フレームレート * ゲーム時間(3分)で補間
			cameraNormalT_ += 1.0f / (kFlamerate * 1.0f);

		}

		// カメラ座標
		camera_.rotate_.x = OtherCode::ExponentialInterpolation(1.57f, -1.57f, cameraNormalT_, 1.0f);

		// カメラ座標を保持
		basePos = camera_.translate_;


		if (cameraNormalT_ >= 1.0f) {
			

			// シェイク処理
			if (shakePower > 0) {
				shakePos.x = float(rand() % shakePower) - float(shakePower / 2);
				shakePos.y = float(rand() % shakePower) - float(shakePower / 2);

				camera_.translate_.x = basePos.x + shakePos.x;
				camera_.translate_.y = basePos.y + shakePos.y;
			}
			else {
				if (!gameManager->IsTransitioned()) {
					gameManager->SceneEnd();
				}
			}

			shakeCount++;
			if (shakeCount > 8) {
				shakePower -= 1;
				shakeCount = 0;
			}

			if (shakePower > 30) { return; }

		}

	}

	gameManager;
}

GameScene::~GameScene() {

	player_->Final();

	delete hpUI_[0];
	delete hpUI_[1];

	// Sprite 解放 
	for (int32_t i = 0; i < speedUI_.size(); i++) {
		delete speedUI_[i];
	}

	// メーター 解放 
	for (int32_t i = 0; i < materUI_.size(); i++) {
		delete materUI_[i];
	}

}