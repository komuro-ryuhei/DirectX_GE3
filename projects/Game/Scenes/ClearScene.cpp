#include "ClearScene.h"

#include "Engine/Base/System/System.h"
#include "Engine/Base/TextureManager/TextureManager.h"

#include "Engine/lib/Math/MyMath.h"

bool ClearScene::IsFinished() const { return isFinished_; }

void ClearScene::Init(DirectXCommon* dxCommon, PipelineManager* pipelineManager, WinApp* winApp) {

	dxCommon_ = dxCommon;
	pipelineManager_ = pipelineManager;
	winApp_ = winApp;

	const std::string& clear = "./Resources/images/Clear.png";
	TextureManager::GetInstance()->LoadTexture(dxCommon_, clear);
	//
	sprite_ = std::make_unique<Sprite>();
	sprite_->Init(dxCommon_, pipelineManager_, clear);

	input_ = std::make_unique<Input>();
	input_->Initialize(winApp_);

	isFinished_ = false;
}

void ClearScene::Update() {

	// Sprite描画前処理
	sprite_->PreDraw();

	Vector2 size = {1280.0f, 720.0f};
	sprite_->SetSize(size);

	sprite_->Update();

	input_->Update();

	if (input_->TriggerKey(DIK_SPACE)) {
		isFinished_ = true;
	}
}

void ClearScene::Draw() { sprite_->Draw(); }