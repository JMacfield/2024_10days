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

// ゲームオブジェクト
#include "GameObject/Player.h"

//StatePatternを使う時は必ず前方宣言をするように
class GameManager;

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
	// -- カメラ -- //
	Camera camera_ = {};

	// -- 床(仮置き) -- //
	std::unique_ptr<Model> planeModel_ = nullptr;
	uint32_t planeModelHandle_ = 0;
	WorldTransform planeModelWorldTransform_ = {};
	

	float debugFloat_;

	// -- Player -- //
	std::unique_ptr<Player> player_;


};