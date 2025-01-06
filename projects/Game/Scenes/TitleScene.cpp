#include "TitleScene.h"

#include "Engine/Base/System/System.h"
#include "Engine/Base/TextureManager/TextureManager.h"

#include "Engine/lib/Math/MyMath.h"

bool TitleScene::IsFinished() const { return isFinished_; }

void TitleScene::Init(DirectXCommon* dxCommon, PipelineManager* pipelineManager, WinApp* winApp) {

	dxCommon_ = dxCommon;
	pipelineManager_ = pipelineManager;
	winApp_ = winApp;

	const std::string& uvTexture = "./Resources/images/monsterBall.png";
	TextureManager::GetInstance()->LoadTexture(dxCommon_, uvTexture);
	//
	sprite_ = std::make_unique<Sprite>();
	sprite_->Init(dxCommon_, pipelineManager_, uvTexture);

	input_ = std::make_unique<Input>();
	input_->Initialize(winApp_);
}

void TitleScene::Update() {

	// Sprite描画前処理
	sprite_->PreDraw();

	sprite_->Update();

	Vector2 size = {1280.0f, 720.0f};
	sprite_->SetSize(size);

	input_->Update();

	if (input_->TriggerKey(DIK_SPACE)) {
	}
}

void TitleScene::Draw() { sprite_->Draw(); }