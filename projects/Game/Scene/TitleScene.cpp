#include "TitleScene.h"

#include "Engine/Base/System/System.h"
#include "GameScene.h"

void TitleScene::Init() {

	// テクスチャの読み込み
	const std::string& titleTexture = "./Resources/images/Title.png";
	TextureManager::GetInstance()->LoadTexture(titleTexture);

	// Sprite
	sprite_ = std::make_unique<Sprite>();
	sprite_->Init(titleTexture);

	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({0.0f, 0.0f, 0.0f});
	camera_->SetTranslate({0.0f, 0.0f, -10.0f});
}

void TitleScene::Update() {

	// Sprite描画前処理
	// sprite_->PreDraw();

	camera_->Update();

	sprite_->Update();

	if (System::TriggerKey(DIK_SPACE)) {
		// ゲームシーンを生成
		sceneManager_->ChangeScene("EXPLANATION");
	}
}

void TitleScene::Draw() { sprite_->Draw(); }

void TitleScene::Finalize() {}