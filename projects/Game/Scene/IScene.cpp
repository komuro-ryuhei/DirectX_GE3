#include "IScene.h"

void IScene::Init(DirectXCommon* dxCommon, PipelineManager* pipelineManager, Input* input) {}

void IScene::Update() {}

void IScene::Draw() {}

void IScene::Finalize() {}

void IScene::SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }