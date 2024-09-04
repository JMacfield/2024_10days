#include "ImGuiManager.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "SrvManager.h"

ImGuiManager::ImGuiManager() 
{
	
	
}

ImGuiManager* ImGuiManager::GetInstance() 
{
	static ImGuiManager instance;

	return &instance;
}

void ImGuiManager::Initialize() 
{

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(WinApp::GetInstance()->GetHwnd());
	ImGui_ImplDX12_Init(
		DirectXCommon::GetInstance()->GetDevice().Get(),
		DirectXCommon::GetInstance()->GetswapChain().swapChainDesc.BufferCount,
		DirectXCommon::GetInstance()->GetRtvDesc().Format,
		SrvManager::GetInstance()->GetSrvDescriptorHeap().Get(),
		SrvManager::GetInstance()->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		SrvManager::GetInstance()->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
}

void ImGuiManager::BeginFrame() 
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::Update() 
{

}

void ImGuiManager::PreDraw() 
{
	ImGui::Render();
}

void ImGuiManager::Draw() 
{
	ID3D12DescriptorHeap* descriptorHeaps[] = { SrvManager::GetInstance()->GetSrvDescriptorHeap().Get() };
	DirectXCommon::GetInstance()->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
}

void ImGuiManager::EndFrame() 
{
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectXCommon::GetInstance()->GetCommandList().Get());
}

void ImGuiManager::Release() 
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

ImGuiManager::~ImGuiManager() 
{

}