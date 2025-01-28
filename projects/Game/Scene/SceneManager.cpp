#include "SceneManager.h"

SceneManager::~SceneManager() {

	// シーンの終了
	currentScene_->Finalize();
	delete currentScene_;
}

void SceneManager::Update(DirectXCommon* dxCommon, PipelineManager* pipelineManager, Input* input) {

	if (nextScene_) {
		if (currentScene_) {
			currentScene_->Finalize();
			delete currentScene_;
		}

		// シーンの切り替え
		currentScene_ = nextScene_;
		nextScene_ = nullptr;

		// シーンマネージャーのセット
		currentScene_->SetSceneManager(this);

		// 次のシーンの初期化
		currentScene_->Init(dxCommon, pipelineManager, input);
	}

	// 現在シーンの更新
	currentScene_->Update();
}

void SceneManager::Draw() { currentScene_->Draw(); }

void SceneManager::SetNextScene(IScene* nextScene) { nextScene_ = nextScene; }