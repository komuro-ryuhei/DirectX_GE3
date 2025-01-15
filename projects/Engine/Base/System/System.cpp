
#include "System.h"

// MyClass
#include "Engine/Base/2d/Sprite/Sprite.h"
#include "Engine/Base/3d/Object3d/Object3d.h"
#include "Engine/Base/Audio/Audio.h"
#include "Engine/Base/Camera/Camera.h"
#include "Engine/Base/DirectXCommon/DirectXCommon.h"
#include "Engine/Base/Mesh/Mesh.h"
#include "Engine/Base/WinApp/WinApp.h"
#include "Engine/lib/Input/Input.h"
#include "Engine/lib/Logger/Logger.h"
#include "Game/Objects/Enemy/Enemy.h"
#include "Game/Objects/Player/Player.h"

#include "Engine/Base/ImGuiManager/ImGuiManager.h"
#include "Engine/Base/ModelManager/ModelManager.h"
#include "Engine/Base/PSO/PipelineManager/PipelineManager.h"
#include "Engine/Base/SrvManager/SrvManager.h"
#include "Engine/Base/TextureManager/TextureManager.h"

#include "imgui/imgui.h"

#include <cassert>
#include <cstdint>
#include <format>
#include <random>
#include <string>
#include <vector>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

/*==================================================================================*/
// システム全体の初期化

// WindowsAPI
std::unique_ptr<WinApp> winApp_ = nullptr;
// DirectXCommon
std::unique_ptr<DirectXCommon> dxCommon_ = nullptr;
// Pipeline
std::unique_ptr<PipelineManager> pipelineManager_ = nullptr;
// Input
std::unique_ptr<Input> input_ = nullptr;
// Mesh
std::unique_ptr<Mesh> mesh_ = nullptr;
// SrvManager
std::unique_ptr<SrvManager> srvManager_ = nullptr;
// ImGuiManager
std::unique_ptr<ImGuiManager> imguiManager_ = nullptr;

/// <summary>
/// getter
/// </summary>
DirectXCommon* System::GetDXCommon() { return dxCommon_.get(); }
PipelineManager* System::GetPipelineManager() { return pipelineManager_.get(); }
WinApp* System::GetWinApp() { return winApp_.get(); }
// std::vector<std::unique_ptr<Enemy>>& System::GetEnemies() { return enemies_; }

void System::Initialize(const char* title, int width, int height) {

	winApp_ = std::make_unique<WinApp>();

	// ゲームウインドウの作成
	std::string windowTitle = std::string(title);
	auto&& titleString = StringUtility::ConvertString(windowTitle);
	winApp_->CreateGameWindow(titleString.c_str(), WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME), width, height);

	// DirectXの初期化処理
	dxCommon_ = std::make_unique<DirectXCommon>();
	dxCommon_->Initialize(winApp_.get());

	// pipelineの初期化
	pipelineManager_ = std::make_unique<PipelineManager>();
	pipelineManager_->PSOSetting(dxCommon_.get());

	// SrvManager
	srvManager_ = std::make_unique<SrvManager>();
	srvManager_->Init(dxCommon_.get());

	imguiManager_ = std::make_unique<ImGuiManager>();
	imguiManager_->Init(winApp_.get(), dxCommon_.get());

	// TextureManager
	TextureManager::GetInstance()->Init(srvManager_.get());
}

bool System::ProcessMessage() { return winApp_->ProcessMessage(); }

void System::BeginFrame() {

	// DirectX描画前処理
	dxCommon_->PreDraw();

	srvManager_->PreDraw();

	// Sprite描画前処理
	// sprite_->PreDraw();

	// object3d_->PreDraw();
}

void System::Update() { imguiManager_->Begin(); }

void System::EndFrame() {

	imguiManager_->End();

	imguiManager_->Draw();

	// DirectX描画終了
	dxCommon_->PostDraw();
}

void System::Finalize() {

	winApp_->TerminateGameWindow();

	winApp_.reset();
	dxCommon_.reset();
	pipelineManager_.reset();
	input_.reset();
	mesh_.reset();

	//
	TextureManager::GetInstance()->Finalize();
	ModelManager::GetInstance()->Finalize();

	imguiManager_->Finalize();
}