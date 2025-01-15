#pragma once

#include "Engine/Base/DirectXCommon/DirectXCommon.h"
#include "Engine/Base/PSO/PipelineManager/PipelineManager.h"
#include "Engine/Base/WinApp/WinApp.h"

#include "Engine/Base/Audio/Audio.h"
#include "Engine/Base/Camera/Camera.h"
#include "Engine/lib/Input/Input.h"
#include "Game/Objects/Enemy/Enemy.h"
#include "Game/Objects/Player/Player.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public:

	~GameScene();

	void Init(DirectXCommon* dxCommon, PipelineManager* pipelineManager, WinApp* winApp);

	void Update();

	void Draw();

	void Finalize();

	bool IsFinished() const;

private:

	void CheckCollisions();

private:
	DirectXCommon* dxCommon_ = nullptr;
	PipelineManager* pipelineManager_ = nullptr;
	WinApp* winApp_ = nullptr;

	// Camera
	std::unique_ptr<Camera> camera_ = nullptr;
	// Audio
	std::unique_ptr<Audio> audio_ = nullptr;
	// Input
	std::unique_ptr<Input> input_ = nullptr;
	// Sprite
	std::unique_ptr<Sprite> sprite_ = nullptr;
	// Model
	std::unique_ptr<Object3d> object3d_ = nullptr;

	// Player
	std::unique_ptr<Player> player_ = nullptr;

	// Enemy
	std::vector<std::unique_ptr<Enemy>> enemies_;
	std::vector<std::unique_ptr<Object3d>> enemyObjects3d_;

	bool isFinished_ = false;
};