#pragma once
#include "DirectXCommon.h"
#include "ConvertLog.h"

#include "Vector4.h"
#include "Matrix4x4.h"
#include "Matrix4x4Calculation.h"

#include <string>
#include "Material.h"
#include "TransformationMatrix.h"
#include <Camera.h>



class Line {
public:
	//コンストラクタ
	Line();

	//初期化
	void Initialize();

	//描画
	void Draw(Vector3 start,Vector3 end, Camera& camera);



	~Line();

public:
	void SetColor(Vector4 color) {
		this->color_ = color;
	}




private:
	struct LineMaterial {
		Vector4 color;
	};


	struct LineTransformMatrix {
		//始点
		Matrix4x4 WVP;
		Matrix4x4 World;

		//終点
		/*Matrix4x4 endWVP;
		Matrix4x4 endWorld;*/

	};

	//頂点データの拡張
	struct LineVertexData {
		Vector4 position;
	};

private:
	

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};

	//Resource
	ComPtr<ID3D12Resource> vertexResouce_;

	//Resourceにデータを書き込む
	LineVertexData* vertexData_;

	//マテリアル用のリソースを作る
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	

	//TransformationMatrix用のResource
	ComPtr<ID3D12Resource> wvpResource_ = nullptr;
	LineTransformMatrix* wvpData_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Vector4 color_ = {};
};

