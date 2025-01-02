#include "Player.h"
#include "externals/imgui/imgui.h"

void Player::Init(DirectXCommon* dxCommon, Camera* camera, Object3d* object3d, Object3d* bulletObject3d, Input* input) {

	dxCommon_ = dxCommon;
	camera_ = camera;
	input_ = input;
	object3d_ = object3d;
	bulletObject3d_ = bulletObject3d;

	object3d_->Init(dxCommon_);
	bullet_.Init(dxCommon_, camera_, bulletObject3d);

	transform_.translate = object3d_->GetTranslate();
}

void Player::Update() {

	Attack();

	// 弾がアクティブなら更新
	if (isBulletActive_) {
		bullet_.Update();

		// 弾が画面外に出たら非アクティブ化
		if (bullet_.GetTranslate().z > 20.0f) {
			isBulletActive_ = false;
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
	if (isBulletActive_) {
		bullet_.Draw();
	}
	object3d_->Draw();
}

void Player::ImGuiDebug() {
	ImGui::Begin("Player");
	ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.1f);
	ImGui::DragFloat3("translate", &transform_.translate.x, 1.0f);
	ImGui::End();

	bullet_.ImGuiDebug();
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE) && !isBulletActive_) {
		// 弾を発射
		bullet_.SetTranlate(transform_.translate);
		isBulletActive_ = true;
	}
}