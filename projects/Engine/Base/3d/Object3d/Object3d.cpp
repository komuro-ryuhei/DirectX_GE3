#include "Object3d.h"
#include "Engine/lib/Logger/Logger.h"

#include "externals/imgui/imgui.h"

void Object3d::Init(DirectXCommon* dxCommon) {

	camera_ = defaultCamera_;

	dxCommon_ = dxCommon;

	/*model_ = std::make_unique<Model>();
	model_->Init(dxCommon_);*/

	// 座標変換用
	transformationMatrixResource = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(TransformationMatrix));
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));
	// 単位行列を書き込む
	transformationMatrixData->WVP = MyMath::MakeIdentity4x4();
	transformationMatrixData->World = MyMath::MakeIdentity4x4();

	transform = {
	    {1.0f, 1.0f, 1.0f},
	    {0.0f, 0.0f, 0.0f},
	    {0.0f, 0.0f, 0.0f},
	};

	cameraTransform = {
	    {1.0f, 1.0f, 1.0f  },
	    {0.3f, 0.0f, 0.0f  },
	    {0.0f, 4.0f, -10.0f},
	};
}

void Object3d::Update() {

	Matrix4x4 worldMatrix = MyMath::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 projectionMatrix = MyMath::MakePerspectiveFovMatrix(0.45f, float(winApp_->GetWindowWidth()) / float(winApp_->GetWindowHeight()), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix/* = MyMath::Multiply(worldMatrix, MyMath::Multiply(viewMatrix, projectionMatrix))*/;
	if (defaultCamera_) {
		const Matrix4x4& viewProjectionMatrix = defaultCamera_->GetViewProjectionMatrix();
		worldViewProjectionMatrix = MyMath::Multiply(worldMatrix, viewProjectionMatrix);
	} else {
		worldViewProjectionMatrix = worldMatrix;
	}
	transformationMatrixData->WVP = worldViewProjectionMatrix;
	transformationMatrixData->World = worldMatrix;
}

void Object3d::Draw() {

	ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();

	// TransformationMatrixCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());

	if (model_) {
		// 
		model_->Draw();
	}
}

void Object3d::ImGuiDebug() {

	//
	ImGui::Begin("model");

	ImGui::DragFloat3("rotate", &transform.rotate.x, 0.1f);
	ImGui::DragFloat3("translate", &transform.translate.x, 1.0f);

	ImGui::End();
}

void Object3d::SetModel(const std::string& filePath) {

	// モデルを検索してセットする
	model_ = ModelManager::GetInstance()->FindModel(filePath);
}

void Object3d::SetCamera(Camera* camera) { camera_ = camera; }

void Object3d::SetDefaultCamera(Camera* camera) { defaultCamera_ = camera; }

Camera* Object3d::GetDefaultCamera() const { return defaultCamera_; }