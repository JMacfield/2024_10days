#include "Transition.h"

#include <imgui.h>
#include <Input.h>

#include "GlobalVariables.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Other/Code/OtherCode.h"

Transition::Transition()
{
}

Transition::~Transition()
{

	for (int32_t i = 0; i < transitionSprites_.size(); i++) {
		for (int32_t j = 0; j < transitionSprites_[i].size(); j++) {
			delete transitionSprites_[i][j];
		}
	}

}

void Transition::Init(){

	// テクスチャ読み込み
	tex_ = TextureManager::GetInstance()->LoadTexture("Resources/AssignmentTexture/Transition.png");

	// スプライト初期化
	for (int32_t i = 0; i < transitionSprites_.size(); i++) {
		for (int32_t j = 0; j < transitionSprites_[i].size(); j++) {
			transitionSprites_[i][j] = Sprite::Create(tex_, Vector2(float(i * 80 + 40), float(j * 80 + 40)));
			transitionSprites_[i][j]->SetTextureSize({ 8,8 });
			transitionSprites_[i][j]->SetAnchorPoint({ 0.5f,0.5f });

		}
	}


	normalT_ = 0.0f;

	// トランジション中であるか
	isTransition_ = true;
	// シーン終了処理中であるか
	isSceneEnd_ = false;

}

void Transition::Update()
{

	// トランジション中でなければ早期リターン
	if (!isTransition_) { return; }

	if (normalT_ < 1.0f) {
		normalT_ += 1.0f / 60.0f;
		if (normalT_ > 1.0f) {
			normalT_ = 1.0f;
			isTransition_ = false;
		}
	}

	// シーン開始時
	if (!isSceneEnd_) {
		float newScale = OtherCode::ExponentialInterpolation(1.0f, 0.0f, normalT_, 1.0f);;
		float newRot = OtherCode::ExponentialInterpolation(0.0f, 1.0f, normalT_, 1.0f);;

		// 上記の計算結果をすべてのスプライトに反映
		for (int32_t i = 0; i < transitionSprites_.size(); i++) {
			for (int32_t j = 0; j < transitionSprites_[i].size(); j++) {
				transitionSprites_[i][j]->SetScale(Vector2(newScale, newScale));
				transitionSprites_[i][j]->SetRotate(newRot);
			}
		}
	}

	// シーン終了時
	if (isSceneEnd_) {
		float newScale = OtherCode::ExponentialInterpolation(0.0f, 1.0f, normalT_, 1.0f);;
		float newRot = OtherCode::ExponentialInterpolation(1.0f, 0.0f, normalT_, 1.0f);;

		// 上記の計算結果をすべてのスプライトに反映
		for (int32_t i = 0; i < transitionSprites_.size(); i++) {
			for (int32_t j = 0; j < transitionSprites_[i].size(); j++) {
				transitionSprites_[i][j]->SetScale(Vector2(newScale, newScale));
				transitionSprites_[i][j]->SetRotate(newRot);
			}
		}
	}

}

void Transition::Draw()
{

	for (int32_t i = 0; i < transitionSprites_.size(); i++) {
		for (int32_t j = 0; j < transitionSprites_[i].size(); j++) {
			transitionSprites_[i][j]->Draw();
		}
	}

}

bool Transition::IsTransitioned()
{
	if(!isTransition_&&isSceneEnd_){
		return true;
	}
	return false;
}
