#pragma once

// MyCalss
#include "Engine/Base/3d/Object3d/Object3d.h"
#include "Engine/Base/Camera/Camera.h"
#include "Engine/Base/DirectXCommon/DirectXCommon.h"
#include "Engine/lib/Input/Input.h"
#include "struct.h"

class PlayerBullet {

public:
	void Init(DirectXCommon* dxCommon, Camera* camera, Object3d* object3d);

	void Update();

	void Draw();

	void ImGuiDebug();

	Vector3 GetTranslate() const;
	void SetTranlate(Vector3 translate);

private:
	Camera* camera_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;

	Object3d* object3d_ = nullptr;

	Transform transform_;

	float speed_ = 0.1f;
};