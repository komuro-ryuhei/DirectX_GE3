#pragma once

#include "Engine/Base/DirectXCommon/DirectXCommon.h"
#include "Engine/Base/PSO/PipelineManager/PipelineManager.h"
#include "Engine/lib/Input/Input.h"

#include "Game/Scene/AbstractSceneFactory.h"
#include "Game/Scene/IScene.h"
#include <memory>

class SceneManager {

public:
	~SceneManager();

	void Update(DirectXCommon* dxCommon, PipelineManager* pipelineManager, Input* input);

	void Draw();

	// 次シーン予約
	void SetNextScene(IScene* nextScene);

	void ChangeScene(const std::string& sceneName);

	// シーンファクトリーのsetter
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

private:
	// シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;

	// 今のシーン
	IScene* currentScene_ = nullptr;
	// 次のシーン
	IScene* nextScene_ = nullptr;
};