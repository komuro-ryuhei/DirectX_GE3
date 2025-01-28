#pragma once

#include "Engine/Base/DirectXCommon/DirectXCommon.h"
#include "Engine/Base/PSO/PipelineManager/PipelineManager.h"
#include "Engine/lib/Input/Input.h"

#include "Game/Scene/IScene.h"
#include <memory>

class SceneManager {

public:
	~SceneManager();

	void Update(DirectXCommon* dxCommon, PipelineManager* pipelineManager, Input* input);

	void Draw();

	// 次シーン予約
	void SetNextScene(IScene* nextScene);

private:
	// 今のシーン
	IScene* currentScene_ = nullptr;
	// 次のシーン
	IScene* nextScene_ = nullptr;
};