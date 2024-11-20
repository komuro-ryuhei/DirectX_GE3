#include "Object3d.h"

void Object3d::Init(DirectXCommon* dxCommon) {

	dxCommon_ = dxCommon;

	model_ = std::make_unique<Model>();
	model_->Init(dxCommon_);

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

	transform.rotate.y += 0.01f;

	Matrix4x4 worldMatrix = MyMath::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MyMath::MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = MyMath::Inverse4x4(cameraMatrix);
	Matrix4x4 projectionMatrix = MyMath::MakePerspectiveFovMatrix(0.45f, float(winApp_->GetWindowWidth()) / float(winApp_->GetWindowHeight()), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = MyMath::Multiply(worldMatrix, MyMath::Multiply(viewMatrix, projectionMatrix));
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