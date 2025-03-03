#pragma once

#include "Engine/Base/3d/Object3d/Object3d.h"
#include "Engine/Base/Camera/Camera.h"

#include "Game/Objects/PlayerBullet.h"

#include <memory>

class Player {

public:
	void Init(Camera* camera);

	void Update();

	void Draw();

	void ImGuiDebug();

private:
	void ChangeBulletMode();
	void Attack();

private:
	// モデル
	std::unique_ptr<Object3d> object3d_ = nullptr;
	std::vector<Object3d*> bulletObjects_;
	// 弾のリスト
	std::vector<std::unique_ptr<PlayerBullet>> bullets_;

	Camera* camera_ = nullptr;

	// SRT
	Transform transform_;

	// 
	BulletMode currentBulletMode_ = BulletMode::normal;
};