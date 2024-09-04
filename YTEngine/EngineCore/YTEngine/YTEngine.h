#pragma once

#include "GameManager/GameManager.h"
#include "WinApp.h"
#include "DirectXCommon.h"

class YTEngine final
{
private:

	//コンストラクタ
	YTEngine() = default;
		
	//デストラクタ
	~YTEngine();

public:

	//インスタンス
	static YTEngine* GetInstance();

	YTEngine(const YTEngine& YTEngine) = delete;

	YTEngine& operator=(const YTEngine& YTEngine) = delete;

public:

	void Operate();

private:

	void Initialize();

 	void BeginFrame();

	void Update();

	void Draw();

	void EndFrame();

	void Release();

private:

	GameManager* gameManager_ = nullptr;
};