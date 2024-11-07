#include "System.h"

// MyClass
#include "DirectXCommon.h"
#include "Input.h"
#include "Mesh.h"
#include "PipelineManager.h"
#include "Triangle.h"
#include "WinApp.h"

#include "Logger.h"

#include <cassert>
#include <cstdint>
#include <format>
#include <string>

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
// TextureManager
std::unique_ptr<TextureManager> textureManager_ = nullptr;
    // Pipeline
std::unique_ptr<PipelineManager> pipelineManager_ = nullptr;
// Input
std::unique_ptr<Input> input_ = nullptr;
// Sprite
std::unique_ptr<Sprite> sprite_ = nullptr;
std::unique_ptr<SpriteCommon> spriteCommon_ = nullptr;
// Triangle
std::unique_ptr<Triangle> triangle_ = nullptr;
// Mesh
std::unique_ptr<Mesh> mesh_ = nullptr;

void System::Initialize(const char* title, int width, int height) {

	winApp_ = std::make_unique<WinApp>();

	mesh_ = std::make_unique<Mesh>();

	// ゲームウインドウの作成
	std::string titleWithVersion = std::string(title);
	auto&& titleString = StringUtility::ConvertString(titleWithVersion);
	winApp_->CreateGameWindow(titleString.c_str(), WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME), width, height);

	// DirectXの初期化処理
	dxCommon_ = std::make_unique<DirectXCommon>();
	dxCommon_->Initialize(winApp_.get());

	// pipelineの初期化
	pipelineManager_ = std::make_unique<PipelineManager>();
	pipelineManager_->PSOSetting(dxCommon_.get());

	// TextureManagerの初期化
	textureManager_ = std::make_unique<TextureManager>();
	textureManager_->Init();

	// Inputの初期化
	input_ = std::make_unique<Input>();
	input_->Initialize(winApp_.get());

	// Spriteの初期化
	spriteCommon_ = std::make_unique<SpriteCommon>();
	spriteCommon_->Initialize(dxCommon_.get());

	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(spriteCommon_.get(), dxCommon_.get(), "./Resources/uvChecker.png");

	triangle_ = std::make_unique<Triangle>();
	triangle_->Initialize(dxCommon_.get(), pipelineManager_.get());
}

bool System::ProcessMessage() { return winApp_->ProcessMessage(); }

void System::BeginFrame() {

	// DirectX描画前処理
	dxCommon_->PreDraw();
	// Sprite描画前処理
	spriteCommon_->PreDraw();

	input_->Update();

	// sprite_->Update();
	// sprite_->Draw();
}

void System::EndFrame() {

	// DirectX描画終了
	dxCommon_->PostDraw();
	// 
	textureManager_->Finalize();
}

void System::CreateMesh() {}

void System::DrawTriangle() { triangle_->Draw(); }