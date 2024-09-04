#pragma once
#include "GameManager/IGameScene.h"

#include "Sprite.h"
#include "GameManager/GameManager.h"
#include "Model.h"
#include "AnimationModel.h"
#include "TextureManager.h"
#include "Camera.h"

#include <memory>
#include <Particle3D.h>
#include "SkinCluster.h"

//StatePatternを使う時は必ず前方宣言をするように
class Gamemanager;

class GameScene : public IGameScene {
public:

	//コンストラクタ
	GameScene();

	/// デストラクタ
	~GameScene();

	/// 初期化
	void Initialize()override;

	/// 更新
	void Update(GameManager* gameManager)override;

	/// 描画
	void Draw()override;



private:
	Camera camera_ = {};

	//SimpleSkin
	static const int SIMPLE_SKIN_AMOUNT_ = 2;
	std::unique_ptr<AnimationModel> simpleModel_[SIMPLE_SKIN_AMOUNT_] = { nullptr };
	uint32_t modelHandle = 0;
	uint32_t animationHande_ = {};
	WorldTransform worldTransform_[SIMPLE_SKIN_AMOUNT_] = {};
	
	Skeleton skeleton_[SIMPLE_SKIN_AMOUNT_] = {};
	float animationTime_[SIMPLE_SKIN_AMOUNT_] = {};
	SkinCluster skinCluster_[SIMPLE_SKIN_AMOUNT_] = {};

	//歩き
	static const int WALK_HUMAN_AMOUNT_ = 2;
	std::unique_ptr<AnimationModel> human_[WALK_HUMAN_AMOUNT_] = { nullptr };
	uint32_t humanModelHandle = {};
	uint32_t humanAnimationModel_ = {};
	WorldTransform humanWorldTransform_[WALK_HUMAN_AMOUNT_] = {};
	Skeleton humanSkeleton_[WALK_HUMAN_AMOUNT_] = {};
	float humanAnimationTime_[WALK_HUMAN_AMOUNT_] = {};
	SkinCluster humanSkinCluster_[WALK_HUMAN_AMOUNT_] = {};

	//歩き(アニメーションなし)
	std::unique_ptr<Model> humanNoneAnimation_ = nullptr;
	uint32_t humanNoneAnimationModelHandle_ = 0;
	WorldTransform humanNoneAnimationWorldTransform_ = {};
	
	float debugFloat_;

	std::unique_ptr<Model> noneAnimationModel_ = nullptr;
	WorldTransform noneAnimationWorldTransform_ = {};

	const char* GroupName = "Player";
};