
#include "System.h"

// MyClass
#include "Engine/Base/2d/Sprite/Sprite.h"
#include "Engine/Base/3d/Object3d/Object3d.h"
#include "Engine/Base/3d/Triangle/Triangle.h"
#include "Engine/Base/Audio/Audio.h"
#include "Engine/Base/Camera/Camera.h"
#include "Engine/Base/DirectXCommon/DirectXCommon.h"
#include "Engine/Base/Mesh/Mesh.h"
#include "Engine/Base/WinApp/WinApp.h"
#include "Engine/lib/Input/Input.h"
#include "Engine/lib/Logger/Logger.h"

#include "Engine/Base/ImGuiManager/ImGuiManager.h"
#include "Engine/Base/ModelManager/ModelManager.h"
#include "Engine/Base/PSO/PipelineManager/PipelineManager.h"
#include "Engine/Base/SrvManager/SrvManager.h"
#include "Engine/Base/TextureManager/TextureManager.h"

#include "imgui/imgui.h"

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
// SrvManager
std::unique_ptr<SrvManager> srvManager_ = nullptr;
// ImGuiManager
std::unique_ptr<ImGuiManager> imguiManager_ = nullptr;

/*==================================================================================*/
// ゲームシーンの初期化

// Camera
std::unique_ptr<Camera> camera_ = nullptr;
// Audio
std::unique_ptr<Audio> audio_ = nullptr;
// Sprite
std::unique_ptr<Sprite> sprite_ = nullptr;
// Model
std::unique_ptr<Object3d> object3d_ = nullptr;

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

	// SrvManager
	srvManager_ = std::make_unique<SrvManager>();
	srvManager_->Init(dxCommon_.get());

	// Inputの初期化
	input_ = std::make_unique<Input>();
	input_->Initialize(winApp_.get());

	// TextureManager
	TextureManager::GetInstance()->Init(srvManager_.get());

	// テクスチャの読み込み
	const std::string& uvTexture = "./Resources/images/uvChecker.png";
	TextureManager::GetInstance()->LoadTexture(dxCommon_.get(), uvTexture);
	const std::string& monsterBallTexture = "./Resources/images/monsterBall.png";
	TextureManager::GetInstance()->LoadTexture(dxCommon_.get(), monsterBallTexture);

	// Sprite
	sprite_ = std::make_unique<Sprite>();
	sprite_->Init(dxCommon_.get(), pipelineManager_.get(), uvTexture);

	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(dxCommon_.get());

	ModelManager::GetInstance()->Init(dxCommon_.get());

	ModelManager::GetInstance()->LoadModel("plane.obj");
	object3d_->SetModel("plane.obj");

	// Mesh
	mesh_ = std::make_unique<Mesh>();
	mesh_->LightSetting(dxCommon_.get());

	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({0.0f, 0.0f, 0.0f});
	camera_->SetTranslate({0.0f, 0.0f, -10.0f});
	object3d_->SetDefaultCamera(camera_.get());

	imguiManager_ = std::make_unique<ImGuiManager>();
	imguiManager_->Init(winApp_.get(), dxCommon_.get());

	audio_ = std::make_unique<Audio>();
	audio_->Init();

	SoundData soundData = audio_->SoundLoadWave("Resources/fanfare.wav");
	// audio_->SoundPlayWave(audio_->GetXAudio2(), soundData);
}

bool System::ProcessMessage() { return winApp_->ProcessMessage(); }

void System::BeginFrame() {

	// DirectX描画前処理
	dxCommon_->PreDraw();

	srvManager_->PreDraw();
	imguiManager_->Begin();

	// Sprite描画前処理
	sprite_->PreDraw();
}

void System::Update() {

	input_->Update();

	camera_->Update();

	sprite_->Update();

	object3d_->Update();

	sprite_->ImGuiDebug();
}

void System::Draw() {

	sprite_->Draw();
	object3d_->Draw();
}

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
	sprite_.reset();

	//
	TextureManager::GetInstance()->Finalize();
	ModelManager::GetInstance()->Finalize();

	imguiManager_->Finalize();
}