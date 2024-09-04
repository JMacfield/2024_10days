#pragma once

#include <d3d12.h>
#include <cassert>

#include <CompileShaderManager.h>
#include "ConvertLog.h"
#include "DirectXCommon.h"

enum BlemdMode 
{
	//ブレンド無し
	BlendModeNone,

	//通常ブレンド
	//Src*SrcA+Dest*(1-SrcA)
	BlendModeNormal,

	//加算ブレンド
	//Src*SrcA+Dest*1
	BlendModeAdd,

	//減算ブレンド
	//Dest*1-Src*SrcA
	BlendModeSubtract,

	//乗算ブレンド
	//Src*0+Dest*Src
	BlendModeMultiply,

	//スクリーンブレンド
	//Src*(1-Dest)+Dest*1
	BlendModeScreen,
};

class PipelineManager final
{
private:

	PipelineManager()=default;

	~PipelineManager()=default;

public:
	
	static PipelineManager* GetInstance();

	PipelineManager(const PipelineManager& pipelineManager) = delete;

	PipelineManager& operator=(const PipelineManager& pipelineManager) = delete;

#pragma region Getter
	ComPtr<ID3D12RootSignature> GetLineRootSignature() { return linePSO_.rootSignature_; }

	ComPtr<ID3D12PipelineState> GetLineGraphicsPipelineState() { return linePSO_.graphicsPipelineState_; }

	ComPtr<ID3D12RootSignature> GetSpriteRootSignature() { return spritePSO_.rootSignature_; }

	ComPtr<ID3D12PipelineState> GetSpriteGraphicsPipelineState() { return spritePSO_.graphicsPipelineState_; }

	ComPtr<ID3D12RootSignature> GetModelRootSignature() { return modelPSO_.rootSignature_; }

	ComPtr<ID3D12PipelineState> GetModelGraphicsPipelineState() { return modelPSO_.graphicsPipelineState_; }

	ComPtr<ID3D12RootSignature> GetAnimationModelRootSignature() { return animationModelPSO_.rootSignature_; }

	ComPtr<ID3D12PipelineState> GetAnimationModelGraphicsPipelineState() { return animationModelPSO_.graphicsPipelineState_; }

	ComPtr<ID3D12RootSignature> GetParticle3DRootSignature() { return particle3DPSO_.rootSignature_; }

	ComPtr<ID3D12PipelineState> GetParticle3DGraphicsPipelineState() { return particle3DPSO_.graphicsPipelineState_; }

	void SetSpriteBlendMode(uint32_t blendmode) { selectSpriteBlendMode_ = blendmode; }

	void SetModelBlendMode(uint32_t blendmode) { selectModelBlendMode_ = blendmode; }
#pragma endregion

public:

	static void GenaratedLinePSO();

	static void GenerateSpritePSO();

	static void GenerateModelPSO(int32_t isSkinning);

	static void GenerateAnimationModelPSO();

	static void GenerateParticle3DPSO();

private:

	struct PSOInformation 
	{
		ComPtr<ID3DBlob> signatureBlob_ = nullptr;
		ComPtr<ID3DBlob> errorBlob_ = nullptr;
		ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
		ComPtr<IDxcBlob> pixelShaderBlob_ = nullptr;
		ComPtr<IDxcBlob> vertexShaderBlob_ = nullptr;
		ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;
	};

	//ライン用
	PSOInformation linePSO_ = {};
	//スプライト用
	PSOInformation spritePSO_ = {};
	//モデル用の変数
	PSOInformation modelPSO_ = {};
	//モデル用の変数
	PSOInformation particle3DPSO_ = {};
	//モデル用の変数
	PSOInformation animationModelPSO_ = {};

	//スプライト用
	int32_t selectSpriteBlendMode_ = 1;

	//モデル用の
	int32_t selectModelBlendMode_ = 1;

	//モデル用の
	int32_t selectAnimiationModelBlendMode_ = 1;
};