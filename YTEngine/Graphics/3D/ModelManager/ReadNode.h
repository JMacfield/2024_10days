#pragma once
#include <Node.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class ReadNode final{
private:
	ReadNode()=default;
	
	~ReadNode() = default;

public:
	//インスタンス
	static ReadNode* GetInstance();

	//コピーコンストラクタ禁止
	ReadNode(const ReadNode& readNode) = delete;

	//代入演算子を無効にする
	ReadNode& operator=(const ReadNode& readNode) = delete;


public:

	Node Read(aiNode* node);

};

