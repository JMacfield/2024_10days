#pragma once

#include <d3d12.h>
#include <string>
#include <array>
#include <DirectXTex.h>
#include <d3dx12.h>

#include "DirectXCommon.h"
#include "ConvertLog.h"

class TextureManager 
{
private:
	
	TextureManager();

	
	~TextureManager();

public:

	static TextureManager* GetInstance();

	TextureManager(const TextureManager& textureManager) = delete;

	TextureManager& operator=(const TextureManager& textureManager) = delete;

public:
	
	static void Initialize();

	static uint32_t LoadTexture(const std::string& filePath);

	static void GraphicsCommand(uint32_t texHandle);

	void Release();

	const D3D12_RESOURCE_DESC GetResourceDesc(uint32_t textureHandle);

private:

#pragma region テクスチャ
	//1.TextureデータそのものをCPUで読み込む
	static DirectX::ScratchImage LoadTextureData(const std::string& filePath);

	//2.DirectX12のTextureResourceを作る
	static ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);

	//3.TextureResourceに1で読んだデータを転送する
	static void UploadTextureData(ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages);
#pragma endregion
	
public:

	static const int TEXTURE_MAX_AMOUNT_ = 128;

	struct TextureInformation 
	{
		ComPtr<ID3D12Resource> resource_= nullptr;

		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU_ = {};
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU_ = {};

		std::string name_={};

		uint32_t handle_ = 0;
	};

private:

	ComPtr<ID3D12Resource> textureResource_[TEXTURE_MAX_AMOUNT_] = {nullptr};

	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_[TEXTURE_MAX_AMOUNT_] = {};
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_[TEXTURE_MAX_AMOUNT_] = {};

	std::array<TextureInformation, TEXTURE_MAX_AMOUNT_> textureInformation_{};
};