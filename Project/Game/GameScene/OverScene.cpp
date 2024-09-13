#include "OverScene.h"
#include "TitleScene.h"

void OverScene::Initialize()
{

}

void OverScene::Update(GameManager* gameManager)
{
	// デバッグ用 シーン切替
	if (Input::GetInstance()->IsPushKey(DIK_N))
	{
		gameManager->ChangeScene(new TitleScene);
	}

	/*ImGui::Begin("Debug");
	ImGui::Text("Now, This is Over.");
	ImGui::End();*/
}

void OverScene::Draw()
{

}