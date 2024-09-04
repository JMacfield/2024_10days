#pragma once
#include "Animation.h"
#include <array>
struct Skeleton;



class AnimationManager{
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	AnimationManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AnimationManager() = default;


public:
	
	/// <summary>
	/// インスタンス
	/// </summary>
	/// <returns></returns>
	static AnimationManager* GetInstance();

	//コピーコンストラクタ禁止
	AnimationManager(const AnimationManager& modelManager) = delete;

	//代入演算子を無効にする
	AnimationManager& operator=(const AnimationManager& modelManager) = delete;


private:
	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="directoryPath">フォルダ名</param>
	/// <param name="fileName">ファイル名</param>
	/// <returns></returns>
	static Animation LoadAnimationFile(const std::string& directoryPath, const std::string& fileName);


	/// <summary>
	/// 任意の時刻の値を取得(Vector3版)
	/// </summary>
	/// <param name="keyFrames"></param>
	/// <param name="time"></param>
	/// <returns></returns>
	static Vector3 CalculationValue(const std::vector<KeyFrameVector3>& keyFrames, float time);

	/// <summary>
	///  任意の時刻の値を取得(Quaternion版)
	/// </summary>
	/// <param name="keyFrames"></param>
	/// <param name="time"></param>
	/// <returns></returns>
	static Quaternion CalculationValue(const std::vector<KeyFrameQuaternion>& keyFrames, float time);



public:
	/// <summary>
	/// 実際に使う方の読み込み
	/// </summary>
	/// <param name="directoryPath">フォルダ名</param>
	/// <param name="fileName">ファイル名</param>
	/// <returns></returns>
	static uint32_t LoadFile(const std::string& directoryPath, const std::string& fileName);


	static void ApplyAnimation(Skeleton& skeleton,uint32_t animationHandle,uint32_t modelHandle, float animationTime);

private:
	//読み込みの最大数
	static const uint32_t ANIMATION_MAX_AMOUNT_ = 512;


	struct AnimationInformation {
		//モデルデータ
		Animation animationData;
		
		//ハンドル
		uint32_t handle;

		//ファイル名など
		std::string directoryPath;
		std::string fileName;
	};

	static uint32_t handle_;

	//mapにしたい
	std::array<AnimationInformation, ANIMATION_MAX_AMOUNT_> animationInfromtion_;


};

