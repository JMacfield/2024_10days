#pragma once

#include <array>
#include "Sprite.h"

class Transition
{
public:

	Transition();
	~Transition();

	void Init();
	void Update();
	void Draw();

	void SceneStartTransition() {
		isTransition_ = true;
		isSceneEnd_ = false;
		normalT_ = 0.0f;
	}

	void SceneEndTransition() { 
		isTransition_ = true; 
		isSceneEnd_ = true;
		normalT_ = 0.0f;
	}

	// トランジション中であるか
	bool IsTransition();

	// トランジションが終了したか
	bool IsTransitioned();

private:

	// トランジション用
	std::array<std::array<Sprite*, 9>, 16 >transitionSprites_;
	int32_t tex_;
	float normalT_;

	// トランジション中であるか
	bool isTransition_;
	// シーン終了処理中であるか
	bool isSceneEnd_;

};

