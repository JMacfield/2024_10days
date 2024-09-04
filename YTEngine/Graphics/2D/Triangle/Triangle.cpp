#include "Triangle.h"
#include "Transform.h"

//補助ライブラリ
#include <d3dx12.h>
//動的配列
#include <vector>
#include <TextureManager.h>

Triangle::Triangle() {
	
}



void Triangle::Initialize() {

	//ここでBufferResourceを作る
	//頂点を6に増やす
	vertexResouce_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(VertexData) * 6);
	////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource_=DirectXCommon::GetInstance()->CreateBufferResource(sizeof(Material));

	

	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	wvpResource_ =DirectXCommon::GetInstance()-> CreateBufferResource(sizeof(TransformationMatrix));

	//頂点バッファビューを作成する
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResouce_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 6;
	//１頂点あたりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	


}






//描画
void Triangle::Draw(Transform transform, Vector4 color) {

	//TextureCoordinate(テクスチャ座標系)
	//TexCoord,UV座標系とも呼ばれている

	//左上が(0,0)右下が(1,1)で画像全体を指定することが出来る
	//U(x)V(y)

	//書き込むためのアドレスを取得
	vertexResouce_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	//左下
	vertexData_[0].position = { -0.5f,-0.5f,0.0f,1.0f };
	vertexData_[0].texCoord = { 0.0f,1.0f };
	//上
	vertexData_[1].position = {0.0f,0.5f,0.0f,1.0f};
	vertexData_[1].texCoord = { 0.5f,0.0f };
	//右下
	vertexData_[2].position = {0.5f,-0.5f,0.0f,1.0f} ;
	vertexData_[2].texCoord = { 1.0f,1.0f };
	//範囲外は危険だよ！！


	//左下2
	vertexData_[3].position = {-0.5f,-0.5f,0.5f,1.0f};
	vertexData_[3].texCoord = { 0.0f,1.0f };
	//上2
	vertexData_[4].position = {0.0f,0.0f,0.0f,1.0f};
	vertexData_[4].texCoord = { 0.5f,0.0f };
	//右下2
	vertexData_[5].position = {0.5f,-0.5f,-0.5f,1.0f} ;
	vertexData_[5].texCoord = { 1.0f,1.0f };


	//マテリアルにデータを書き込む
	

	//書き込むためのアドレスを取得
	//reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	materialData_->color = color;
	materialData_->enableLighting = false;
	materialData_->uvTransform = MakeIdentity4x4();
	
	//materialResource_ = CreateBufferResource(directXSetup_->GetDevice(), sizeof(Vector4));
	
	
	
	
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale,transform.rotate,transform.translate);
	//遠視投影行列
	//Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.5f, float() / float(WINDOW_SIZE_HEIGHT), 0.1f, 100.0f);
	//Matrix4x4 worldMatrix = MakeAffineMatrix();
	//遠視投影行列
	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
	
	Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::GetInstance()->GetClientWidth()), float(WinApp::GetInstance()->GetClientHeight()), 0.0f, 100.0f);
	

	//WVP行列を作成
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrixSprite, projectionMatrixSprite));

	//書き込む為のアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	
	
	
	
	wvpData_->WVP = worldViewProjectionMatrix;
	wvpData_->World = MakeIdentity4x4();
	



	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	DirectXCommon::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXCommon::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//マテリアルCBufferの場所を設定
	//ここでの[0]はregisterの0ではないよ。rootParameter配列の0番目
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//CBVを設定する
	//wvp用のCBufferの場所を指定
	//今回はRootParameter[1]に対してCBVの設定を行っている
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());


	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	
	if (textureHandle_ != 0) {
		TextureManager::GraphicsCommand(textureHandle_);

	}
	//directXSetup_->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_);
	

	//描画(DrawCall)３頂点で１つのインスタンス。
	DirectXCommon::GetInstance()->GetCommandList()->DrawInstanced(6, 1, 0, 0);

}

Triangle::~Triangle() {

}

