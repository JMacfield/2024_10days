#pragma once

class ImGuiManager final
{
private:

	ImGuiManager();

	~ImGuiManager();

public:
	
	ImGuiManager(const ImGuiManager& imGui) = delete;

	ImGuiManager& operator=(const ImGuiManager& imGui) = delete;

	static ImGuiManager* GetInstance();

public:

	void Initialize();

	void BeginFrame();

	void Update();


	void PreDraw();

	void Draw();

	void EndFrame();

	void Release();

private:

};