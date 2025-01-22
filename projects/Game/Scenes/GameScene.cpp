#include "GameScene.h"

#include "Engine/Base/System/System.h"

#include <random>

bool GameScene::IsFinished() const { return isFinished_; }
// std::vector<std::unique_ptr<Enemy>>& GameScene::GetEnemies() { return enemies_; }

GameScene::~GameScene() {

	player_.reset();
	sprite_.reset();
	camera_.reset();
	audio_.reset();

	enemyObjects3d_.clear();
	enemies_.clear();

	object3d_.reset();
}

void GameScene::Init(DirectXCommon* dxCommon, PipelineManager* pipelineManager, WinApp* winApp) {

	dxCommon_ = dxCommon;
	pipelineManager_ = pipelineManager;
	winApp_ = winApp;

	// Inputの初期化
	input_ = std::make_unique<Input>();
	input_->Initialize(winApp_);

	// テクスチャの読み込み
	const std::string& target = "./Resources/images/point.png";
	TextureManager::GetInstance()->LoadTexture(dxCommon_, target);

	// Sprite
	sprite_ = std::make_unique<Sprite>();
	sprite_->Init(dxCommon_, pipelineManager_, target);

	// モデル読み込み
	ModelManager::GetInstance()->Init(dxCommon_);
	ModelManager::GetInstance()->LoadModel("skydome.obj");
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("player.obj");
	ModelManager::GetInstance()->LoadModel("playerBullet.obj");
	ModelManager::GetInstance()->LoadModel("enemy.obj");

	// カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({0.0f, 0.0f, 0.0f});
	camera_->SetTranslate({0.0f, 0.0f, -10.0f});

	// 天球の生成
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(dxCommon_);
	object3d_->SetModel("skydome.obj");
	object3d_->SetDefaultCamera(camera_.get());

	// オーディオの生成
	audio_ = std::make_unique<Audio>();
	audio_->Init();

	SoundData soundData = audio_->SoundLoadWave("Resources/fanfare.wav");
	// audio_->SoundPlayWave(audio_->GetXAudio2(), soundData);

	player_ = std::make_unique<Player>();
	player_->Init(dxCommon_, camera_.get(), sprite_.get(), input_.get());

	// ランダムな値を生成するための設定
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distX(-5.0f, 5.0f);  // X座標の範囲
	std::uniform_real_distribution<float> distY(-5.0f, 5.0f);  // Y座標の範囲
	std::uniform_real_distribution<float> distZ(20.0f, 50.0f); // Z座標の範囲
	// 敵の生成
	const int enemyCount = 5;
	for (int i = 0; i < enemyCount; ++i) {
		auto enemyObject = std::make_unique<Object3d>();
		enemyObject->Init(dxCommon_);
		enemyObject->SetModel("enemy.obj");
		enemyObject->SetDefaultCamera(camera_.get());

		auto enemy = std::make_unique<Enemy>();
		enemy->Init(dxCommon_, camera_.get(), enemyObject.get());

		// 敵の初期位置をランダムに設定
		float randomX = distX(gen);
		float randomY = distY(gen);
		float randomZ = distZ(gen);
		enemy->SetTranslate({randomX, randomY, randomZ});

		enemies_.emplace_back(std::move(enemy));
		enemyObjects3d_.emplace_back(std::move(enemyObject));
	}
	isFinished_ = false;
}

void GameScene::Update() {

	sprite_->PreDraw();

	camera_->Update();

	input_->Update();

	object3d_->Update();

	sprite_->Update();

	sprite_->SetSize({32.0f, 32.0f});

	player_->Update();

	if (input_->TriggerKey(DIK_L)) {
		// Lキーでロックオン対象を更新
		player_->LockOnTarget(enemies_);
	}

	for (auto& enemy : enemies_) {
		enemy->Update();
	}

	CheckCollisions();

	/*==================================================================================*/
	// ImGui

#ifdef DEBUG

	camera_->ImGuiDebug();

	sprite_->ImGuiDebug();
	player_->ImGuiDebug();
	for (auto& enemy : enemies_) {
		enemy->ImGuiDebug();
	}

#endif // DEBUG
}

void GameScene::Draw() {

	sprite_->Draw();

	player_->Draw();

	for (auto& enemy : enemies_) {
		enemy->Draw();
	}
}

void GameScene::CheckCollisions() {

	auto& bullets = player_->GetBullets();

	// 敵と弾の当たり判定
	for (auto itBullet = bullets.begin(); itBullet != bullets.end();) {
		bool bulletHit = false;

		for (auto itEnemy = enemies_.begin(); itEnemy != enemies_.end();) {
			float distance = MyMath::CalculateDistance((*itBullet)->GetTranslate(), (*itEnemy)->GetTranslate());
			float collisionDistance = (*itBullet)->GetRadius() + (*itEnemy)->GetRadius();

			if (distance < collisionDistance) {
				// 衝突処理
				bulletHit = true;

				// 敵を削除
				itEnemy = enemies_.erase(itEnemy);
			} else {
				++itEnemy;
			}
		}

		if (bulletHit) {
			// 衝突した弾を削除
			itBullet = bullets.erase(itBullet);
		} else {
			++itBullet;
		}
	}

	// 敵が全ていなくなった場合
	if (enemies_.empty()) {
		isFinished_ = true;
	}
}