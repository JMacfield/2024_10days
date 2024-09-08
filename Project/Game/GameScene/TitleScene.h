#pragma once

#include "GameManager/IGameScene.h"
#include "GameManager/GameManager.h"

#include "Input.h"

class GameManager;

class TitleScene : public IGameScene
{
public:

	// 初期化
	void Initialize() override;

	// 更新
	void Update(GameManager* gameManager) override;

	// 描画
	void Draw() override;

private:

	XINPUT_STATE joyState;
};