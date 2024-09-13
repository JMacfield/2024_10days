#include "TitleScene.h"
#include "GameScene.h"

#include <imgui.h>
#include <Input.h>
#include "GlobalVariables.h"
#include "ModelManager.h"

#include "Other/Code/OtherCode.h"

TitleScene::TitleScene()
{
}

void TitleScene::Initialize()
{

	// -- カメラ 初期化 -- //
	camera_.Initialize();
	camera_.rotate_ = { 0.0f,0.0f,0.0f };
	camera_.translate_ = { 0.0f,0.0f,-15.0f };

	// -- 天球  -- //
	skydomeModelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/AssignmentModel/skydome", "skydome.gltf");
	skydomeModel_.reset(Model::Create(skydomeModelHandle_));
	skydomeModel_->SetLighting(false);
	skydomeModelWorldTransform_.Initialize();
	skydomeModelWorldTransform_.scale_ = { 1024.0f,1024.0f,1024.0f };

	// -- 床 仮置き -- //
	planeModelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/AssignmentModel/earth", "earth.gltf");
	planeModel_.reset(Model::Create(planeModelHandle_));
	planeModel_->SetLighting(false);
	planeModelWorldTransform_.Initialize();
	planeModelWorldTransform_.scale_ = { 16.0f,16.0f, 16.0f };
	planeModelWorldTransform_.translate_ = { 0.0f,-4.0f,32.0f };

	// -- タイトルのUI -- //
	titleUITex_[0] = TextureManager::LoadTexture("Resources/AssignmentTexture/UI/title.png");
	titleUITex_[1] = TextureManager::LoadTexture("Resources/AssignmentTexture/UI/start.png");
	titleUITex_[2] = TextureManager::LoadTexture("Resources/AssignmentTexture/UI/0.png");
	titleUITex_[3] = TextureManager::LoadTexture("Resources/AssignmentTexture/UI/enter.png");
	titleUITex_[4] = TextureManager::LoadTexture("Resources/AssignmentTexture/UI/break.png");
	
	for (int32_t i = 0; i < titleUI_.size(); i++) {
		titleUI_[i].reset(Sprite::Create(titleUITex_[i], Vector2(640.0f, float(i + 1) * 80.0f + 260.0f)));
		titleUI_[i]->SetAnchorPoint(Vector2(0.5f, 0.5f));
	}

	titleUI_[0]->SetScale(Vector2(1.5f, 1.5f));
	titleUI_[0]->SetPosition(Vector2(640.0f, 140.0f));

	titleUI_[3]->SetScale(Vector2(0.4f, 0.4f));

	titleUI_[4]->SetScale(Vector2(0.25f, 0.25f));
	titleUI_[4]->SetPosition(Vector2(300.0f, 350.0f));
	titleUI_[4]->SetRotate(-0.25f);

	selectNumber_ = 0;;
	normalT_ = 0.5f;
	isUpper_ = true;

	titleBGMHandle_ = Audio::GetInstance()->LoadMP3(L"Resources/Sounds/title.mp3");

	Audio::GetInstance()->ChangeVolume(titleBGMHandle_, 0.05f);

	tutorialHandle_ = TextureManager::LoadTexture("Resources/AssignmentTexture/tutorial.png");
	tutorial_.reset(Sprite::Create(tutorialHandle_, Vector2(0.0f, 0.0f)));
}

void TitleScene::Update(GameManager* gameManager)
{
	//XINPUT_STATE joyState{};

	// BGM再生
	Audio::GetInstance()->PlayMP3(titleBGMHandle_, true);

	// -- カメラ 更新 -- //
	camera_.Update();

	// -- 地球 更新 -- //
	planeModelWorldTransform_.rotate_.y += 0.01f;
	planeModelWorldTransform_.rotate_.z += 0.0025f;
	planeModelWorldTransform_.Update();

	// -- 天球 更新 -- //
	skydomeModelWorldTransform_.Update();

	if (isAudioPlay_ == false) {
		Audio::GetInstance()->StopMP3(titleBGMHandle_);
	}

	/*if (Input::GetInstance()->IsTriggerKey(DIK_M)) {
		isFlip_ = true;
	}*/

	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE)) {
		count_++;
	}

	/*if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		count_++;
	}*/

	// デバッグ用 シーン切替
	if (count_ == 1) {
		isFlip_ = true;
	}

	if (count_ == 2) {
		gameManager->SceneEnd();
		isAudioPlay_ = false;
		count_ = 3;
	}
	if (gameManager->IsTransitioned()) {
		gameManager->ChangeScene(new GameScene);
	}
	else {

		// normalT 増減
		if (isUpper_) {
			if (normalT_ < 1.0f) {
				normalT_ += 1.0f / 60.0f;

				if (normalT_ >= 1.0f) {
					isUpper_ = false;
				}
			}
		}
		else {

			if (normalT_ > 0.0f) {
				normalT_ -= 1.0f / 60.0f;

				if (normalT_ <= 0.0f) {
					isUpper_ = true;
				}
			}

		}

		// 選択中のUIを回転させる
		float rotUI = OtherCode::ExponentialInterpolation(-1.0f, 1.0f, normalT_, 1.0f);

		switch (selectNumber_)
		{
		case 0:

			titleUI_[1]->SetRotate(rotUI);

		default:
			break;
		}

	}

#ifdef _DEBUG

	/*ImGui::Begin("Debug");
	ImGui::Text("Now, This is Title.");
	ImGui::End();*/

#endif // _DEBUG


}

void TitleScene::Draw()
{
	// -- 天球 描画 -- //
	skydomeModel_->Draw(skydomeModelWorldTransform_, camera_);

	// -- 地球 描画 -- //
	planeModel_->Draw(planeModelWorldTransform_, camera_);

	for (int32_t i = 0; i < titleUI_.size(); i++) {
		if (i == 2) { continue; }
		if (isFlip_ == false) {
			titleUI_[i]->Draw();
		}
	}

	if (isFlip_ == true) {
		tutorial_->Draw();
	}
}

TitleScene::~TitleScene()
{

}
