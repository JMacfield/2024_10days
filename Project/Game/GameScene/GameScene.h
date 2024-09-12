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

#include "GameObject/Enemy/Enemy.h"
#include "GameObject/Enemy/Randam/Randam.h"

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
	
	// -- 雲 -- //
	std::unique_ptr<Model> clowdModel_;
	uint32_t clowdModelHandle_ = 0;
	WorldTransform clowdModelWorldTransform_ = {};
	
	// -- Player -- //
	Player* player_;

	//Enemy
	std::unique_ptr<Enemy> enemy_;

	//ミサイル
	std::list<std::unique_ptr<Missile>> missiles_{};

	// -- UI -- //

	// 速度
	std::array<Sprite*,7> speedUI_;
	// 数値テクスチャ
	std::array<int32_t, 10> numberTexHandle_;
	// 文字テクスチャ
	std::map<std::string, int32_t> stringTexHandle_;

};