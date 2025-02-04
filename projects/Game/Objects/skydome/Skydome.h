#pragma once

#include "Engine/Base/3d/Object3d/Object3d.h"

class Skydome {

public:
	void Init();

	void Update();

	void Draw();

	void ImGuiDebug();

private:
	// モデル
	std::unique_ptr<Object3d> object3d_ = nullptr;

	Transform transform_;
};