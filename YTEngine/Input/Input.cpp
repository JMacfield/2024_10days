#include "Input.h"

//コンストラクタ
Input::Input() {

}

//シングルインスタンス
Input* Input::GetInstance() {
	//関数内static変数として宣言する
	static Input instance;

	return &instance;
}



void Input::Initialize() {
	

	HRESULT hr;
	

	//DirectInputオブジェクトの生成

	hr = DirectInput8Create(
		WinApp::GetInstance()->GetHInstance(),
		DIRECTINPUT_VERSION, 
		IID_IDirectInput8, 
		(void**)&directInput_, nullptr);

	assert(SUCCEEDED(hr));


	//キーボードデバイスの生成
	
	hr = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(hr));
	//マウスデバイスの生成
	hr = directInput_->CreateDevice(GUID_SysMouse, &mouseDevice_, NULL);
	assert(SUCCEEDED(hr));

	
	//入力データの形式のセット
	//標準形式
	hr = keyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));
	//マウスの方の入力データ形式セット
	hr = mouseDevice_->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(hr));



	hr = keyboard_->SetCooperativeLevel(WinApp::GetInstance()->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));
	hr = mouseDevice_->SetCooperativeLevel(WinApp::GetInstance()->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(hr));


}

#pragma region キーボード

//Push状態
bool Input::IsPushKey(uint8_t keyNumber) {
	//指定されていたキーを押していればtrueを返す
	if (currentKey_[keyNumber]!=0) {
		return true;
	}
	//押されていなければfalseを返す
	return false;

}

//トリガー
bool Input::IsTriggerKey(uint8_t keyNumber) {
	if (currentKey_[keyNumber]!=0 && preKey_[keyNumber]==0) {
		return true;
	}

	return false;
}

#pragma endregion

#pragma region マウス
//Push状態
bool Input::IsPushMouse(int32_t mouseNumber) {
	if (currentMouse_.rgbButtons[mouseNumber] != 0) {
		return true;
	}
	return false;
}

//Trigger状態
bool Input::IsTriggerMouse(int32_t mouseNumber) {
	if (currentMouse_.rgbButtons[mouseNumber] != 0 && preMouse_.rgbButtons[mouseNumber] == 0) {
		return true;
	}
	return false;
}
#pragma endregion

#pragma region コントローラー

//状態を取得
bool Input::GetJoystickState(XINPUT_STATE& state){
	DWORD dwResult = XInputGetState(0, &state);
	if (dwResult == ERROR_SUCCESS){
		return true;
	}
	return false;
	
}

bool Input::IsPushLeft(XINPUT_STATE& state) {
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
		return true;
	}
	return false;
}

bool Input::IsPushRight(XINPUT_STATE& state) {
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		return true;
	}
	return false;
}

Vector2 Input::GetJoyLStickPos(const float& mode)
{
	return Vector2
	(
		Input::GetInstance()->state_.Gamepad.sThumbLX / mode,
		Input::GetInstance()->state_.Gamepad.sThumbLY / mode
	);
}

Vector2 Input::GetJoyRStickPos(const float& mode)
{
	return Vector2
	(
		Input::GetInstance()->state_.Gamepad.sThumbRX / mode,
		Input::GetInstance()->state_.Gamepad.sThumbRY / mode
	);
}


#pragma endregion

void Input::Update() {

	//前回のキー入力を保存
	memcpy(preKey_, currentKey_, sizeof(currentKey_));

	
	preMouse_ = currentMouse_;

	//キーボード情報の取得開始
	keyboard_->Acquire();
	//マウス情報の取得開始
	mouseDevice_->Acquire();


	keyboard_->GetDeviceState(sizeof(currentKey_), currentKey_);

	mouseDevice_->GetDeviceState(sizeof(DIMOUSESTATE), &currentMouse_);
	

}

Input::~Input() {

}
