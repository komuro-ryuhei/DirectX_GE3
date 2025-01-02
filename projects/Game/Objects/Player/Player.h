#pragma once

// MyCalss
#include "Engine/Base/3d/Object3d/Object3d.h"
#include "Engine/Base/Camera/Camera.h"
#include "Engine/Base/DirectXCommon/DirectXCommon.h"
#include "Engine/lib/Input/Input.h"
#include "struct.h"

#include "Game/Objects/PlayerBullet/PlayerBullet.h"

// C++
#include <vector>

class Player {

public:
	void Init(DirectXCommon* dxCommon, Camera* camera, Object3d* object3d, Object3d* bulletObject3d, Input* input);

	void Update();

	void Draw();

	void ImGuiDebug();

private:
	void Attack();

private:
	Camera* camera_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;

	Object3d* object3d_ = nullptr;
	Object3d* bulletObject3d_ = nullptr;

	Input* input_ = nullptr;

	Transform transform_;

	float velocity_ = 0.05f;

	// std::vector<PlayerBullet> bullets_;

	PlayerBullet bullet_;
	bool isBulletActive_ = false;
};