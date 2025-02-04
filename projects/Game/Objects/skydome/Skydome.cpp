#include "Skydome.h"

#include "externals/imgui/imgui.h"

void Skydome::Init() {

	//
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init();

	object3d_->SetModel("skydome.obj");

	transform_.scale = {1.0f, 1.0f, 1.0f};
	transform_.translate = {0.0f, 0.0f, 30.0f};
}

void Skydome::Update() {

	//
	object3d_->Update();
	// transform_.translate = object3d_->GetTranslate();
	object3d_->SetTranslate(transform_.translate);
	object3d_->SetScale(transform_.scale);
}

void Skydome::Draw() {

	//
	object3d_->Draw();
}

void Skydome::ImGuiDebug() {
	ImGui::Begin("Skydome");
	ImGui::DragFloat3("scale", &transform_.scale.x, 0.01f);
	ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.01f);
	ImGui::DragFloat3("translate", &transform_.translate.x, 0.01f);
	ImGui::End();
}