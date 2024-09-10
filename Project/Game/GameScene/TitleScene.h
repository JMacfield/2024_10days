#pragma once

#include "GameManager/IGameScene.h"
#include "GameManager/GameManager.h"

#include "Input.h"

#include <array>
#include "Sprite.h"

class GameManager;

class TitleScene : public IGameScene
{
public:

	// デストラクタ
	TitleScene();

	// 初期化
	void Initialize() override;

	// 更新
	void Update(GameManager* gameManager) override;

	// 描画
	void Draw() override;

	// デストラクタ
	~TitleScene();

private:

	XINPUT_STATE joyState;

	// -- タイトルのUI -- //
	std::array<std::unique_ptr<Sprite>, 4> titleUI_;
	std::array<int32_t, 4> titleUITex_;

	int32_t selectNumber_;
	float normalT_;
	bool isUpper_;


};