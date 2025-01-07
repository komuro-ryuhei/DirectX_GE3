#include "Enemy.h"
#include "externals/imgui/imgui.h"

float Enemy::GetRadius() const { return radius_; }
bool Enemy::GetIsAlive() const { return isAlive_; }
Vector3 Enemy::GetTranslate() { return transform_.translate; }
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
	if (isBlinking_) {
		blinkCounter_++;
		if (blinkCounter_ >= blinkDuration_) {
			isBlinking_ = false;
			blinkCounter_ = 0;
		}
	} else if (transform_.translate.z >= 10.0f) {
		transform_.translate.z -= velocity_;
	}

	object3d_->SetTranslate(transform_.translate);
	object3d_->Update();
}

void Enemy::Draw() {
	// 点滅中はフレーム数によって描画を制御
	if (isBlinking_ && (blinkCounter_ / 5) % 2 == 0) {
		return; // 点滅効果で描画をスキップ
	}

	object3d_->Draw();
}

void Enemy::ImGuiDebug() {
	ImGui::Begin("Enemy");
	ImGui::DragFloat3("translate", &transform_.translate.x, 0.01f);
	ImGui::End();
}

void Enemy::OnHit() {
	isBlinking_ = true; // 点滅を開始
	blinkCounter_ = 0;  // カウンタをリセット

	isAlive_ = false;
}