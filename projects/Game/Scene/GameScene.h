#pragma once

#include <memory>

#include "Engine/Base/DirectXCommon/DirectXCommon.h"
#include "Engine/Base/PSO/PipelineManager/PipelineManager.h"

#include "Engine/Base/2d/Sprite/Sprite.h"
#include "Engine/Base/3d/Object3d/Object3d.h"
#include "Engine/Base/Audio/Audio.h"
#include "Engine/Base/Camera/Camera.h"
#include "Game/Objects/Enemy/Enemy.h"
#include "Game/Objects/Player/Player.h"
#include "Game/Objects/skydome/Skydome.h"

#include "Game/Scene/IScene.h"
#include "Game/Scene/SceneManager.h"

class GameScene : public IScene {
public:
	void Init() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:
	void CheckCollisions();

private:
	// Camera
	std::unique_ptr<Camera> camera_ = nullptr;
	// Audio
	std::unique_ptr<Audio> audio_ = nullptr;
	// Sprite
	std::unique_ptr<Sprite> sprite_ = nullptr;
	// Model
	std::unique_ptr<Object3d> object3d_ = nullptr;

	// Skydome
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// Player
	std::unique_ptr<Player> player_ = nullptr;

	// Enemy
	std::vector<std::unique_ptr<Enemy>> enemies_;
	std::vector<std::unique_ptr<Object3d>> enemyObjects3d_;
};