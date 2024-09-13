#pragma once
#include "GameManager/IGameScene.h"

#include "Sprite.h"
#include "GameManager/GameManager.h"
#include "Model.h"
#include "AnimationModel.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Collision.h"

#include <memory>
#include <Particle3D.h>
#include "SkinCluster.h"

#include "Audio.h"

// ゲームオブジェクト
#include "GameObject/Player.h"

#include "GameObject/Enemy/Enemy.h"
#include "GameObject/Enemy/Randam/Randam.h"

//StatePatternを使う時は必ず前方宣言をするように
class GameManager;

// ゲーム進行を示す
enum class GameBehavior{
	kStart,
	kInGame,
	kPerfectClear,
	kGameClear,
	kGameOver,
};

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

	// 更新処理分割
	void StartUpdate(GameManager* gameManager);
	void InGameUpdate(GameManager* gameManager);
	void PerfectUpdate(GameManager* gameManager);
	void ClearUpdate(GameManager* gameManager);
	void OverUpdate(GameManager* gameManager);

private:
	// -- カメラ -- //
	Camera camera_ = {};
	float cameraNormalT_; // 補間用の時間

	// -- ゲーム進行段階 -- //
	GameBehavior gameBehavior_;

	// -- 天球  -- //
	std::unique_ptr<Model> skydomeModel_ = nullptr;
	uint32_t skydomeModelHandle_ = 0;
	WorldTransform skydomeModelWorldTransform_ = {};

	// -- 床(仮置き) -- //
	std::unique_ptr<Model> planeModel_ = nullptr;
	uint32_t planeModelHandle_ = 0;
	WorldTransform planeModelWorldTransform_ = {};
	
	// -- 雲 -- //
	std::unique_ptr<Model> clowdModel_;
	uint32_t clowdModelHandle_ = 0;
	WorldTransform clowdModelWorldTransform_ = {};
	
	// -- 隕石 -- //
	std::unique_ptr<Model> cometModel_;
	int32_t cometModelHandle_;
	WorldTransform cometWorldTransform_;

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

	std::array<Sprite*, 2> hpUI_;

	// 速度メーター
	std::array<Sprite*, 2> materUI_;
	// メーターテクスチャ
	std::array<int32_t, 2> materTex_;
	// メーター段階
	int32_t materStep_;

	// -- シェイク強度 -- //
	Vector3 basePos;
	Vector2 shakePos;
	int32_t shakePower;
	int32_t shakeCount;

	// -- ホワイトアウト -- //
	std::unique_ptr<Sprite> whiteOutSprite;
	int32_t whiteOutTexHandle_;
	float whiteOutNormalT_;

	// -- 上下黒帯 -- //
	std::unique_ptr<Sprite> movieScreenSprite;
	int32_t movieScreenTexHandle_;
	float movieScreenNormalT_;

	// インゲームBGM
	uint32_t gameBGMHandle_;

	bool isAudioPlay_ = true;

	std::unique_ptr<Sprite> hpIcon_;
};