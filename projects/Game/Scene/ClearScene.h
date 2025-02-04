#pragma once

#include "Engine/Base/2d/Sprite/Sprite.h"
#include "Engine/Base/3d/Object3d/Object3d.h"
#include "Engine/Base/Audio/Audio.h"
#include "Engine/Base/Camera/Camera.h"

#include "Game/Scene/IScene.h"
#include "Game/Scene/SceneManager.h"

class ClearScene : public IScene {

public:
	void Init() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:
	// Sprite
	std::unique_ptr<Sprite> sprite_ = nullptr;
};