#include "PerfectScene.h"
#include "TitleScene.h"

#include <imgui.h>
#include <Input.h>
#include "GlobalVariables.h"
#include "ModelManager.h"
#include "TextureManager.h"

#include "Other/Code/OtherCode.h"

void PerfectScene::Initialize()
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

	//// -- 床 仮置き -- //
	//planeModelHandle_ = ModelManager::GetInstance()->LoadModelFile("Resources/AssignmentModel/earth", "brokenEarth.gltf");
	//planeModel_.reset(Model::Create(planeModelHandle_));
	//planeModel_->SetLighting(false);
	//planeModelWorldTransform_.Initialize();
	//planeModelWorldTransform_.scale_ = { 16.0f,16.0f, 16.0f };
	//planeModelWorldTransform_.translate_ = { 0.0f,-4.0f,32.0f };

	// -- タイトルのUI -- //
	titleUITex_[0] = TextureManager::LoadTexture("Resources/AssignmentTexture/UI/perfect.png");
	titleUITex_[1] = TextureManager::LoadTexture("Resources/AssignmentTexture/UI/remove.png");

	
	titleUI_[0].reset(Sprite::Create(titleUITex_[0], Vector2(640.0f, 200.0f)));
	titleUI_[0]->SetAnchorPoint(Vector2(0.5f, 0.5f));
	titleUI_[0]->SetScale(Vector2(0.5f, 0.5f));
	

	titleUI_[1].reset(Sprite::Create(titleUITex_[1], Vector2(640.0f, 500.0f)));
	titleUI_[1]->SetAnchorPoint(Vector2(0.5f, 0.5f));
	titleUI_[1]->SetScale(Vector2(0.5f, 0.5f));

	normalT_ = 0.5f;
	isUpper_ = true;
}

void PerfectScene::Update(GameManager* gameManager)
{
	// -- カメラ 更新 -- //
	camera_.Update();

	// -- 天球 更新 -- //
	skydomeModelWorldTransform_.rotate_.y += 0.01f;
	skydomeModelWorldTransform_.rotate_.z += 0.0025f;
	skydomeModelWorldTransform_.Update();

	// -- 天球 更新 -- //
	skydomeModelWorldTransform_.Update();

	// デバッグ用 シーン切替
	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE))
	{
		gameManager->SceneEnd();
	}
	if (gameManager->IsTransitioned()) {
		gameManager->ChangeScene(new TitleScene);
	}
	else {

		// normalT 増減
		if (isUpper_) {
			if (normalT_ < 1.0f) {
				normalT_ += 1.0f / 30.0f;

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
		float rotUI = OtherCode::ExponentialInterpolation(0.3f, 1.0f, normalT_, 1.0f);

		titleUI_[0]->SetScale(Vector2(rotUI,rotUI));

	}

	ImGui::Begin("Debug");
	ImGui::Text("Now, This is Clear.");
	ImGui::End();
}

void PerfectScene::Draw()
{
	// -- 天球 描画 -- //
	skydomeModel_->Draw(skydomeModelWorldTransform_, camera_);

	// -- 地球 描画 -- //
	//planeModel_->Draw(planeModelWorldTransform_, camera_);

	for (int32_t i = 0; i < titleUI_.size(); i++) {
		if (i == 2) { continue; }
		titleUI_[i]->Draw();
	}
}