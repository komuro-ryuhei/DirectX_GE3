#include "Player.h"
#include "externals/imgui/imgui.h"

void Player::Init(DirectXCommon* dxCommon, Camera* camera, Object3d* object3d, std::vector<Object3d*> bulletObjects, Input* input) {

	dxCommon_ = dxCommon;
	camera_ = camera;
	input_ = input;
	object3d_ = object3d;
	bulletObjects_ = bulletObjects;

	object3d_->Init(dxCommon_);
	// bullet_.Init(dxCommon_, camera_, bulletObject3d);

	transform_.translate = object3d_->GetTranslate();
}

void Player::Update() {

	Attack();

	// 弾の更新と削除
	for (auto it = bullets_.begin(); it != bullets_.end();) {
		(*it)->Update();
		if ((*it)->GetTranslate().z > 20.0f) {
			it = bullets_.erase(it); // 画面外に出た弾を削除
		} else {
			++it;
		}
	}

	transform_.translate = object3d_->GetTranslate();

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

	object3d_->Update();
}

void Player::Draw() {
	for (auto& bullet : bullets_) {
		bullet->Draw();
	}
	object3d_->Draw();
}

void Player::ImGuiDebug() {
	ImGui::Begin("Player");
	ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.1f);
	ImGui::DragFloat3("translate", &transform_.translate.x, 0.1f);
	ImGui::End();

	bullet_.ImGuiDebug();
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE) && !bulletObjects_.empty()) {
		// 新しい弾を発射
		Object3d* bulletObject = bulletObjects_.back();
		bulletObjects_.pop_back();

		auto newBullet = std::make_unique<PlayerBullet>();
		newBullet->Init(dxCommon_, camera_, bulletObject);
		newBullet->SetTranlate(transform_.translate);

		bullets_.emplace_back(std::move(newBullet));
	}
}