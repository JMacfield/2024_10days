#pragma once
#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <list>


#include <ModelData.h>
#include "Animation.h"

class ModelManager final{
private:
	ModelManager()=default;
	
	~ModelManager()=default;

public:
	//シングルインスタンス
	static ModelManager* GetInstance();

	//コピーコンストラクタ禁止
	ModelManager(const ModelManager& modelManager) = delete;

	//代入演算子を無効にする
	ModelManager& operator=(const ModelManager& modelManager) = delete;


private:

	//モデルデータの読み込み
	static ModelData LoadFile(const std::string& directoryPath, const std::string& fileName);


public:

	



	/// <summary>
	/// モデルデータの読み込み(ハンドルを登録する)
	/// </summary>
	/// <param name="directoryPath">パス</param>
	/// <param name="fileName">ファイル名</param>
	/// <returns></returns>
	static uint32_t LoadModelFile(const std::string& directoryPath, const std::string& fileName);

	/// <summary>
	/// アニメーション付きのglTFを読み込みたいときはこっちで
	/// falseにすればobjも読み込めるよ
	/// </summary>
	/// <param name="directoryPath">パス</param>
	/// <param name="fileName">ファイル名</param>
	/// <param name="isAnimationLoad">アニメーションを読み込むかどうか</param>
	/// <returns></returns>
	static uint32_t LoadModelFile(const std::string& directoryPath, const std::string& fileName,bool isAnimationLoad);





public:

	ModelData GetModelData(uint32_t handle) {
		return modelInfromtion_[handle].modelData;
	}
	Animation GetModelAnimation(uint32_t handle) {
		return modelInfromtion_[handle].animationData;
	}



private:

	//読み込みの最大数
	static const uint32_t MODEL_MAX_AMOUNT_ = 512;

	struct ModelInformation {
		//モデルデータ
		ModelData modelData;
		//アニメーション
		Animation animationData;

		//ハンドル
		uint32_t handle;

		//ファイル名など
		std::string directoryPath;
		std::string filePath;
	};


	//mapにしたい
	std::array<ModelInformation, MODEL_MAX_AMOUNT_> modelInfromtion_{};

};
