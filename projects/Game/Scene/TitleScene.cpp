#include "TitleScene.h"

#include "GameScene.h"

void TitleScene::Init(DirectXCommon* dxCommon, PipelineManager* pipelineManager, Input* input) {

	dxCommon_ = dxCommon;
	pipelineManager_ = pipelineManager;
	input_ = input;

	// テクスチャの読み込み
	const std::string& uvTexture = "./Resources/images/uvChecker.png";
	TextureManager::GetInstance()->LoadTexture(dxCommon_, uvTexture);
	const std::string& monsterBallTexture = "./Resources/images/monsterBall.png";
	TextureManager::GetInstance()->LoadTexture(dxCommon_, monsterBallTexture);

	// Sprite
	sprite_ = std::make_unique<Sprite>();
	sprite_->Init(dxCommon_, pipelineManager_, uvTexture);

	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({0.0f, 0.0f, 0.0f});
	camera_->SetTranslate({0.0f, 0.0f, -10.0f});
}

void TitleScene::Update() {

	// Sprite描画前処理
	sprite_->PreDraw();

	camera_->Update();

	sprite_->Update();

	sprite_->ImGuiDebug();

	if (input_->TriggerKey(DIK_SPACE)) {
		// ゲームシーンを生成
		IScene* scene = new GameScene();
		// 
		sceneManager_->SetNextScene(scene);
	}
}

void TitleScene::Draw() { sprite_->Draw(); }

void TitleScene::Finalize() {}