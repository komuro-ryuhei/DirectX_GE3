#include "Player.h"
#include "Engine/Base/System/System.h"

#include "externals/imgui/imgui.h"
#include <random>

void Player::Init(Camera* camera) {

	camera_ = camera;

	object3d_ = std::make_unique<Object3d>();
	object3d_->Init();

	object3d_->SetModel("player.obj");
	object3d_->SetDefaultCamera(camera_);

	// 弾オブジェクトの生成
	for (int i = 0; i < 999; ++i) {
		auto bulletObject = new Object3d();
		bulletObject->Init();
		bulletObject->SetModel("player.obj");
		bulletObject->SetDefaultCamera(camera_);
		bulletObjects_.push_back(bulletObject);
	}
}

void Player::Update() {

	object3d_->Update();

	Attack();
	ChangeBulletMode();

	// 弾の更新と削除
	for (auto it = bullets_.begin(); it != bullets_.end();) {
		(*it)->Update();
		(*it)->ImGuiDebug();

		// 寿命が尽きた弾を削除
		if (!(*it)->IsAlive()) {
			it = bullets_.erase(it);
		} else {
			++it;
		}
	}

	ImGuiDebug();
}

void Player::Draw() {

	//
	object3d_->Draw();

	for (auto& bullet : bullets_) {
		bullet->Draw();
	}
}

void Player::ImGuiDebug() {

	ImGui::Begin("Player");

	if (currentBulletMode_ == BulletMode::normal) {
		ImGui::Text("normal");
	} else if (currentBulletMode_ == BulletMode::machinegun) {
		ImGui::Text("machinegun");
	} else if (currentBulletMode_ == BulletMode::shotgun) {
		ImGui::Text("shotgun");
	}

	ImGui::End();
}

void Player::ChangeBulletMode() {

	//
	if (System::TriggerKey(DIK_1)) {
		currentBulletMode_ = BulletMode::normal;
	} else if (System::TriggerKey(DIK_2)) {
		currentBulletMode_ = BulletMode::machinegun;
	} else if (System::TriggerKey(DIK_3)) {
		currentBulletMode_ = BulletMode::shotgun;
	}
}

void Player::Attack() {

	if (System::TriggerKey(DIK_SPACE) && !bulletObjects_.empty()) {
		if (currentBulletMode_ == BulletMode::shotgun) {
			// 8発のショットガン弾を生成
			const int bulletCount = 12;
			const float spreadAngle = 30.0f;

			// ランダム生成の準備
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> angleDist(-spreadAngle / 2.0f, spreadAngle / 2.0f);

			for (int i = 0; i < bulletCount; ++i) {
				if (bulletObjects_.empty())
					break;

				Object3d* bulletObject = bulletObjects_.back();
				bulletObjects_.pop_back();

				auto newBullet = std::make_unique<PlayerBullet>();
				newBullet->Init(camera_, bulletObject);
				newBullet->SetTranlate(transform_.translate);

				// ランダムな角度を設定
				float angleX = angleDist(gen); // 左右の拡散
				float angleY = angleDist(gen); // 上下の拡散

				float radianX = angleX * (3.1415926535f / 180.0f);
				float radianY = angleY * (3.1415926535f / 180.0f);

				// 前方向のベクトル
				Vector3 spreadDirection = {
				    sin(radianX),               // X方向
				    sin(radianY),               // Y方向
				    cos(radianX) * cos(radianY) // Z方向
				};

				MyMath::Normalize(spreadDirection);
				newBullet->SetDirection(spreadDirection);

				// ショットガンの弾のモードを設定
				newBullet->SetBulletMode(BulletMode::shotgun);

				bullets_.emplace_back(std::move(newBullet));
			}
		} else {
			// 通常の弾・マシンガン
			Object3d* bulletObject = bulletObjects_.back();
			bulletObjects_.pop_back();

			auto newBullet = std::make_unique<PlayerBullet>();
			newBullet->Init(camera_, bulletObject);
			newBullet->SetTranlate(transform_.translate);

			Vector3 straightDirection = {0.0f, 0.0f, 1.0f};
			MyMath::Normalize(straightDirection);
			newBullet->SetDirection(straightDirection);

			newBullet->SetBulletMode(currentBulletMode_);

			bullets_.emplace_back(std::move(newBullet));
		}
	}
}