#include "PipelineManager.h"

#include <array>

PipelineManager* PipelineManager::GetInstance() 
{
	static PipelineManager instance;

	return &instance;
 }

void PipelineManager::GenaratedLinePSO() 
{
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};
	descriptionRootSignature_.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_ROOT_PARAMETER rootParameters[3] = {};

	descriptionRootSignature_.pParameters = rootParameters;
	
	descriptionRootSignature_.NumParameters = _countof(rootParameters);

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	
	rootParameters[0].Descriptor.ShaderRegister = 0;

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	
	rootParameters[1].Descriptor.ShaderRegister = 1;

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	
	rootParameters[2].Descriptor.ShaderRegister = 0;

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	
	staticSamplers[0].ShaderRegister = 0;
	
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	descriptionRootSignature_.pStaticSamplers = staticSamplers;
	descriptionRootSignature_.NumStaticSamplers = _countof(staticSamplers);

	HRESULT hr_ = {};
	
	hr_ = D3D12SerializeRootSignature(&descriptionRootSignature_,
		D3D_ROOT_SIGNATURE_VERSION_1, &PipelineManager::GetInstance()->linePSO_.signatureBlob_, &PipelineManager::GetInstance()->linePSO_.errorBlob_);
	
	if (FAILED(hr_)) 
	{
		Log(reinterpret_cast<char*>(PipelineManager::GetInstance()->linePSO_.errorBlob_->GetBufferPointer()));
		assert(false);
	}

	hr_ = DirectXCommon::GetInstance()->GetDevice()->CreateRootSignature(0, PipelineManager::GetInstance()->linePSO_.signatureBlob_->GetBufferPointer(),
		PipelineManager::GetInstance()->linePSO_.signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&PipelineManager::GetInstance()->linePSO_.rootSignature_));

	assert(SUCCEEDED(hr_));
	
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	D3D12_BLEND_DESC blendDesc{};
	
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	//RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	//ShaderをCompileする
	PipelineManager::GetInstance()->linePSO_.vertexShaderBlob_ = CompileShaderManager::GetInstance()->CompileShader(L"Resources/Shader/Line3D/Line3D.VS.hlsl", L"vs_6_0");
	assert(PipelineManager::GetInstance()->linePSO_.vertexShaderBlob_ != nullptr);

	//PS
	PipelineManager::GetInstance()->linePSO_.pixelShaderBlob_ = CompileShaderManager::GetInstance()->CompileShader(L"Resources/Shader/Line3D/Line3D.PS.hlsl", L"ps_6_0");
	assert(PipelineManager::GetInstance()->linePSO_.pixelShaderBlob_ != nullptr);

	////PSO生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	graphicsPipelineStateDesc.pRootSignature = PipelineManager::GetInstance()->linePSO_.rootSignature_.Get();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = { PipelineManager::GetInstance()->linePSO_.vertexShaderBlob_->GetBufferPointer(),PipelineManager::GetInstance()->linePSO_.vertexShaderBlob_->GetBufferSize() };
	//vertexShaderBlob_->GetBufferSize();
	graphicsPipelineStateDesc.PS = { PipelineManager::GetInstance()->linePSO_.pixelShaderBlob_->GetBufferPointer(),PipelineManager::GetInstance()->linePSO_.pixelShaderBlob_->GetBufferSize() };
	//pixelShaderBlob_->GetBufferSize();
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;


	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//利用するトポロジ(形状)のタイプライン
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;

	//どのように画面に色を打ち込むのか設定
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	//書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//比較関数はLessEqual 近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	//DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//実際に生成
	hr_ = DirectXCommon::GetInstance()->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&PipelineManager::GetInstance()->linePSO_.graphicsPipelineState_));
	assert(SUCCEEDED(hr_));
}

//スプライト用
void PipelineManager::GenerateSpritePSO() 
{	
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};
	descriptionRootSignature_.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//VSでもCBufferを利用することになったので設定を追加
	D3D12_ROOT_PARAMETER rootParameters[4] = {};
	//CBVを使う
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	////PixelShaderで使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号とバインド
	//register...Shader上のResource配置情報
	rootParameters[0].Descriptor.ShaderRegister = 0;

	//CBVを使う
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//VertwxShaderで使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	//register...Shader上のResource配置情報
	rootParameters[1].Descriptor.ShaderRegister = 0;
	//ルートパラメータ配列へのポイント
	descriptionRootSignature_.pParameters = rootParameters;
	//配列の長さ
	descriptionRootSignature_.NumParameters = _countof(rootParameters);

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	//0から始まる
	descriptorRange[0].BaseShaderRegister = 0;	
	//数は1つ
	descriptorRange[0].NumDescriptors = 1;	
	//SRVを使う
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;	
	 //Offsetを自動計算
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; 

	//DescriptorTableを使う
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	
	//PixelShaderを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;				
	//Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	//Tableで利用する数
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	//CBVを使う
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//PixelShaderで使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号1を使う
	rootParameters[3].Descriptor.ShaderRegister = 1;
	
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	//バイリニアフィルタ
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	//0~1の範囲外をリピート
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//比較しない
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	//ありったけのMipmapを使う
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	//レジスタ番号0を使う
	staticSamplers[0].ShaderRegister = 0;
	//PixelShaderで使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	descriptionRootSignature_.pStaticSamplers = staticSamplers;
	descriptionRootSignature_.NumStaticSamplers = _countof(staticSamplers);

	//シリアライズしてバイナリにする
	HRESULT hr_ = {};
	hr_ = D3D12SerializeRootSignature(&descriptionRootSignature_,
		D3D_ROOT_SIGNATURE_VERSION_1, &PipelineManager::GetInstance()->spritePSO_.signatureBlob_, &PipelineManager::GetInstance()->spritePSO_.errorBlob_);
	
	if (FAILED(hr_)) 
	{
		Log(reinterpret_cast<char*>(PipelineManager::GetInstance()->spritePSO_.errorBlob_->GetBufferPointer()));

		assert(false);
	}

	//バイナリを元に生成
	hr_ = DirectXCommon::GetInstance()->GetDevice()->CreateRootSignature(0, PipelineManager::GetInstance()->spritePSO_.signatureBlob_->GetBufferPointer(),
		PipelineManager::GetInstance()->spritePSO_.signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&PipelineManager::GetInstance()->spritePSO_.rootSignature_));
	
	assert(SUCCEEDED(hr_));

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	D3D12_BLEND_DESC blendDesc{};
	//全ての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask =D3D12_COLOR_WRITE_ENABLE_ALL;
	//透明度これはどのブレンドモードでも同じ
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	switch (PipelineManager::GetInstance()->selectSpriteBlendMode_) {
	case BlendModeNone:
		blendDesc.RenderTarget[0].BlendEnable = false;
		break;

	case BlendModeNormal:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend=D3D12_BLEND_INV_SRC_ALPHA;
		break;

	case BlendModeAdd: 
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend=D3D12_BLEND_ONE;
		break;

	case BlendModeSubtract:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blendDesc.RenderTarget[0].DestBlend=D3D12_BLEND_ONE;
		break;

	case BlendModeMultiply: 
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend=D3D12_BLEND_SRC_COLOR;
		break;

	case BlendModeScreen:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend=D3D12_BLEND_ONE;
		break;
	}
	
	//RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	//ShaderをCompileする
	PipelineManager::GetInstance()->spritePSO_.vertexShaderBlob_ = CompileShaderManager::GetInstance()->CompileShader(L"Resources/Shader/Object2D/Object2d.VS.hlsl", L"vs_6_0");
	
	assert(PipelineManager::GetInstance()->spritePSO_.vertexShaderBlob_ != nullptr);

	PipelineManager::GetInstance()->spritePSO_.pixelShaderBlob_ = CompileShaderManager::GetInstance()->CompileShader(L"Resources/Shader/Object2D/Object2d.PS.hlsl", L"ps_6_0");
	
	assert(PipelineManager::GetInstance()->spritePSO_.pixelShaderBlob_ != nullptr);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	
	graphicsPipelineStateDesc.pRootSignature = PipelineManager::GetInstance()->spritePSO_.rootSignature_.Get();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = { PipelineManager::GetInstance()->spritePSO_.vertexShaderBlob_->GetBufferPointer(),PipelineManager::GetInstance()->spritePSO_.vertexShaderBlob_->GetBufferSize() };
	
	//vertexShaderBlob_->GetBufferSize();
	graphicsPipelineStateDesc.PS = { PipelineManager::GetInstance()->spritePSO_.pixelShaderBlob_->GetBufferPointer(),PipelineManager::GetInstance()->spritePSO_.pixelShaderBlob_->GetBufferSize() };
	
	//pixelShaderBlob_->GetBufferSize();
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;

	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//利用するトポロジ(形状)のタイプ三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//どのように画面に色を打ち込むのか設定
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	//書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//比較関数はLessEqual 近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	//DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//実際に生成
	//ID3D12PipelineState* graphicsPipelineState_ = nullptr;
	hr_ = DirectXCommon::GetInstance()->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&PipelineManager::GetInstance()->spritePSO_.graphicsPipelineState_));
	
	assert(SUCCEEDED(hr_));
}

//モデル用
void PipelineManager::GenerateModelPSO(int32_t isSkinning) 
{
	isSkinning;
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};
	descriptionRootSignature_.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//VSでもCBufferを利用することになったので設定を追加
	D3D12_ROOT_PARAMETER rootParameters[8] = {};
	//CBVを使う
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	////PixelShaderで使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号とバインド
	//register...Shader上のResource配置情報
	rootParameters[0].Descriptor.ShaderRegister = 0;

	//CBVを使う
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//VertwxShaderで使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	//register...Shader上のResource配置情報
	rootParameters[1].Descriptor.ShaderRegister = 0;

	//ルートパラメータ配列へのポイント
	descriptionRootSignature_.pParameters = rootParameters;
	//配列の長さ
	descriptionRootSignature_.NumParameters = _countof(rootParameters);

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	//0から始まる
	descriptorRange[0].BaseShaderRegister = 0;
	//数は1つ
	descriptorRange[0].NumDescriptors = 1;
	//SRVを使う
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//Offsetを自動計算
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//DescriptorTableを使う
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//PixelShaderを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	//Tableで利用する数
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	//CBVを使う
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//PixelShaderで使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号1を使う
	rootParameters[3].Descriptor.ShaderRegister = 1;

	//CBVを使う
	//カメラ用
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//VertwxShaderで使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	//register...Shader上のResource配置情報
	rootParameters[4].Descriptor.ShaderRegister = 1;

	//PixelShaderに送る方のカメラ
	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//PixelShaderで使う
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号2を使う
	rootParameters[5].Descriptor.ShaderRegister = 2;

	//PointLight
	rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//PixelShaderで使う
	rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号2を使う
	rootParameters[6].Descriptor.ShaderRegister = 3;

	//SpotLight
	rootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//PixelShaderで使う
	rootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号2を使う
	rootParameters[7].Descriptor.ShaderRegister = 4;

	//ルートパラメータ配列へのポイント
	descriptionRootSignature_.pParameters = rootParameters;
	//配列の長さ
	descriptionRootSignature_.NumParameters = _countof(rootParameters);

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	//バイリニアフィルタ
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	//0~1の範囲外をリピート
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//比較しない
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	//ありったけのMipmapを使う
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	//レジスタ番号0を使う
	staticSamplers[0].ShaderRegister = 0;
	//PixelShaderで使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	descriptionRootSignature_.pStaticSamplers = staticSamplers;
	descriptionRootSignature_.NumStaticSamplers = _countof(staticSamplers);

	//シリアライズしてバイナリにする
	HRESULT hr = {};
	hr = D3D12SerializeRootSignature(&descriptionRootSignature_,
		D3D_ROOT_SIGNATURE_VERSION_1, &PipelineManager::GetInstance()->modelPSO_.signatureBlob_, &PipelineManager::GetInstance()->modelPSO_.errorBlob_);

	if (FAILED(hr)) 
	{
		Log(reinterpret_cast<char*>(PipelineManager::GetInstance()->modelPSO_.errorBlob_->GetBufferPointer()));
		
		assert(false);
	}

	//バイナリを元に生成
	hr = DirectXCommon::GetInstance()->GetDevice()->CreateRootSignature(0, PipelineManager::GetInstance()->modelPSO_.signatureBlob_->GetBufferPointer(),
		PipelineManager::GetInstance()->modelPSO_.signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&PipelineManager::GetInstance()->modelPSO_.rootSignature_));
	
	assert(SUCCEEDED(hr));

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	D3D12_BLEND_DESC blendDesc{};
	//全ての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//α
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	//ブレンドモードの選択
	switch (PipelineManager::GetInstance()->selectModelBlendMode_) 
	{
	case BlendModeNone:
		blendDesc.RenderTarget[0].BlendEnable = false;
		break;

	case BlendModeNormal:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;

	case BlendModeAdd:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;

	case BlendModeSubtract:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;

	case BlendModeMultiply:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
		break;

	case BlendModeScreen:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;
	}

	//RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	//ShaderをCompileする
	PipelineManager::GetInstance()->modelPSO_.vertexShaderBlob_ = CompileShaderManager::GetInstance()->CompileShader(L"Resources/Shader/Object3D/Object3d.VS.hlsl", L"vs_6_0");
	
	assert(PipelineManager::GetInstance()->modelPSO_.vertexShaderBlob_ != nullptr);

	PipelineManager::GetInstance()->modelPSO_.pixelShaderBlob_ = CompileShaderManager::GetInstance()->CompileShader(L"Resources/Shader/Object3D/Object3d.PS.hlsl", L"ps_6_0");
	
	assert(PipelineManager::GetInstance()->modelPSO_.pixelShaderBlob_ != nullptr);

	////PSO生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = PipelineManager::GetInstance()->modelPSO_.rootSignature_.Get();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = { PipelineManager::GetInstance()->modelPSO_.vertexShaderBlob_->GetBufferPointer(),PipelineManager::GetInstance()->modelPSO_.vertexShaderBlob_->GetBufferSize() };
	//vertexShaderBlob_->GetBufferSize();
	graphicsPipelineStateDesc.PS = { PipelineManager::GetInstance()->modelPSO_.pixelShaderBlob_->GetBufferPointer(),PipelineManager::GetInstance()->modelPSO_.pixelShaderBlob_->GetBufferSize() };
	//pixelShaderBlob_->GetBufferSize();
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;

	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//利用するトポロジ(形状)のタイプ三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//どのように画面に色を打ち込むのか設定
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	//書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//比較関数はLessEqual 近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	//DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//実際に生成
	hr = DirectXCommon::GetInstance()->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&PipelineManager::GetInstance()->modelPSO_.graphicsPipelineState_));
	assert(SUCCEEDED(hr));
}

void PipelineManager::GenerateAnimationModelPSO()
{
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};
	descriptionRootSignature_.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//VSでもCBufferを利用することになったので設定を追加
	D3D12_ROOT_PARAMETER rootParameters[9] = {};
	//CBVを使う
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	////PixelShaderで使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号とバインド
	//register...Shader上のResource配置情報
	rootParameters[0].Descriptor.ShaderRegister = 0;

	//CBVを使う
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//VertwxShaderで使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	//register...Shader上のResource配置情報
	rootParameters[1].Descriptor.ShaderRegister = 0;

	//ルートパラメータ配列へのポイント
	descriptionRootSignature_.pParameters = rootParameters;
	//配列の長さ
	descriptionRootSignature_.NumParameters = _countof(rootParameters);

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	//0から始まる
	descriptorRange[0].BaseShaderRegister = 0;
	//数は1つ
	descriptorRange[0].NumDescriptors = 1;
	//SRVを使う
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//Offsetを自動計算
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//DescriptorTableを使う
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//PixelShaderを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	//Tableで利用する数
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	//CBVを使う
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//PixelShaderで使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号1を使う
	rootParameters[3].Descriptor.ShaderRegister = 1;

	//CBVを使う
	//カメラ用
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//VertwxShaderで使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	//register...Shader上のResource配置情報
	rootParameters[4].Descriptor.ShaderRegister = 1;

	//PixelShaderに送る方のカメラ
	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//PixelShaderで使う
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号2を使う
	rootParameters[5].Descriptor.ShaderRegister = 2;

	//PointLight
	rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//PixelShaderで使う
	rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号2を使う
	rootParameters[6].Descriptor.ShaderRegister = 3;

	//SpotLight
	rootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//PixelShaderで使う
	rootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号2を使う
	rootParameters[7].Descriptor.ShaderRegister = 4;

	D3D12_DESCRIPTOR_RANGE descriptorRangeForWell[1] = {};
	//0から始まる
	descriptorRangeForWell[0].BaseShaderRegister = 0;
	//数は一つ
	descriptorRangeForWell[0].NumDescriptors = 1;
	//SRVを使う
	descriptorRangeForWell[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeForWell[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParameters[8].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//VertwxShaderで使う
	rootParameters[8].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	//register...Shader上のResource配置情報(t0)
	rootParameters[8].Descriptor.ShaderRegister = 0;
	//Tableの中身の配列を指定
	rootParameters[8].DescriptorTable.pDescriptorRanges = descriptorRangeForWell;
	//Tableで利用する数
	rootParameters[8].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForWell);

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	//バイリニアフィルタ
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	//0~1の範囲外をリピート
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//比較しない
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	//ありったけのMipmapを使う
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	//レジスタ番号0を使う
	staticSamplers[0].ShaderRegister = 0;
	//PixelShaderで使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	descriptionRootSignature_.pStaticSamplers = staticSamplers;
	descriptionRootSignature_.NumStaticSamplers = _countof(staticSamplers);

	//シリアライズしてバイナリにする
	HRESULT hr = {};
	hr = D3D12SerializeRootSignature(&descriptionRootSignature_,
		D3D_ROOT_SIGNATURE_VERSION_1, &PipelineManager::GetInstance()->animationModelPSO_.signatureBlob_, &PipelineManager::GetInstance()->animationModelPSO_.errorBlob_);
	
	if (FAILED(hr)) 
	{
		Log(reinterpret_cast<char*>(PipelineManager::GetInstance()->animationModelPSO_.errorBlob_->GetBufferPointer()));
		
		assert(false);
	}

	//バイナリを元に生成
	hr = DirectXCommon::GetInstance()->GetDevice()->CreateRootSignature(0, PipelineManager::GetInstance()->animationModelPSO_.signatureBlob_->GetBufferPointer(),
		PipelineManager::GetInstance()->animationModelPSO_.signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&PipelineManager::GetInstance()->animationModelPSO_.rootSignature_));
	
	assert(SUCCEEDED(hr));

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[5] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[3].SemanticName = "WEIGHT";
	inputElementDescs[3].SemanticIndex = 0;
	inputElementDescs[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;	
	inputElementDescs[3].InputSlot = 1;							
	inputElementDescs[3].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[4].SemanticName = "INDEX";
	inputElementDescs[4].SemanticIndex = 0;
	inputElementDescs[4].Format = DXGI_FORMAT_R32G32B32A32_SINT;	
	inputElementDescs[4].InputSlot = 1;								
	inputElementDescs[4].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	D3D12_BLEND_DESC blendDesc{};
	//全ての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//α
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	switch (PipelineManager::GetInstance()->selectAnimiationModelBlendMode_)
	{
	case BlendModeNone:
		blendDesc.RenderTarget[0].BlendEnable = false;
		break;

	case BlendModeNormal:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;

	case BlendModeAdd:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;

	case BlendModeSubtract:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;

	case BlendModeMultiply:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
		break;

	case BlendModeScreen:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;
	}

	//RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	//ShaderをCompileする
	PipelineManager::GetInstance()->animationModelPSO_.vertexShaderBlob_ = CompileShaderManager::GetInstance()->CompileShader(L"Resources/Shader/AnimationObject3D/AnimationObject3D.VS.hlsl", L"vs_6_0");
	
	assert(PipelineManager::GetInstance()->animationModelPSO_.vertexShaderBlob_ != nullptr);

	PipelineManager::GetInstance()->animationModelPSO_.pixelShaderBlob_ = CompileShaderManager::GetInstance()->CompileShader(L"Resources/Shader/AnimationObject3D/AnimationObject3D.PS.hlsl", L"ps_6_0");
	
	assert(PipelineManager::GetInstance()->animationModelPSO_.pixelShaderBlob_ != nullptr);

	////PSO生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = PipelineManager::GetInstance()->animationModelPSO_.rootSignature_.Get();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = { PipelineManager::GetInstance()->animationModelPSO_.vertexShaderBlob_->GetBufferPointer(),PipelineManager::GetInstance()->animationModelPSO_.vertexShaderBlob_->GetBufferSize() };
	//vertexShaderBlob_->GetBufferSize();
	graphicsPipelineStateDesc.PS = { PipelineManager::GetInstance()->animationModelPSO_.pixelShaderBlob_->GetBufferPointer(),PipelineManager::GetInstance()->animationModelPSO_.pixelShaderBlob_->GetBufferSize() };
	//pixelShaderBlob_->GetBufferSize();
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;

	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//利用するトポロジ(形状)のタイプ三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//どのように画面に色を打ち込むのか設定
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	//書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//比較関数はLessEqual 近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	//DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//実際に生成
	hr = DirectXCommon::GetInstance()->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&PipelineManager::GetInstance()->animationModelPSO_.graphicsPipelineState_));
	
	assert(SUCCEEDED(hr));
}

void PipelineManager::GenerateParticle3DPSO() 
{
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};
	descriptionRootSignature_.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	
	//VSでもCBufferを利用することになったので設定を追加
	D3D12_ROOT_PARAMETER rootParameters[5] = {};
	//CBVを使う
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	////PixelShaderで使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号とバインド
	//register...Shader上のResource配置情報
	rootParameters[0].Descriptor.ShaderRegister = 0;

	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	//0から始まる
	descriptorRangeForInstancing[0].BaseShaderRegister = 0;
	//数は一つ
	descriptorRangeForInstancing[0].NumDescriptors = 1;
	//SRVを使う
	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//今回はDescriptorTableを使う
	//Instancing
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//VertwxShaderで使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	//register...Shader上のResource配置情報
	rootParameters[1].Descriptor.ShaderRegister = 0;
	//Tableの中身の配列を指定
	rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;
	//Tableで利用する数
	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);
	
	//ルートパラメータ配列へのポイント
	descriptionRootSignature_.pParameters = rootParameters;
	//配列の長さ
	descriptionRootSignature_.NumParameters = _countof(rootParameters);

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	//0から始まる
	descriptorRange[0].BaseShaderRegister = 0;	
	//数は1つ
	descriptorRange[0].NumDescriptors = 1;	
	//SRVを使う
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;	
	 //Offsetを自動計算
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; 

	//DescriptorTableを使う
	//Texture
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	
	//PixelShaderを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;				
	//Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	//Tableで利用する数
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	//CBVを使う
	//DirectionalLight
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//PixelShaderで使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号1を使う
	rootParameters[3].Descriptor.ShaderRegister = 1;
	
	//CBVを使う
	//カメラ
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//VertwxShaderで使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	//register...Shader上のResource配置情報
	rootParameters[4].Descriptor.ShaderRegister = 1;

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	//バイリニアフィルタ
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	//0~1の範囲外をリピート
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//比較しない
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	//ありったけのMipmapを使う
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	//レジスタ番号0を使う
	staticSamplers[0].ShaderRegister = 0;
	//PixelShaderで使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	descriptionRootSignature_.pStaticSamplers = staticSamplers;
	descriptionRootSignature_.NumStaticSamplers = _countof(staticSamplers);

	//シリアライズしてバイナリにする
	HRESULT hr = {};
	hr = D3D12SerializeRootSignature(&descriptionRootSignature_,
		D3D_ROOT_SIGNATURE_VERSION_1, &PipelineManager::GetInstance()->particle3DPSO_.signatureBlob_, &PipelineManager::GetInstance()->particle3DPSO_.errorBlob_);
	
	if (FAILED(hr)) 
	{
		Log(reinterpret_cast<char*>(PipelineManager::GetInstance()->particle3DPSO_.errorBlob_->GetBufferPointer()));
		
		assert(false);
	}

	//バイナリを元に生成
	hr = DirectXCommon::GetInstance()->GetDevice()->CreateRootSignature(0,PipelineManager::GetInstance()->particle3DPSO_.signatureBlob_->GetBufferPointer(),
		PipelineManager::GetInstance()->particle3DPSO_.signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&PipelineManager::GetInstance()->particle3DPSO_.rootSignature_));
	
	assert(SUCCEEDED(hr));

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	D3D12_BLEND_DESC blendDesc{};
	//全ての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask =D3D12_COLOR_WRITE_ENABLE_ALL;
	//加算ブレンド
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend=D3D12_BLEND_ONE;
		
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	
	//RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//今回はカリングをオフにする
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	//ShaderをCompileする
	PipelineManager::GetInstance()->particle3DPSO_.vertexShaderBlob_ = CompileShaderManager::GetInstance()->CompileShader(L"Resources/Shader/Particle3D/Particle3d.VS.hlsl", L"vs_6_0");
	
	assert(PipelineManager::GetInstance()->particle3DPSO_.vertexShaderBlob_ != nullptr);

	PipelineManager::GetInstance()->particle3DPSO_.pixelShaderBlob_ = CompileShaderManager::GetInstance()->CompileShader(L"Resources/Shader/Particle3D/Particle3d.PS.hlsl", L"ps_6_0");
	
	assert(PipelineManager::GetInstance()->particle3DPSO_.pixelShaderBlob_ != nullptr);

	////PSO生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = PipelineManager::GetInstance()->particle3DPSO_.rootSignature_.Get();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = { PipelineManager::GetInstance()->particle3DPSO_.vertexShaderBlob_->GetBufferPointer(),PipelineManager::GetInstance()->particle3DPSO_.vertexShaderBlob_->GetBufferSize() };
	//vertexShaderBlob_->GetBufferSize();
	graphicsPipelineStateDesc.PS = { PipelineManager::GetInstance()->particle3DPSO_.pixelShaderBlob_->GetBufferPointer(),PipelineManager::GetInstance()->particle3DPSO_.pixelShaderBlob_->GetBufferSize() };
	//pixelShaderBlob_->GetBufferSize();
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;

	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//利用するトポロジ(形状)のタイプ三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//どのように画面に色を打ち込むのか設定
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	//書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	//比較関数はLessEqual 近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	//DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//実際に生成
	hr = DirectXCommon::GetInstance()->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&PipelineManager::GetInstance()->particle3DPSO_.graphicsPipelineState_));
	
	assert(SUCCEEDED(hr));
}