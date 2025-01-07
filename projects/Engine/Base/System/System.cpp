
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

/*==================================================================================*/
// ゲームシーンの初期化

// Camera
std::unique_ptr<Camera> camera_ = nullptr;
// Audio
std::unique_ptr<Audio> audio_ = nullptr;

// Sprite
std::unique_ptr<Sprite> sprite_ = nullptr;
// std::vector<std::unique_ptr<Sprite>> sprites_;
// Model
std::unique_ptr<Object3d> object3d_ = nullptr;

// Player
std::unique_ptr<Player> player_ = nullptr;
std::vector<Object3d*> bulletObjects;

// Enemy
std::vector<std::unique_ptr<Enemy>> enemies_;
std::vector<std::unique_ptr<Object3d>> enemyObjects3d_;
/// <summary>
/// getter
/// </summary>
/// <returns>DxCommonの取得</returns>
DirectXCommon* System::GetDXCommon() { return dxCommon_.get(); }
PipelineManager* System::GetPipelineManager() { return pipelineManager_.get(); }
WinApp* System::GetWinApp() { return winApp_.get(); }
std::vector<std::unique_ptr<Enemy>>& System::GetEnemies() { return enemies_; }
bool System::IsFinished() { return System::isFinished_; }

void System::SetIsFinished(bool finished) { isFinished_ = finished; }

bool System::isFinished_ = false;

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

void System::GameInit() {

	// Inputの初期化
	input_ = std::make_unique<Input>();
	input_->Initialize(winApp_.get());

	// テクスチャの読み込み
	const std::string& uvChecker = "./Resources/images/point.png";
	TextureManager::GetInstance()->LoadTexture(dxCommon_.get(), uvChecker);

	// Sprite
	sprite_ = std::make_unique<Sprite>();
	sprite_->Init(dxCommon_.get(), pipelineManager_.get(), uvChecker);

	// モデル読み込み
	ModelManager::GetInstance()->Init(dxCommon_.get());
	ModelManager::GetInstance()->LoadModel("skydome.obj");
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("player.obj");
	ModelManager::GetInstance()->LoadModel("playerBullet.obj");
	ModelManager::GetInstance()->LoadModel("enemy.obj");

	// Mesh
	mesh_ = std::make_unique<Mesh>();
	mesh_->LightSetting(dxCommon_.get());

	// カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({0.0f, 0.0f, 0.0f});
	camera_->SetTranslate({0.0f, 0.0f, -10.0f});

	// 天球の生成
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(dxCommon_.get());
	object3d_->SetModel("skydome.obj");
	object3d_->SetDefaultCamera(camera_.get());

	// オーディオの生成
	audio_ = std::make_unique<Audio>();
	audio_->Init();

	SoundData soundData = audio_->SoundLoadWave("Resources/fanfare.wav");
	// audio_->SoundPlayWave(audio_->GetXAudio2(), soundData);

	for (int i = 0; i < 50; ++i) {
		auto bulletObject = new Object3d();
		bulletObject->Init(dxCommon_.get());
		bulletObject->SetModel("playerBullet.obj");
		bulletObject->SetDefaultCamera(camera_.get());
		bulletObjects.push_back(bulletObject);
	}

	player_ = std::make_unique<Player>();
	player_->Init(dxCommon_.get(), camera_.get(), bulletObjects, sprite_.get(), input_.get());

	// ランダムな値を生成するための設定
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distX(-5.0f, 5.0f);  // X座標の範囲
	std::uniform_real_distribution<float> distY(-5.0f, 5.0f);  // Y座標の範囲
	std::uniform_real_distribution<float> distZ(20.0f, 50.0f); // Z座標の範囲
	// 敵の生成
	const int enemyCount = 5;
	for (int i = 0; i < enemyCount; ++i) {
		auto enemyObject = std::make_unique<Object3d>();
		enemyObject->Init(dxCommon_.get());
		enemyObject->SetModel("enemy.obj");
		enemyObject->SetDefaultCamera(camera_.get());

		auto enemy = std::make_unique<Enemy>();
		enemy->Init(dxCommon_.get(), camera_.get(), enemyObject.get());

		// 敵の初期位置をランダムに設定
		float randomX = distX(gen);
		float randomY = distY(gen);
		float randomZ = distZ(gen);
		enemy->SetTranslate({randomX, randomY, randomZ});

		enemies_.emplace_back(std::move(enemy));
		enemyObjects3d_.emplace_back(std::move(enemyObject));
	}
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

void System::GameUpdate() {

	sprite_->PreDraw();

	camera_->Update();

	input_->Update();

	object3d_->Update();

	sprite_->Update();

	sprite_->SetSize({32.0f, 32.0f});

	player_->Update();

	for (auto& enemy : enemies_) {
		enemy->Update();
	}

	CheckCollisions();

	/*==================================================================================*/
	// ImGui

#ifdef DEBUG

	camera_->ImGuiDebug();

	sprite_->ImGuiDebug();
	player_->ImGuiDebug();
	for (auto& enemy : enemies_) {
		enemy->ImGuiDebug();
	}

#endif // DEBUG
}

void System::Draw() {

	sprite_->Draw();

	player_->Draw();

	for (auto& enemy : enemies_) {
		enemy->Draw();
	}

	// object3d_->Draw();
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

	for (Object3d* bulletObject : bulletObjects) {
		delete bulletObject;
	}
	bulletObjects.clear();

	//
	TextureManager::GetInstance()->Finalize();
	ModelManager::GetInstance()->Finalize();

	imguiManager_->Finalize();
}

void System::DrawSprite() { sprite_->Draw(); }

void System::DrawObj() { object3d_->Draw(); }

void System::CheckCollisions() {
	auto& bullets = player_->GetBullets();

	// 敵と弾の当たり判定
	for (auto itBullet = bullets.begin(); itBullet != bullets.end();) {
		bool bulletHit = false;

		for (auto itEnemy = enemies_.begin(); itEnemy != enemies_.end();) {
			float distance = MyMath::CalculateDistance((*itBullet)->GetTranslate(), (*itEnemy)->GetTranslate());
			float collisionDistance = (*itBullet)->GetRadius() + (*itEnemy)->GetRadius();

			if (distance < collisionDistance) {
				// 衝突処理
				bulletHit = true;

				// 敵を削除
				itEnemy = enemies_.erase(itEnemy);
			} else {
				++itEnemy;
			}
		}

		if (bulletHit) {
			// 衝突した弾を削除
			itBullet = bullets.erase(itBullet);
		} else {
			++itBullet;
		}
	}

	// 敵が全ていなくなった場合
	if (enemies_.empty()) {
		isFinished_ = true;
	}
}