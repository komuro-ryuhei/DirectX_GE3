#include "Enemy.h"

#include "externals/imgui/imgui.h"

void Enemy::SetTranslate(Vector3 translate) { transform_.translate = translate; }

void Enemy::Init(DirectXCommon* dxCommon, Camera* camera, Object3d* object3d) {

	dxCommon_ = dxCommon;
	camera_ = camera;
	object3d_ = object3d;

	object3d_->Init(dxCommon_);

	transform_.translate = {0.0f, 0.0f, 10.0f};
	object3d_->SetTranslate(transform_.translate);
}

void Enemy::Update() {

	if (transform_.translate.z >= 10.0f) {
		transform_.translate.z -= velocity_;
	}

	object3d_->SetTranslate(transform_.translate);

	object3d_->Update();
}

void Enemy::Draw() { object3d_->Draw(); }

void Enemy::ImGuiDebug() {
	ImGui::Begin("Enemy");
	ImGui::DragFloat3("translate", &transform_.translate.x, 0.01f);
	ImGui::End();
}

void Enemy::Attack() {}