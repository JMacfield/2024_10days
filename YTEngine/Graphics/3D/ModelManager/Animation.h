#pragma once

#include <map>
#include <string>

#include "NodeAnimation.h"
#include "Joint.h"
#include "Skeleton.h"

struct Animation {
	//アニメーション全体の尺
	float duration;
	//NodeAnimationの集合
	//Node名でひけるようにする
	std::map<std::string, NodeAnimation>nodeAnimations;

};

/// <summary>
/// アニメーションの読み込み
/// </summary>
/// <param name="directoryPath">フォルダ名</param>
/// <param name="fileName">ファイル名</param>
/// <returns></returns>
Animation LoadAnimationFile(const std::string& directoryPath, const std::string& fileName);

/// <summary>
/// 任意の時刻の値を取得(Vector3版)
/// </summary>
/// <param name="keyFrames"></param>
/// <param name="time"></param>
/// <returns></returns>
Vector3 CalculationValue(const std::vector<KeyFrameVector3>& keyFrames, float time);

/// <summary>
///  任意の時刻の値を取得(Quaternion版)
/// </summary>
/// <param name="keyFrames"></param>
/// <param name="time"></param>
/// <returns></returns>
Quaternion CalculationValue(const std::vector<KeyFrameQuaternion>& keyFrames, float time);


/// <summary>
/// Animationを適用する
/// </summary>
/// <param name="skeleton"></param>
/// <param name="animation"></param>
/// <param name="animationTime"></param>
void ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime);
