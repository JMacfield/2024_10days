#pragma once

#include "GameManager/IGameScene.h"
#include "GameManager/GameManager.h"

#include "Input.h"

#include <array>
#include <vector>
#include <memory>
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

	// トランジション用
	std::array<std::array<Sprite*, 9>, 16 >transitionSprites_;
	Sprite* test_;
	int32_t testTex_;
	float normalT_;

};