#include "GameManager.h"
#include "GameScene/GameScene.h"
#include "GameScene/TitleScene.h"
#include "GameScene/OverScene.h"
#include "GameScene/ClearScene.h"

//コンストラクタ
GameManager::GameManager() {
	
}
	
void GameManager::Initialize() {
	
	currentGamaScene_ = new TitleScene();
	currentGamaScene_->Initialize();

	transition = std::make_unique<Transition>();
	transition->Init();

}


void GameManager::Update() {

	transition->Update();

#ifdef _DEBUG

	// 画面上部にデバッグ用のメニューバーを表示
	//ImGui::BeginMainMenuBar();

	////
	//// デバッグ用シーン切り替え
	////

	//const char* items[] = { "Title Scene", "GameScene", "Over Scene", "Clear Scene" };
	//static int currentItem = 0; 

	//// 横幅を設定 (例: 150.0f)
	//ImGui::SetNextItemWidth(150.0f);
	//
	//// シーンを選択する
	//if (ImGui::Combo("", &currentItem, items, IM_ARRAYSIZE(items)))
	//{
	//	// 選択したシーンへ遷移を行う
	//	switch (currentItem)
	//	{
	//	case 0:
	//		ChangeScene(new TitleScene);
	//		break;
	//	case 1:
	//		ChangeScene(new GameScene);
	//		break;
	//	case 2:
	//		ChangeScene(new OverScene);
	//		break;
	//	case 3:
	//		ChangeScene(new ClearScene);
	//		break;

	//	default:
	//		break;
	//	}

	//}

	//ImGui::EndMainMenuBar();
	
#endif // _DEBUG


	currentGamaScene_->Update(this);
}

void GameManager::Draw() {

	currentGamaScene_->Draw();

	transition->Draw();

}



void GameManager::ChangeScene(IGameScene* newGameScene) {
	
	delete currentGamaScene_;
	currentGamaScene_ = newGameScene;
	
	currentGamaScene_->Initialize();
	transition->SceneStartTransition();
}


//デストラクタ
GameManager::~GameManager() {
	delete currentGamaScene_;

}

void GameManager::SceneEnd()
{
	if (!transition->IsTransition()) {
		transition->SceneEndTransition();
	}
}

bool GameManager::IsTransitioned()
{
	return transition->IsTransitioned();
}



bool GameManager::IsStartTransition()
{
	return transition->IsTransition();
}
