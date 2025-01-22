#pragma once

// MyCalss
#include "Engine/Base/3d/Object3d/Object3d.h"
#include "Engine/Base/Camera/Camera.h"
#include "Engine/Base/DirectXCommon/DirectXCommon.h"
#include "Engine/lib/Input/Input.h"
#include "struct.h"
#include "Game/Objects/Enemy/Enemy.h"

// C++
#include <vector>

class Enemy {
public:
	void Init(DirectXCommon* dxCommon, Camera* camera, Object3d* object3d);

	void Update();

	void Draw();

	void ImGuiDebug();

public:
	Vector3 GetTranslate();
	void SetTranslate(Vector3 translate);

	float GetRadius() const;

	bool GetIsAlive() const;

	void OnHit();

private:
	void Attack();

private:
	Camera* camera_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;

	Object3d* object3d_ = nullptr;

	Transform transform_;

	float velocity_ = 0.05f;

	// std::vector<PlayerBullet> bullets_;

	float radius_ = 1.0f;

	bool isAlive_ = true;

	// 点滅用の変数 
	bool isBlinking_ = false;
	int blinkCounter_ = 0;
	const int blinkDuration_ = 60;
};