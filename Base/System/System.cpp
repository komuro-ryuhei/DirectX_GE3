
#include "System.h"

// MyClass
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Base/Mesh/Mesh.h"
#include "Base/Sprite/Sprite.h"
#include "Base/Object3d/Object3d.h"
#include "Base/Triangle/Triangle.h"
#include "Base/WinApp/WinApp.h"
#include "lib/Input/Input.h"
#include "lib/Logger/Logger.h"

#include "Base/PSO/PipelineManager/PipelineManager.h"
#include "Base/TextureManager/TextureManager.h"
#include "Base/ModelManager/ModelManager.h"

#include <cassert>
#include <cstdint>
#include <format>
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
// Triangle
std::unique_ptr<Triangle> triangle_ = nullptr;
// SPrite
// std::unique_ptr<Sprite> sprite_ = nullptr;
std::vector<std::unique_ptr<Sprite>> sprites_;
// Model
std::unique_ptr<Object3d> object3d_ = nullptr;
// ModelManager
// std::unique_ptr<ModelManager> modelManager_ = nullptr;

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

	// TextureManager
	TextureManager::GetInstance()->Init();

	// Sprite
	/*sprite_ = std::make_unique<Sprite>();
	sprite_->Init(dxCommon_.get(),pipelineManager_.get());*/

	// テクスチャの読み込み
	const std::string& uvTexture = "./Resources/images/uvChecker.png";
	TextureManager::GetInstance()->LoadTexture(dxCommon_.get(), uvTexture);
	/*const std::string& monsterBallTexture = "./Resources/images/monsterBall.png";
	TextureManager::GetInstance()->LoadTexture(dxCommon_.get(), monsterBallTexture);*/


	for (uint32_t i = 0; i < 1; ++i) {
		if (i % 2 == 0) {
			auto sprite = std::make_unique<Sprite>();
			sprite->Init(dxCommon_.get(), pipelineManager_.get(), uvTexture);
			sprite->SetPosition(Vector2{200.0f * i, 0.0f});
			sprites_.push_back(std::move(sprite));
		} else {
			auto sprite = std::make_unique<Sprite>();
			sprite->Init(dxCommon_.get(), pipelineManager_.get(), uvTexture);
			sprite->SetPosition(Vector2{200.0f * i, 0.0f});
			sprites_.push_back(std::move(sprite));
		}
	}

	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(dxCommon_.get());

	ModelManager::GetInstance()->Init(dxCommon_.get());

	ModelManager::GetInstance()->LoadModel("plane.obj");
	object3d_->SetModel("plane.obj");

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

	// object3d_->PreDraw();
}

void System::Update() {

	input_->Update();

	// sprite_->Update();

	for (auto& sprite : sprites_) {
		sprite->Update();

		/*Vector2 position = sprite->GetPosition();
		position += Vector2{0.1f, 0.1f};
		sprite->SetPosition(position);*/

		/*Vector4 color = sprite->GetColor();
		color.x += 0.01f;
		if (color.x > 1.0f) {
		    color.x -= 1.0f;
		}
		sprite->SetColor(color);*/

		Vector2 size = sprite->GetSize();
		sprite->SetSize(size);
		/*size.x -= 0.5f;
		size.y -= 0.5f;
		sprite->SetSize(size);*/

		/*Vector2 size = sprite->GetSize();
		size.x += 0.3f;
		size.y += 0.3f;
		sprite->SetSize(size);*/

		/*float rotation = sprite->GetRotation();
		rotation += 0.01f;
		sprite->SetRotation(rotation);*/

		/*Vector2 anchor = {0.5f, 0.5f};
		sprite->SetAnchorPoint(anchor);*/

		/*sprite->SetIsFlipY(true);*/
	}

	object3d_->Update();
}

void System::EndFrame() {

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
	triangle_.reset();
	for (auto& sprite : sprites_) {
		sprite.reset();
	}

	//
	TextureManager::GetInstance()->Finalize();
	ModelManager::GetInstance()->Finalize();
}

void System::DrawTriangle() { triangle_->Draw(); }

void System::DrawSprite() {
	// sprite_->Draw();
	for (auto& sprite : sprites_) {
		sprite->Draw();
	}
}

void System::DrawObj() {
	// 
	object3d_->Draw();
}