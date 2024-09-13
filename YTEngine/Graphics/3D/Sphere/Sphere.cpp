#include "Sphere.h"

//補助ライブラリ
#include <d3dx12.h>
//動的配列
#include <vector>
#include <numbers>
#include "VectorCalculation.h"



//コンストラクタ
Sphere::Sphere() {

}

//Resource作成の関数化
ID3D12Resource* Sphere::CreateBufferResource(size_t sizeInBytes) {
	//void返り値も忘れずに
	ID3D12Resource* resource = nullptr;
	
	////VertexResourceを生成
	//頂点リソース用のヒープを設定
	
	uploadHeapProperties_.Type = D3D12_HEAP_TYPE_UPLOAD;

	//頂点リソースの設定
	
	//バッファリソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc_.Width = sizeInBytes;
	//バッファの場合はこれらは1にする決まり
	vertexResourceDesc_.Height = 1;
	vertexResourceDesc_.DepthOrArraySize = 1;
	vertexResourceDesc_.MipLevels = 1;
	vertexResourceDesc_.SampleDesc.Count = 1;

	//バッファの場合はこれにする決まり
	vertexResourceDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//実際に頂点リソースを作る
	//ID3D12Resource* vertexResource_ = nullptr;
	//hrは調査用
	HRESULT hr;
	hr = directXSetup_->GetDevice()->CreateCommittedResource(
		&uploadHeapProperties_,
		D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));

	return resource;
}

D3D12_CPU_DESCRIPTOR_HANDLE Sphere::GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE Sphere::GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}



//頂点バッファビューを作成する
void Sphere::GenerateVertexBufferView() {
	

	
	//リソースの先頭のアドレスから使う
	vertexBufferViewSphere_.BufferLocation = vertexResourceSphere_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferViewSphere_.SizeInBytes = sizeof(VertexData) * SUBDIVISION_ * SUBDIVISION_*6;
	//１頂点あたりのサイズ
	vertexBufferViewSphere_.StrideInBytes = sizeof(VertexData);
	

}


//初期化
void Sphere::Initialize() {
	this->directXSetup_ = DirectXCommon::GetInstance();

	
	vertexResourceSphere_ = CreateBufferResource(sizeof(VertexData) * SUBDIVISION_ *SUBDIVISION_ * 6);
	////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResourceSphere_=CreateBufferResource(sizeof(Material));

	//Lighting
	directionalLightResource_ = CreateBufferResource(sizeof(DirectionalLight));
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color={ 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 1.0f;

	
	transformationMatrixResourceSphere_ = CreateBufferResource(sizeof(TransformationMatrix));

	//DescriptorSize
	descriptorSizeSRV_=directXSetup_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	descriptorSizeRTV_=directXSetup_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	descriptorSizeDSV_=directXSetup_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);


	//頂点バッファビューを作成する
	GenerateVertexBufferView();

	//LoadTexture("Resources/uvChecker.png");
}



void Sphere::Update() {
	//ImGui::Begin("Sphere");
	////ImGui::Checkbox("useMonsterBall", &useMonsterBall_);
	//ImGui::InputFloat3("color", &directionalLightData_->color.x);
	//ImGui::SliderFloat3("color", &directionalLightData_->color.x, 0.0f, 1.0f);
	//ImGui::InputFloat3("direction", &directionalLightData_->direction.x);
	//ImGui::SliderFloat3("direction", &directionalLightData_->direction.x, -1.0f, 1.0f);
	//ImGui::InputFloat("interity", &directionalLightData_->intensity);
	//ImGui::SliderFloat("interity", &directionalLightData_->intensity, 0.0f, 1.0f);

	//ImGui::End();
}


//描画
//左上、右上、左下、右下
void Sphere::Draw(SphereShape sphereCondtion, Transform transform,Matrix4x4 viewMatrix,Matrix4x4 projectionMatrix ,Vector4 color) {

	
	//書き込み用のアドレスを取得
	vertexResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSphere_));


	

	
	const float LON_EVERY = float(std::numbers::pi) * 2.0f / float(SUBDIVISION_);

	//緯度分割1つ分の角度θd
	const float LAT_EVERY = float(std::numbers::pi)/float(SUBDIVISION_);


	//緯度の方向に分割
	for (int latIndex = 0; latIndex < SUBDIVISION_; ++latIndex) {
		//θ
		float lat = - float (std::numbers::pi) / 2.0f + LAT_EVERY * latIndex;
		//経度の方向に分割しながら線を描く
		for (int lonIndex = 0; lonIndex < SUBDIVISION_; ++lonIndex) {
			uint32_t start = (latIndex * SUBDIVISION_ + lonIndex) * 6;
			
			//texcoord専用のxy座標
			float u = float(lonIndex) / float(SUBDIVISION_);
			//下から0上が1になっていたので「1.0f-～」にして逆にする
			float v = 1.0f - float(latIndex) / float(SUBDIVISION_);

			//φ
			float lon = lonIndex * LON_EVERY;
			//頂点にデータを入力する。基準点a
			
			 
			//点間の距離
			float length = 1.0f / SUBDIVISION_;

#pragma region 三角形１枚目 
			//左上(点B)が原点
			//abc
			
			vertexDataSphere_[start].position.x = sphereCondtion.radius*(cos(lat) * cos(lon));
			vertexDataSphere_[start].position.y = sphereCondtion.radius*(sin(lat));
			vertexDataSphere_[start].position.z = sphereCondtion.radius*(cos(lat) * sin(lon)) ;
			vertexDataSphere_[start].position.w = 1.0f;
			//分割分移動
			vertexDataSphere_[start].texCoord.x = u;
			vertexDataSphere_[start].texCoord.y = v + length;
			//法線情報を追加
			vertexDataSphere_[start].normal.x = vertexDataSphere_[start].position.x;
			vertexDataSphere_[start].normal.y = vertexDataSphere_[start].position.y;
			vertexDataSphere_[start].normal.z = vertexDataSphere_[start].position.z;

			//普通にstart+1していけばいいのかな
			//点b(左上)
			vertexDataSphere_[start + 1].position.x = sphereCondtion.radius*(cos(lat + LAT_EVERY)) * cos(lon);
			vertexDataSphere_[start + 1].position.y = sphereCondtion.radius*(sin(lat + LAT_EVERY)) ;
			vertexDataSphere_[start + 1].position.z = sphereCondtion.radius*(cos(lat + LAT_EVERY)) * sin(lon);
			vertexDataSphere_[start + 1].position.w = 1.0f;
			vertexDataSphere_[start + 1].texCoord.x = u;
			vertexDataSphere_[start + 1].texCoord.y = v;
			vertexDataSphere_[start + 1].normal.x = vertexDataSphere_[start + 1].position.x;
			vertexDataSphere_[start + 1].normal.y = vertexDataSphere_[start + 1].position.y;
			vertexDataSphere_[start + 1].normal.z = vertexDataSphere_[start + 1].position.z;


			//点c(右下)
			vertexDataSphere_[start + 2].position.x = sphereCondtion.radius*(cos(lat) * cos(lon + LON_EVERY)) ;
			vertexDataSphere_[start + 2].position.y = sphereCondtion.radius*(sin(lat));
			vertexDataSphere_[start + 2].position.z = sphereCondtion.radius*(cos(lat) * sin(lon + LON_EVERY));
			vertexDataSphere_[start + 2].position.w = 1.0f;
			vertexDataSphere_[start + 2].texCoord.x = u + length;
			vertexDataSphere_[start + 2].texCoord.y = v + length;
			vertexDataSphere_[start + 2].normal.x = vertexDataSphere_[start + 2].position.x;
			vertexDataSphere_[start + 2].normal.y = vertexDataSphere_[start + 2].position.y;
			vertexDataSphere_[start + 2].normal.z = vertexDataSphere_[start + 2].position.z;

#pragma endregion

#pragma region 三角形２枚目
			//bcd
			//点d(右上)
			vertexDataSphere_[start + 3].position.x = sphereCondtion.radius*(cos(lat + LAT_EVERY) * cos(lon + LON_EVERY));
			vertexDataSphere_[start + 3].position.y = sphereCondtion.radius*(sin(lat + LAT_EVERY));
			vertexDataSphere_[start + 3].position.z = sphereCondtion.radius*(cos(lat + LAT_EVERY) * sin(lon + LON_EVERY));
			vertexDataSphere_[start + 3].position.w = 1.0f;
			vertexDataSphere_[start + 3].texCoord.x = u + length;
			vertexDataSphere_[start + 3].texCoord.y = v;
			vertexDataSphere_[start + 3].normal.x = vertexDataSphere_[start + 3].position.x;
			vertexDataSphere_[start + 3].normal.y = vertexDataSphere_[start + 3].position.y;
			vertexDataSphere_[start + 3].normal.z = vertexDataSphere_[start + 3].position.z;


			//点c(右下)
			vertexDataSphere_[start + 4].position.x = sphereCondtion.radius*(cos(lat) * cos(lon + LON_EVERY));
			vertexDataSphere_[start + 4].position.y = sphereCondtion.radius*(sin(lat));
			vertexDataSphere_[start + 4].position.z = sphereCondtion.radius*(cos(lat) * sin(lon + LON_EVERY));
			vertexDataSphere_[start + 4].position.w = 1.0f;
			vertexDataSphere_[start + 4].texCoord.x = u + length;
			vertexDataSphere_[start + 4].texCoord.y = v + length;
			vertexDataSphere_[start + 4].normal.x = vertexDataSphere_[start + 4].position.x;
			vertexDataSphere_[start + 4].normal.y = vertexDataSphere_[start + 4].position.y;
			vertexDataSphere_[start + 4].normal.z = vertexDataSphere_[start + 4].position.z;



			//点b(左上)
			vertexDataSphere_[start + 5].position.x = sphereCondtion.radius*(cos(lat + LAT_EVERY) * cos(lon)) ;
			vertexDataSphere_[start + 5].position.y = sphereCondtion.radius*(sin(lat + LAT_EVERY));
			vertexDataSphere_[start + 5].position.z = sphereCondtion.radius*(cos(lat + LAT_EVERY) * sin(lon));
			vertexDataSphere_[start + 5].position.w = 1.0f;
			vertexDataSphere_[start + 5].texCoord.x = u;
			vertexDataSphere_[start + 5].texCoord.y = v;
			vertexDataSphere_[start + 5].normal.x = vertexDataSphere_[start + 5].position.x;
			vertexDataSphere_[start + 5].normal.y = vertexDataSphere_[start + 5].position.y;
			vertexDataSphere_[start + 5].normal.z = vertexDataSphere_[start + 5].position.z;

			
			

#pragma endregion
			

		}
	
	
	
	}

	


	materialResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = color;
	materialData_->enableLighting=true;

	materialData_->uvTransform = MakeIdentity4x4();

	

	transformationMatrixResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSphere_));
	

	//新しく引数作った方が良いかも
	Matrix4x4 worldMatrixSphere = MakeAffineMatrix(transform.scale,transform.rotate,Add(transform.translate,sphereCondtion.center));
	//遠視投影行列
	Matrix4x4 viewMatrixSphere = MakeIdentity4x4();
	
	Matrix4x4 projectionMatrixSphere = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::GetInstance()->GetClientWidth()), float(WinApp::GetInstance()->GetClientHeight()), 0.0f, 100.0f);
	
	//WVP行列を作成
	Matrix4x4 worldViewProjectionMatrixSphere = Multiply(worldMatrixSphere, Multiply(viewMatrix, projectionMatrix));


	transformationMatrixDataSphere_->WVP = worldViewProjectionMatrixSphere;
	transformationMatrixDataSphere_->World =MakeIdentity4x4();


	
	
	
	//コマンドを積む
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//CBVを設定する
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceSphere_->GetGPUVirtualAddress());

	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXSetup_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewSphere_);

	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSphere_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	//trueだったらtextureSrvHandleGPU2_
	//directXSetup_->GetCommandList()->SetGraphicsRootDescriptorTable(2,useMonsterBall_?textureSrvHandleGPU2_:textureSrvHandleGPU_);
	//Light
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());


	//描画(DrawCall)３頂点で１つのインスタンス。
	directXSetup_->GetCommandList()->DrawInstanced(SUBDIVISION_*SUBDIVISION_*6, 1, 0, 0);

	


}


//解放
void Sphere::Release() {
	//初期化
	vertexResourceSphere_->Release();

	//マテリアル用のリソースを作る
	materialResourceSphere_->Release();
	

	
	transformationMatrixResourceSphere_->Release();
	
	

}

//デストラクタ
Sphere::~Sphere() {

}