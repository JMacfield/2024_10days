#include "ClearScene.h"
#include "TitleScene.h"

void ClearScene::Initialize()
{

}

void ClearScene::Update(GameManager* gameManager)
{
	// デバッグ用 シーン切替
	if (Input::GetInstance()->IsPushKey(DIK_N))
	{
		gameManager->ChangeScene(new TitleScene);
	}

	ImGui::Begin("Debug");
	ImGui::Text("Now, This is Clear.");
	ImGui::End();
}

void ClearScene::Draw()
{

}