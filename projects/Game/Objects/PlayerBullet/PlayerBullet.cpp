#include "PlayerBullet.h"

#include "externals/imgui/imgui.h"

float PlayerBullet::GetRadius() const { return radius_; }

void PlayerBullet::Init(DirectXCommon* dxCommon, Camera* camera, Object3d* object3d) {

	dxCommon_ = dxCommon;
	camera_ = camera;
	object3d_ = object3d;

	object3d_->Init(dxCommon_);

	transform_.translate = object3d_->GetTranslate();
}

void PlayerBullet::Update() {

	transform_.translate += direction_ * speed_; 
	object3d_->SetTranslate(transform_.translate);
	object3d_->Update();
}

void PlayerBullet::Draw() { object3d_->Draw(); }

void PlayerBullet::ImGuiDebug() {

	//
	ImGui::Begin("PlayerBullet");

	ImGui::DragFloat3("bulletTranlate", &transform_.translate.x, 0.01f);
	ImGui::DragFloat3("bulletRotate", &transform_.rotate.x, 0.01f);

	ImGui::End();
}

Vector3 PlayerBullet::GetTranslate() const { return transform_.translate; }

void PlayerBullet::SetTranlate(Vector3 translate) { transform_.translate = translate; }

void PlayerBullet::SetDirection(const Vector3& direction) { direction_ = direction; }