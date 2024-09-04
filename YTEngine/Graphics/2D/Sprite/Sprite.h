#pragma once
#include "DirectXCommon.h"
#include "ConvertLog.h"
#include <DirectXTex.h>
#include "PipelineManager.h"

#include "Vector4.h"
#include "Vector2.h"
#include "Matrix4x4.h"
#include "Matrix4x4Calculation.h"
#include "Transform.h"
#include <VertexData.h>
#include <Material.h>

#include <string>
#include <format>
#include <DirectionalLight.h>
#include <TransformationMatrix.h>

class Sprite {
public:
	//コンストラクタ
	Sprite();

	
	static Sprite* Create(uint32_t textureHandle,Vector2 position);


	//描画
	void Draw();


	//デストラクタ
	~Sprite();


	
public:
#pragma region アクセッサ
	
	//色
	void SetTransparency(float transparency) {
		this->color_.w = transparency;
	}
	void SetColor(Vector4 color) {
		this->color_ = color;
	}



	//拡縮
	void SetScale(Vector2 scale) {
		this->scale_ = scale;
	}
	const Vector2 GetScale() {
		return scale_;
	}



	//回転
	void SetRotate(float rotate) {
		this->rotate_ = rotate;
	}
	const float GetRotate() {
		return rotate_;
	}



	//位置
	void SetPosition(Vector2 position) {
		this->position_ = position;
	}
	const Vector2 GetPosition() {
		return position_;
	}



	//アンカーポイント
	void SetAnchorPoint(Vector2 point) {
		this->anchorPoint_ = point;
	}
	const Vector2 GetAnchorPoint() {
		return anchorPoint_;
	}
	


	//フリップ
	void SetFlipX(bool isFlipX){
		this->isFlipX_=isFlipX;
	}
	const bool GetFlipX(){
		return isFlipX_;
	}

	void SetFlipY(bool isFlipY){
		this->isFlipY_=isFlipY;
	}
	const bool GetFlipY(){
		return isFlipY_;
	}



	void SetInvisible(bool isInvisible) {
		this->isInvisible_ = isInvisible;
	}
	const bool GetInvisible() {
		return isInvisible_;
	}


	//UV
	void SetTextureLeftTop(Vector2 textureLeftTop) {
		this->textureLeftTop_ = textureLeftTop;
	}
	const Vector2 GetTextureLeftTop() {
		return textureLeftTop_;
	}

	void SetTextureSize(Vector2 textureSize) {
		this->textureSize_ = textureSize;
	}
	const Vector2 GetTextureSize() {
		return textureSize_;
	}

	void SetUVMode(bool isUVMode) {
		this->isUVSetting_ = isUVMode;
	}
	const bool GetUVMode() {
		return isUVSetting_;
	}



#pragma endregion



private:
	//初期化
	void Initialize(uint32_t textureHandle,Vector2 position);
	
	//Vertex
	void CreateVertexBufferView();
	//Index
	void CreateIndexBufferView();



private:


#pragma region リソース

	

	//Sprite用
	//三角形2枚
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;

	//マテリアル用のリソースを作る
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	Material* materialData_ = nullptr;
	


	//Sprite用のTransformationMatrix用のリソースを作る。
	//Matrix4x4 1つ分サイズを用意する
	ComPtr<ID3D12Resource> transformationMatrixResource_ = nullptr;
	TransformationMatrix* transformationMatrixData_ = nullptr;


	//描画
	VertexData* vertexData_ = nullptr;


	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};

	


	//index用
	ComPtr<ID3D12Resource> indexResource_ = nullptr;
	//IndexBufferViewを作成
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};
	//インデックスデータ
	uint32_t* indexData_ = nullptr;

	static const int MAX_TEXTURE_ = 20;


	//画像読み込み
	ComPtr<ID3D12Resource> textureResource_ = nullptr;
	ComPtr<ID3D12Resource> resource_ = nullptr;

	


#pragma endregion


	enum VERTEX_POSITION {
		//左下
		LEFT_BOTTOM,
	
		//左上
		LEFT_TOP,
	
		//右下
		RIGHT_BOTTOM,
	
		//右上
		RIGHT_TOP,

	};


	Transform uvTransformSprite_ = {};

	int textureIndex_ = 0;
	
	//テクスチャの情報
	D3D12_RESOURCE_DESC resourceDesc_{};

	//サイズ
	Vector2 size_ = {};

	//S
	Vector2 scale_ = { 1.0f,1.0f };
	//R
	float rotate_ = 0.0f;
	//T
	Vector2 originPosition_ = { 0.0f,0.0f };
	Vector2 position_ = { 0.0f,0.0f };

	//アンカーポイント
	Vector2 anchorPoint_ = { 0.0f,0.0f };
	//色
	Vector4 color_ = {};

	//左右フリップ
	bool isFlipX_ = false;
	//上下フリップ
	bool isFlipY_ = false;


	//非表示
	bool isInvisible_ = false;


	//テクスチャ範囲設定
	Vector2 textureLeftTop_ = { 0.0f,0.0f };
	//テクスチャ切り出しサイズ
	Vector2 textureSize_ = { 100.0f,100.0f };

	bool isUVSetting_ = false;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//ブレンドモード
	uint32_t blendModeNumber_ = 1;
};