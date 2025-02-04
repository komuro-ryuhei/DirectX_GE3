#include "ClearScene.h"

#include "Engine/Base/System/System.h"
#include "GameScene.h"

void ClearScene::Init() {

	// テクスチャの読み込み
	const std::string& clearTexture = "./Resources/images/Clear.png";
	TextureManager::GetInstance()->LoadTexture(clearTexture);

	// Sprite
	sprite_ = std::make_unique<Sprite>();
	sprite_->Init(clearTexture);
}

void ClearScene::Update() {

	// Sprite描画前処理
	// sprite_->PreDraw();

	sprite_->Update();

	sprite_->ImGuiDebug();

	if (System::TriggerKey(DIK_SPACE)) {
		// ゲームシーンを生成
		sceneManager_->ChangeScene("TITLE");
	}
}

void ClearScene::Draw() { sprite_->Draw(); }

void ClearScene::Finalize() {}