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

	if (System::TriggerKey(DIK_SPACE)) {
		if (currentBulletMode_ == BulletMode::shotgun) {
			const int bulletCount = 12;

			for (int i = 0; i < bulletCount; ++i) {
				auto bulletObject = std::make_unique<Object3d>();
				bulletObject->Init();
				bulletObject->SetModel("player.obj");
				bulletObject->SetDefaultCamera(camera_);

				auto newBullet = std::make_unique<PlayerBullet>();
				newBullet->Init(camera_, bulletObject.get());
				newBullet->SetTranlate(transform_.translate);
				newBullet->SetBulletMode(BulletMode::shotgun);
				newBullet->Fire();

				bullets_.emplace_back(std::move(newBullet));
				bulletObjects_.emplace_back(std::move(bulletObject));
			}
		} else {
			auto bulletObject = std::make_unique<Object3d>();
			bulletObject->Init();
			bulletObject->SetModel("player.obj");
			bulletObject->SetDefaultCamera(camera_);

			auto newBullet = std::make_unique<PlayerBullet>();
			newBullet->Init(camera_, bulletObject.get());
			newBullet->SetTranlate(transform_.translate);
			newBullet->SetBulletMode(currentBulletMode_);
			newBullet->Fire();

			bullets_.emplace_back(std::move(newBullet));
			bulletObjects_.emplace_back(std::move(bulletObject));
		}
	}
}