#include "ExplanationScene.h"

#include "Engine/Base/System/System.h"
#include "GameScene.h"

void ExplanationScene::Init() {

	// テクスチャの読み込み
	const std::string& explanationTexture = "./Resources/images/Explanation.png";
	TextureManager::GetInstance()->LoadTexture(explanationTexture);

	// Sprite
	sprite_ = std::make_unique<Sprite>();
	sprite_->Init(explanationTexture);

	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({0.0f, 0.0f, 0.0f});
	camera_->SetTranslate({0.0f, 0.0f, -10.0f});
}

void ExplanationScene::Update() {

	// Sprite描画前処理
	// sprite_->PreDraw();

	camera_->Update();

	sprite_->Update();

	sprite_->ImGuiDebug();

	if (System::TriggerKey(DIK_SPACE)) {
		// ゲームシーンを生成
		sceneManager_->ChangeScene("GAME");
	}
}

void ExplanationScene::Draw() { sprite_->Draw(); }

void ExplanationScene::Finalize() {}