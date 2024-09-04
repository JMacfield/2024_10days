#include "YTEngine.h"

//staticはdeleteの後に解放されるよ
static D3DResourceLeakChecker checker;

//Winodwsアプリでもエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	YTEngine* myEngine = YTEngine::GetInstance();

	myEngine->Operate();

	return 0;
}

