#include "YTEngine.h"

#include "PipelineManager.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "TextureManager.h"
#include "SrvManager.h"
#include "Audio.h"
#include "GlobalVariables.h"

YTEngine* YTEngine::GetInstance() 
{
	static YTEngine instance;

	return &instance;
}

void YTEngine::Initialize()
{
	const wchar_t* titleBarName = L"YTEngine";
	
	const int WINDOW_SIZE_WIDTH_ = 1280;
	const int WINDOW_SIZE_HEIGHT_ = 720;

	//ウィンドウ
	WinApp::GetInstance()->Initialize(titleBarName,WINDOW_SIZE_WIDTH_,WINDOW_SIZE_HEIGHT_);
	
	//DirectX
	DirectXCommon::GetInstance()->Initialize();
	
	SrvManager::GetInstance()->Initialize();

	//ImGuiManager
	ImGuiManager::GetInstance()->Initialize();
	
	//Input
	Input::GetInstance()->Initialize();
	
	//TextureManager
	TextureManager::GetInstance()->Initialize();
	
	//Audio
	Audio::GetInstance()->Initialize();

	GlobalVariables::GetInstance()->LoadFile();

	//GameManager
	gameManager_ = new GameManager();
	gameManager_->Initialize();
}

void YTEngine::BeginFrame()
{
	DirectXCommon::GetInstance()->BeginFrame();
	SrvManager::GetInstance()->PreDraw();
	ImGuiManager::GetInstance()->BeginFrame();
}

void YTEngine::Update()
{
	GlobalVariables::GetInstance()->GetInstance()->Update();

	ImGuiManager::GetInstance()->Update();

	Input::GetInstance()->Update();
	
	gameManager_->Update();
}

void YTEngine::Draw()
{
	ImGuiManager::GetInstance()->PreDraw();	
	ImGuiManager::GetInstance()->Draw();
	
	gameManager_->Draw();
}


void YTEngine::EndFrame() 
{
	ImGuiManager::GetInstance()->EndFrame();
	DirectXCommon::GetInstance()->EndFrame();			
}

void YTEngine::Operate()
{
	Initialize();
	
	MSG msg{};

	////メインループ
	//ウィンドウの✕ボタンが押されるまでループ
	while (msg.message != WM_QUIT) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			WinApp::GetInstance()->WindowsMSG(msg);
		} 
		else 
		{
			BeginFrame();

			Update();

			Draw();
			
			EndFrame();
		}
	}

	Release();
}

void YTEngine::Release() 
{
	Audio::GetInstance()->Release();
	
	TextureManager::GetInstance()->Release();
	
	ImGuiManager::GetInstance()->Release();
	
	DirectXCommon::GetInstance()->Release();
	
	WinApp::GetInstance()->Close();
}

YTEngine::~YTEngine()
{
	delete gameManager_;
}
