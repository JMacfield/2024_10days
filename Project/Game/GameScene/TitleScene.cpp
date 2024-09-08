#include "TitleScene.h"
#include "GameScene.h"

void TitleScene::Initialize()
{

}

void TitleScene::Update(GameManager* gameManager)
{
	// デバッグ用 シーン切替
	if (Input::GetInstance()->IsPushKey(DIK_N))
	{
		gameManager->ChangeScene(new GameScene);
	}

	ImGui::Begin("Debug");
	ImGui::Text("Now, This is Title.");
	ImGui::End();
}

void TitleScene::Draw()
{

}