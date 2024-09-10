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

	// -- タイトルのUI -- //
	titleUITex_[0] = TextureManager::LoadTexture("Resources/AssignmentTexture/UI/title.png");
	titleUITex_[1] = TextureManager::LoadTexture("Resources/AssignmentTexture/UI/start.png");
	titleUITex_[2] = TextureManager::LoadTexture("Resources/AssignmentTexture/UI/0.png");
	titleUITex_[3] = TextureManager::LoadTexture("Resources/AssignmentTexture/UI/exit.png");
	
	for (int32_t i = 0; i < titleUI_.size(); i++) {
		titleUI_[i].reset(Sprite::Create(titleUITex_[i], Vector2(640.0f, float(i + 1) * 80.0f + 260.0f)));
		titleUI_[i]->SetAnchorPoint(Vector2(0.5f, 0.5f));
	}

	titleUI_[0]->SetScale(Vector2(1.5f, 1.5f));
	titleUI_[0]->SetPosition(Vector2(640.0f, 140.0f));


	selectNumber_ = 0;;
	normalT_ = 0.5f;
	isUpper_ = true;

}

void TitleScene::Update(GameManager* gameManager)
{
	// デバッグ用 シーン切替
	if (Input::GetInstance()->IsPushKey(DIK_N))
	{
		gameManager->SceneEnd();
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

			if (Input::GetInstance()->IsTriggerKey(DIK_UP)) {
				selectNumber_ = 2;
				normalT_ = 0.5f;
				isUpper_ = true;

				titleUI_[1]->SetRotate(0.0f);
			}

			if (Input::GetInstance()->IsTriggerKey(DIK_DOWN)) {
				selectNumber_ = 2;
				normalT_ = 0.5f;
				isUpper_ = true;

				titleUI_[1]->SetRotate(0.0f);
			}



			break;
		case 1:

			break;
		case 2:

			titleUI_[3]->SetRotate(rotUI);

			if (Input::GetInstance()->IsTriggerKey(DIK_UP)) {
				selectNumber_ = 0;
				normalT_ = 0.5f;
				isUpper_ = true;

				titleUI_[3]->SetRotate(0.0f);
			}

			if (Input::GetInstance()->IsTriggerKey(DIK_DOWN)) {
				selectNumber_ = 0;
				normalT_ = 0.5f;
				isUpper_ = true;

				titleUI_[3]->SetRotate(0.0f);
			}


			break;
		default:
			break;
		}

	}

#ifdef _DEBUG

	ImGui::Begin("Debug");
	ImGui::Text("Now, This is Title.");
	ImGui::End();

#endif // _DEBUG


}

void TitleScene::Draw()
{
	for (int32_t i = 0; i < titleUI_.size(); i++) {
		if (i == 2) { continue; }
		titleUI_[i]->Draw();
	}
}

TitleScene::~TitleScene()
{

}
