#include "Player.h"
#include "externals/imgui/imgui.h"

#include "Engine/Base/System/System.h"

#include <iostream>

float Player::GetRadius() const { return radius_; }
std::vector<std::unique_ptr<PlayerBullet>>& Player::GetBullets() { return bullets_; }

void Player::Init(DirectXCommon* dxCommon, Camera* camera, std::vector<Object3d*> bulletObjects, Sprite* sprite, Input* input) {

	dxCommon_ = dxCommon;
	camera_ = camera;
	input_ = input;
	bulletObjects_ = bulletObjects;
	lockOnSprite_ = sprite;

	// 自機、弾の生成
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(dxCommon_);

	object3d_->SetModel("player.obj");
	object3d_->SetDefaultCamera(camera_);

	object3d_->Init(dxCommon_);
	object3d_->SetScale({0.5f, 0.5f, 0.5f});

	transform_.translate = object3d_->GetTranslate();
}

void Player::Update() {

	if (input_->TriggerKey(DIK_L)) {
		// Lキーでロックオン対象を更新
		LockOnTarget(System::GetEnemies());
	}

	if (lockedTarget_ && lockedTarget_->GetIsAlive()) {
		// 必要な行列を取得
		Matrix4x4 viewMatrix = camera_->GetViewMatrix();
		Matrix4x4 projMatrix = camera_->GetProjectionMatrix();
		Matrix4x4 viewportMatrix = MyMath::MakeViewportMatrix(0.0f, 0.0f, 1280, 720, 0.0f, 1.0f);

		// ロックオン対象の座標をスクリーン座標に変換
		Vector3 targetPosition = lockedTarget_->GetTranslate();
		Vector2 screenPosition = MyMath::WorldToScreen(targetPosition, viewMatrix, projMatrix, viewportMatrix);

		// スプライトの位置を更新
		lockOnSprite_->SetPosition(screenPosition);
	}

	Attack();

	// 弾の更新と削除
	for (auto it = bullets_.begin(); it != bullets_.end();) {
		(*it)->Update();

		++it;
	}

	transform_.translate = object3d_->GetTranslate();

	Move();

	object3d_->Update();
}

void Player::Draw() {
	for (auto& bullet : bullets_) {
		bullet->Draw();
	}
	object3d_->Draw();

	// ロックオンスプライトの描画
	if (lockedTarget_ && lockedTarget_->GetIsAlive()) {
		lockOnSprite_->Draw();
	}
}

void Player::ImGuiDebug() {
	ImGui::Begin("Player");
	ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.1f);
	ImGui::DragFloat3("translate", &transform_.translate.x, 0.1f);
	ImGui::End();
}

void Player::Attack() {

	if (input_->TriggerKey(DIK_SPACE) && !bulletObjects_.empty()) {
		Object3d* bulletObject = bulletObjects_.back();
		bulletObjects_.pop_back();

		auto newBullet = std::make_unique<PlayerBullet>();
		newBullet->Init(dxCommon_, camera_, bulletObject);
		newBullet->SetTranlate(transform_.translate);

		// ロックオン対象が有効か確認
		if (lockedTarget_ && lockedTarget_->GetIsAlive()) {
			// ロックオン対象に向けて弾を発射
			Vector3 direction = lockedTarget_->GetTranslate() - transform_.translate;
			MyMath::Normalize(direction);
			newBullet->SetDirection(direction);
		} else {
			// ロックオン対象が無効ならデフォルトの前方向に発射
			lockedTarget_ = nullptr;
			Vector3 straightDirection = {0.0f, 0.0f, 1.0f};
			MyMath::Normalize(straightDirection);
			newBullet->SetDirection(straightDirection);
		}

		bullets_.emplace_back(std::move(newBullet));
	}
}

void Player::Move() {

	if (input_->PushKey(DIK_A)) {
		transform_.translate.x -= velocity_;
		object3d_->SetTranslate(transform_.translate);
	}

	if (input_->PushKey(DIK_D)) {
		transform_.translate.x += velocity_;
		object3d_->SetTranslate(transform_.translate);
	}

	if (input_->PushKey(DIK_W)) {
		transform_.translate.y += velocity_;
		object3d_->SetTranslate(transform_.translate);
	}

	if (input_->PushKey(DIK_S)) {
		transform_.translate.y -= velocity_;
		object3d_->SetTranslate(transform_.translate);
	}
}

void Player::LockOnTarget(std::vector<std::unique_ptr<Enemy>>& enemies) {
	float closestDistance = (std::numeric_limits<float>::max)();
	Enemy* closestEnemy = nullptr;

	for (auto& enemy : enemies) {
		if (!enemy->GetIsAlive())
			continue;

		float distance = MyMath::CalculateDistance(transform_.translate, enemy->GetTranslate());
		if (distance < closestDistance) {
			closestDistance = distance;
			closestEnemy = enemy.get();
		}
	}

	lockedTarget_ = closestEnemy; // 最も近い敵をロックオン
}