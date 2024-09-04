#pragma once

#include <string>

#include <dxgidebug.h>
#include "ConvertLog.h"
#include <cassert>

#include <dxcapi.h>

#pragma comment(lib,"dxcompiler.lib")

class CompileShaderManager 
{
private:

	CompileShaderManager();

	~CompileShaderManager();

public:
	
	static CompileShaderManager* GetInstance();

	CompileShaderManager(const CompileShaderManager& compileShaderManager) = delete;

	CompileShaderManager& operator=(const CompileShaderManager& compileShaderManager) = delete;

private:

	void InitializeDXC();

public:

	IDxcBlob* CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile
		);

	void Release();

private:
	//DXC
	IDxcUtils* dxcUtils_ = nullptr;
	IDxcCompiler3* dxcCompiler_ = nullptr;
	IDxcIncludeHandler* includeHandler_ = nullptr;	
};