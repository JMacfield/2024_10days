#pragma once


#define XAUDIO2_HELPER_FUNCTIONS
#include <xaudio2.h>
#include "xaudio2fx.h"
#include <fstream>
#include <x3daudio.h>
#include <mmsystem.h>
#include <cassert>
#include <array>
#include <XAPOFX.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"xaudio2.lib")




#include <mfapi.h>
#include <mfidl.h>


#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")

#include <wrl.h>
using Microsoft::WRL::ComPtr;


#include <complex>
#include <vector>


#include "AudioStruct.h"

//LoadAudioの時に使う
enum EffectType {
	Filter,
	Reverb,
};


class Audio final {
private:
	//コンストラクタ
	Audio();

	//デストラクタ
	~Audio();
public:
	//インスタンスの取得
	static Audio* GetInstance();

	//コピーコンストラクタ禁止
	Audio(const Audio& obj) = delete;

	//代入演算子を無効にする
	Audio& operator=(const Audio& obj) = delete;

public:

	//初期化
	void Initialize();

#pragma region 基本セット

	//読み込み
	static uint32_t LoadWave(const char* fileName);

	//エフェクト版の読み込み
	static uint32_t LoadWave(const char* fileName, uint32_t effectType);

	static uint32_t LoadMP3(const WCHAR* fileName);


	//音声再生
	void PlayWave(uint32_t audioHandle, bool isLoop);
	//ループ回数あり
	void PlayWave(uint32_t audioHandle, int32_t loopCount);

	/// <summary>
	/// MP3再生
	/// </summary>
	/// <param name="audioHandle"></param>
	/// <param name="isLoop"></param>
	void PlayMP3(uint32_t audioHandle,bool isLoop);

	/// <summary>
	/// MP3再生(ループ回数設定版)
	/// </summary>
	/// <param name="audioHandle"></param>
	/// <param name="loopCount"></param>
	void PlayMP3(uint32_t audioHandle, uint32_t loopCount);


	/// <summary>
	/// 一時停止
	/// </summary>
	/// <param name="ハンドル名"></param>
	void PauseWave(uint32_t audioHandle);

	/// <summary>
	/// 再開
	/// </summary>
	/// <param name="ハンドル名"></param>
	void ResumeWave(uint32_t audioHandle);

	/// <summary>
	/// 停止
	/// </summary>
	/// <param name="ハンドル名"></param>
	void StopWave(uint32_t audioHandle);


#pragma endregion


#pragma region ループ


	/// <summary>
	/// ループから抜ける
	/// </summary>
	/// <param name="ハンドル名"></param>
	void ExitLoop(uint32_t audioHandle);


	/// <summary>
	/// 再生(後半ループ)
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="始まる位置(秒)"></param>
	void AfterLoopPlayWave(uint32_t audioHandle, float second);

	/// <summary>
	/// 再生(前半ループ)
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="ループ間の長さ(秒)"></param>
	void BeforeLoopPlayWave(uint32_t audioHandle, float lengthSecond);


	/// <summary>
	/// 部分ループ
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="開始位置(秒)"></param>
	/// <param name="ループの長さ(秒)"></param>
	void PartlyLoopPlayWave(uint32_t audioHandle, float start, float lengthSecond);


#pragma endregion

	/// <summary>
	/// 音量調節
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="音量"></param>
	void ChangeVolume(uint32_t audioHandle, float volume);

#pragma region ピッチ系

	/// <summary>
	/// 音の高さの変更(滑らか版)
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="値"></param>
	void ChangeFrequency(uint32_t audioHandle, float ratio);


	/// <summary>
	/// ピッチの変更
	/// シンセのように出来るよ
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="値"></param>
	void ChangePitch(uint32_t audioHandle, int32_t scale);

	void StretchAndPitch(uint32_t audioHandle, float timeRatio, float pitchRatio);

#pragma endregion

	/// <summary>
	/// Pan振り
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="Panの値"></param>
	void SetPan(uint32_t audioHandle, float_t pan);

	
#pragma region フィルター

	//単極は必要無いかな

	/// <summary>
	/// ローパスフィルター
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="CutOffの値"></param>
	void SetLowPassFilter(uint32_t audioHandle, float cutOff);

	/// <summary>
	/// ローパスフィルター(Qあり)
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="CutOffの値"></param>
	/// <param name="oneOverQ"></param>
	void SetLowPassFilter(uint32_t audioHandle, float cutOff,float oneOverQ);



	/// <summary>
	/// ハイパスフィルター
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="CutOffの値"></param>
	void SetHighPassFilter(uint32_t audioHandle, float cutOff);

	/// <summary>
	/// ハイパスフィルター(Qあり)
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="CutOffの値"></param>
	/// <param name="oneOverQ"></param>
	void SetHighPassFilter(uint32_t audioHandle, float cutOff, float oneOverQ);

	/// <summary>
	/// バンドパス
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="CutOffの値"></param>
	void SetBandPassFilter(uint32_t audioHandle, float cutOff);

	/// <summary>
	/// バンドパス(Qあり)
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="CutOffの値"></param>
	void SetBandPassFilter(uint32_t audioHandle, float cutOff, float oneOverQ);


	/// <summary>
	/// ノッチフィルター
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="CutOffの値"></param>
	void SetNotchFilter(uint32_t audioHandle, float cutOff);

	/// <summary>
	/// ノッチフィルター
	/// </summary>
	/// <param name="ハンドル名"></param>
	/// <param name="CutOffの値"></param>
	void SetNotchFilter(uint32_t audioHandle, float cutOff, float oneOverQ);

#pragma endregion


	static void CreateSubmixVoice(uint32_t channel);

	void SendChannels(uint32_t audioHandle,uint32_t channelNumber);

	//リバーブ
	void CreateReverb(uint32_t audioHandle, uint32_t channel);

	//エフェクトの効果を無効にする
	void OffEffect(uint32_t audioHandle);

	//エフェクトの効果を有効にする
	void OnEffect(uint32_t audioHandle);


	//解放
	void Release();

private:
	//音声データの開放
	void SoundUnload(uint32_t soundDataHandle);


private:
	//IXAudio2はCOMオブジェクトなのでComPtr管理
	ComPtr<IXAudio2> xAudio2_ = nullptr;
	//マスターボイス
	//最終的にここでまとめるよ(スピーカーみたいな感じだね)
	IXAudio2MasteringVoice* masterVoice_ = nullptr;
	

	//Panに必要な変数
	DWORD dwChannelMask_ = {};
	float outputMatrix_[8] = {};
	float left_ = 0.0f;
	float right_ = 0.0f;



	//Reverb
	IUnknown* pXAPO_ = nullptr;




	//構造体版
	//Texturemanagerとだいたい同じ感じにした
	//音声データの最大数
	static const int SOUND_DATE_MAX_ = 256;
	std::array<AudioInformation, SOUND_DATE_MAX_> audioInformation_{};

	//サブミックス(バス)
	static const int SUBMIXVOICE_AMOUNT_ = 64;
	std::array<IXAudio2SubmixVoice*, SUBMIXVOICE_AMOUNT_> submixVoice_{};


private:




	//自分のエンジンではA4は442Hz基準にする
	//もちろん12段階で1オクターブ
	static const int SCALE_AMOUNT_ = 13;
	const float SEMITONE_RATIO_[SCALE_AMOUNT_] = {
		1.00000f, //C4
		1.05946f, //C#4
		1.12246f, //D4
		1.18921f, //D#4
		1.25992f, //E4
		1.33483f, //F4
		1.41421f, //F#4
		1.49831f, //G4
		1.58740f, //G#4
		1.68179f, //A4
		1.78180f, //A#4
		1.88775f, //B4
		2.00000f  //C5
	};

	//低い方
	const float MINUS_SEMITONE_RATION[SCALE_AMOUNT_] = {
		1.00000f,	//C4
		0.94387f,	//B3
		0.89090f,	//A3#
		0.84090f,	//A3
		0.79370f,	//G#3
		0.74915f,	//G3
		0.70711f,	//F#3
		0.66742f,	//F3
		0.62996f,	//E3
		0.59460f,	//D#3
		0.56123f,	//D3
		0.52973f,	//C#3
		0.50000f,	//C3
	};


};