#pragma once

// C++
#include "fstream"
#include "sstream"
#include <d3d12.h>
#include <vector>

// MyClass
#include "Engine/Base/DirectXCommon/DirectXCommon.h"
#include "Engine/Base/PSO/PipelineManager/PipelineManager.h"
#include "Engine/Base/TextureManager/TextureManager.h"
#include "Engine/Base/ModelManager/ModelManager.h"
#include "Engine/Base/WinApp/WinApp.h"
#include "Engine/lib/Math/MyMath.h"
#include "Engine/Base/3d/Model/Model.h"
#include "Engine/Base/Camera/Camera.h"
#include "struct.h"

// 3Dオブジェクト
class Object3d {

public: // メンバ関数
	Object3d() = default;
	~Object3d() = default;

	void Init(DirectXCommon* dxCommon);

	void Update();

	void Draw();

	// void PreDraw();

	// setter
	void SetModel(const std::string& filePath);
	void SetCamera(Camera* camera);
	void SetDefaultCamera(Camera* camera);

	// getter
	Camera* GetDefaultCamera() const;

private:
	// DxCommon
	DirectXCommon* dxCommon_ = nullptr;
	// パイプライン
	PipelineManager* pipelineManager_ = nullptr;
	// ウィンドウズアプリケーション
	WinApp* winApp_ = nullptr;

	// モデル
	//Model* model_ = nullptr;

	Model* model_ = nullptr;

	Camera* camera_ = nullptr;
	Camera* defaultCamera_ = nullptr;

	// 座標変換用
	// Sprite用のTransformationMatrix用のリソースを作る
	ComPtr<ID3D12Resource> transformationMatrixResource;
	TransformationMatrix* transformationMatrixData = nullptr;


	Transform transform;
	Transform cameraTransform;
};