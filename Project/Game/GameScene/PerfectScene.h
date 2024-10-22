#pragma once

#include "GameManager/IGameScene.h"
#include "GameManager/GameManager.h"

#include "Input.h"
#include "Camera.h"

#include "Model.h"
#include <array>
#include "Sprite.h"

class GameManager;

class PerfectScene : public IGameScene
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

	// -- カメラ -- //
	Camera camera_ = {};

	// -- 天球  -- //
	std::unique_ptr<Model> skydomeModel_ = nullptr;
	uint32_t skydomeModelHandle_ = 0;
	WorldTransform skydomeModelWorldTransform_ = {};

	// -- 地球 -- //
	std::unique_ptr<Model> planeModel_ = nullptr;
	uint32_t planeModelHandle_ = 0;
	WorldTransform planeModelWorldTransform_ = {};

	// -- タイトルのUI -- //
	std::array<std::unique_ptr<Sprite>, 2> titleUI_;
	std::array<int32_t, 2> titleUITex_;

	float normalT_;
	bool isUpper_;
};