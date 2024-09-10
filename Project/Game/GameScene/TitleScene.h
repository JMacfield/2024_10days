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

};