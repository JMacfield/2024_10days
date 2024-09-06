#pragma once

#include "Model.h"
#include "AnimationModel.h"
#include "TextureManager.h"
#include "Camera.h"

class Enemy {
public: 

	Enemy();
	~Enemy();
	void Initialize();
	void Update();
	void Draw(Camera camera);

	WorldTransform GetWorld() { return worldTransform_; }

private:

	// モデル
	std::unique_ptr<Model> model_ = nullptr;
	// テクスチャ
	uint32_t modelHandle_ = 0;
	// ワールド座標
	WorldTransform worldTransform_ = {};

};

