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

	// スプライト
	testTex_ = TextureManager::GetInstance()->LoadTexture("Resources/AssignmentTexture/Transition.png");

	test_ = Sprite::Create(testTex_, Vector2(0.0f, 0.0f));
	test_->SetScale({ 10,10 });
	test_->SetPosition({ 640,360 });
	test_->SetAnchorPoint({ 0.5f,0.5f });

	for (int32_t i = 0; i < transitionSprites_.size(); i++) {
		for (int32_t j = 0; j < transitionSprites_[i].size(); j++) {
			transitionSprites_[i][j] = Sprite::Create(testTex_, Vector2(float(i * 80 + 40), float(j * 80 + 40)));
			transitionSprites_[i][j]->SetTextureSize({ 8,8 });
			transitionSprites_[i][j]->SetAnchorPoint({ 0.5f,0.5f });

		}
	}


	normalT_ = 0.0f;

}

void TitleScene::Update(GameManager* gameManager)
{
	// デバッグ用 シーン切替
	if (Input::GetInstance()->IsPushKey(DIK_N))
	{
		gameManager->ChangeScene(new GameScene);
	}

	if (normalT_ < 1.0f) {
		normalT_ += 1.0f / 60.0f;
		if (normalT_ > 1.0f) {
			normalT_ = 1.0f;
		}
	}

	float newScale = OtherCode::ExponentialInterpolation(0.0f, 1.0f, normalT_, 1.0f);;
	float newRot = OtherCode::ExponentialInterpolation(1.0f, 0.0f, normalT_, 1.0f);;

	// 上記の計算結果をすべてのスプライトに反映
	for (int32_t i = 0; i < transitionSprites_.size(); i++) {
		for (int32_t j = 0; j < transitionSprites_[i].size(); j++) {
			transitionSprites_[i][j]->SetScale(Vector2(newScale,newScale));
			transitionSprites_[i][j]->SetRotate(newRot);
		}
	}

#ifdef _DEBUG

	ImGui::Begin("Debug");
	ImGui::Text("Now, This is Title.");
	ImGui::End();


	ImGui::Begin("transition");

	// Scale
	Vector2 scale = transitionSprites_[0][0]->GetScale();
	ImGui::DragFloat("Scale", &scale.x, 0.01f, -6.28f, 6.28f);
	scale.y = scale.x;
	transitionSprites_[0][0]->SetScale(scale);

	// Rot
	float rot = transitionSprites_[0][0]->GetRotate();
	ImGui::DragFloat("Rot", &rot, 0.01f, -6.28f, 6.28f);
	transitionSprites_[0][0]->SetRotate(rot);

	// 平行移動
	Vector2 pos = transitionSprites_[0][0]->GetPosition();
	ImGui::DragFloat2("Pos", &pos.x, 1.0f, -2048.0f, 2048.0f);
	transitionSprites_[0][0]->SetPosition(pos);

	for (int32_t i = 0; i < transitionSprites_.size(); i++) {
		for (int32_t j = 0; j < transitionSprites_[i].size(); j++) {
			transitionSprites_[i][j]->SetScale(scale);
			transitionSprites_[i][j]->SetRotate(rot);
			//transitionSprites_[i][j]->SetPosition(Vector2(float(pos.x + i * 10), float(pos.y + j * 10)));
		}
	}

	ImGui::End();

#endif // _DEBUG


}

void TitleScene::Draw()
{
	
	//test_->Draw();
	
	for (int32_t i = 0; i < transitionSprites_.size(); i++) {
		for (int32_t j = 0; j < transitionSprites_[i].size(); j++) {
			transitionSprites_[i][j]->Draw();
		}
	}

}

TitleScene::~TitleScene()
{
	for (int32_t i = 0; i < transitionSprites_.size(); i++) {
		for (int32_t j = 0; j < transitionSprites_[i].size(); j++) {
			delete transitionSprites_[i][j];
		}
	}
	delete test_;
}
