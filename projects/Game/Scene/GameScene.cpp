#include "GameScene.h"

void GameScene::Init(DirectXCommon* dxCommon, PipelineManager* pipelineManager) {

	dxCommon_ = dxCommon;
	pipelineManager_ = pipelineManager;

	// テクスチャの読み込み
	const std::string& uvTexture = "./Resources/images/uvChecker.png";
	TextureManager::GetInstance()->LoadTexture(dxCommon_, uvTexture);
	const std::string& monsterBallTexture = "./Resources/images/monsterBall.png";
	TextureManager::GetInstance()->LoadTexture(dxCommon_, monsterBallTexture);

	// Sprite
	sprite_ = std::make_unique<Sprite>();
	sprite_->Init(dxCommon_, pipelineManager_, uvTexture);

	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(dxCommon_);

	ModelManager::GetInstance()->LoadModel("plane.obj");
	object3d_->SetModel("plane.obj");

	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({0.0f, 0.0f, 0.0f});
	camera_->SetTranslate({0.0f, 0.0f, -10.0f});
	object3d_->SetDefaultCamera(camera_.get());

	audio_ = std::make_unique<Audio>();
	audio_->Init();

	SoundData soundData = audio_->SoundLoadWave("Resources/fanfare.wav");
	// audio_->SoundPlayWave(audio_->GetXAudio2(), soundData);

}

void GameScene::Update() {

	// Sprite描画前処理
	sprite_->PreDraw();

	camera_->Update();

	sprite_->Update();

	object3d_->Update();

	sprite_->ImGuiDebug();

}

void GameScene::Draw() {

	sprite_->Draw();
	object3d_->Draw();
}

void GameScene::Finalize() {}
