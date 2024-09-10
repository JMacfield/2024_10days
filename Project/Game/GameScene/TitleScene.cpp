#include "TitleScene.h"
#include "GameScene.h"

#include <imgui.h>
#include <Input.h>
#include "GlobalVariables.h"
#include "ModelManager.h"

#include "Other/Code/OtherCode.h"

TitleScene::TitleScene()
{
}

void TitleScene::Initialize()
{



}

void TitleScene::Update(GameManager* gameManager)
{
	// デバッグ用 シーン切替
	if (Input::GetInstance()->IsPushKey(DIK_N))
	{
		gameManager->SceneEnd();
	}
	if (gameManager->IsTransitioned()) {
		gameManager->ChangeScene(new GameScene);
	}
	

#ifdef _DEBUG

	ImGui::Begin("Debug");
	ImGui::Text("Now, This is Title.");
	ImGui::End();

#endif // _DEBUG


}

void TitleScene::Draw()
{

}

TitleScene::~TitleScene()
{

}
