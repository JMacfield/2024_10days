#pragma once

#include "IGameScene.h"

#include "Other/Transition.h"

class GameManager {
public:
	
	//コンストラクタ
	GameManager();

	//初期化
	void Initialize();

	//シーンチェンジ
	void ChangeScene(IGameScene* newGameScene);

	//更新
	void Update();

	//描画
	void Draw();

	//デストラクタ
	~GameManager();

	// シーン終了をマネージャに伝える
	void SceneEnd();

	// シーン終了状態でかつトランジションが終了したか
	bool IsTransitioned();

	// シーン開始時のトランジションが終了したか
	bool IsStartTransition();

private:

	//StatePatternに必要な変数
	IGameScene* currentGamaScene_ = nullptr;


	std::unique_ptr< Transition> transition;
	
};