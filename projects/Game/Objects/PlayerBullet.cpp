#include "PlayerBullet.h"

#include "externals/imgui/imgui.h"

float PlayerBullet::GetRadius() const { return radius_; }

void PlayerBullet::Init(Camera* camera, Object3d* object3d) {

	camera_ = camera;
	object3d_ = object3d;

	object3d_->Init();

	object3d_->SetTranslate(transform_.translate);
	object3d_->SetScale({0.5f, 0.5f, 0.5f});

	// 初回フレームの時間を記録
	prevTime_ = std::chrono::steady_clock::now();
}

void PlayerBullet::Update() {

	 // 現在の時間を取得し、差分（deltaTime）を計算
	auto now = std::chrono::steady_clock::now();
	std::chrono::duration<float> deltaTime = now - prevTime_;
	prevTime_ = now;

	if (bulletMode_ == BulletMode::normal) {
		speed_ = 0.1f;
	} else if (bulletMode_ == BulletMode::machinegun) {
		speed_ = 0.5f;
	} else if (bulletMode_ == BulletMode::shotgun) {
		speed_ = 0.2f;
	}

	transform_.translate += direction_ * speed_;
	object3d_->SetTranslate(transform_.translate);
	object3d_->Update();

	 // 弾の寿命を減らす
	lifeTime_ -= deltaTime.count();
	if (lifeTime_ <= 0.0f) {
		isAlive_ = false; // 削除フラグを立てる
	}
}


void PlayerBullet::Draw() { object3d_->Draw(); }

void PlayerBullet::ImGuiDebug() {

	//
	ImGui::Begin("PlayerBullet");

	ImGui::DragFloat3("bulletTranlate", &transform_.translate.x, 0.01f);
	ImGui::DragFloat3("bulletRotate", &transform_.rotate.x, 0.01f);

	if (bulletMode_ == BulletMode::normal) {
		ImGui::Text("normal");
	} else if (bulletMode_ == BulletMode::machinegun) {
		ImGui::Text("machinegun");
	}

	ImGui::End();
}

Vector3 PlayerBullet::GetTranslate() const { return transform_.translate; }

void PlayerBullet::SetTranlate(Vector3 translate) { transform_.translate = translate; }

void PlayerBullet::SetDirection(const Vector3& direction) { direction_ = direction; }

void PlayerBullet::SetBulletMode(BulletMode bulletMode) { bulletMode_ = bulletMode; }

bool PlayerBullet::IsAlive() const { return isAlive_; }