#include "ExplanationScene.h"

#include "Engine/Base/System/System.h"
#include "Engine/Base/TextureManager/TextureManager.h"

#include "Engine/lib/Math/MyMath.h"

bool ExplanationScene::IsFinished() const { return isFinished_; }

void ExplanationScene::Init(DirectXCommon* dxCommon, PipelineManager* pipelineManager, WinApp* winApp) {

	dxCommon_ = dxCommon;
	pipelineManager_ = pipelineManager;
	winApp_ = winApp;

	const std::string& explanation = "./Resources/images/explanation.png";
	TextureManager::GetInstance()->LoadTexture(dxCommon_, explanation);
	//
	sprite_ = std::make_unique<Sprite>();
	sprite_->Init(dxCommon_, pipelineManager_, explanation);

	input_ = std::make_unique<Input>();
	input_->Initialize(winApp_);

	isFinished_ = false;
}

void ExplanationScene::Update() {

	// Sprite描画前処理
	sprite_->PreDraw();

	Vector2 size = {1280.0f, 720.0f};
	sprite_->SetSize(size);

	sprite_->Update();

	input_->Update();

	if (input_->TriggerKey(DIK_RETURN)) {
		isFinished_ = true;
	}
}

void ExplanationScene::Draw() { sprite_->Draw(); }