
#include "System.h"

// MyClass
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Base/Mesh/Mesh.h"
#include "Base/PSO/PipelineManager/PipelineManager.h"
#include "Base/Sprite/Sprite.h"
#include "Base/Triangle/Triangle.h"
#include "Base/WinApp/WinApp.h"
#include "lib/Input/Input.h"
#include "lib/Logger/Logger.h"

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
// Pipeline
std::unique_ptr<PipelineManager> pipelineManager_ = nullptr;
// Input
std::unique_ptr<Input> input_ = nullptr;
// Mesh
std::unique_ptr<Mesh> mesh_ = nullptr;
// Triangle
std::unique_ptr<Triangle> triangle_ = nullptr;
// SPrite
// std::unique_ptr<Sprite> sprite_ = nullptr;
std::vector<std::unique_ptr<Sprite>> sprites_;

void System::Initialize(const char* title, int width, int height) {

	winApp_ = std::make_unique<WinApp>();

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

	// Inputの初期化
	input_ = std::make_unique<Input>();
	input_->Initialize(winApp_.get());

	// Triangle
	triangle_ = std::make_unique<Triangle>();
	triangle_->Initialize(dxCommon_.get(), pipelineManager_.get());

	// Sprite
	/*sprite_ = std::make_unique<Sprite>();
	sprite_->Init(dxCommon_.get(),pipelineManager_.get());*/

	for (uint32_t i = 0; i < 5; ++i) {
		auto sprite = std::make_unique<Sprite>();
		sprite->Init(dxCommon_.get(), pipelineManager_.get());
		sprite->SetPosition(Vector2{200.0f * i, 0.0f});
		sprites_.push_back(std::move(sprite));
	}

	// Mesh
	mesh_ = std::make_unique<Mesh>();
	mesh_->LightSetting(dxCommon_.get());
}

bool System::ProcessMessage() { return winApp_->ProcessMessage(); }

void System::BeginFrame() {

	// DirectX描画前処理
	dxCommon_->PreDraw();

	// Sprite描画前処理
	// sprite_->PreDraw();
	// Sprite描画前処理
	for (auto& sprite : sprites_) {
		sprite->PreDraw();
	}
}

void System::Update() {

	input_->Update();

	// sprite_->Update();

	for (auto& sprite : sprites_) {
		sprite->Update();

		/*Vector2 position = sprite->GetPosition();
		position += Vector2{0.1f, 0.1f};
		sprite->SetPosition(position);*/
	}
}

void System::EndFrame() {

	// DirectX描画終了
	dxCommon_->PostDraw();
}

void System::DrawTriangle() { triangle_->Draw(); }

void System::DrawSprite() {
	// sprite_->Draw();
	for (auto& sprite : sprites_) {
		sprite->Draw();
	}
}