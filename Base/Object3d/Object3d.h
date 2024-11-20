#pragma once

// C++
#include "fstream"
#include "sstream"
#include <d3d12.h>
#include <vector>

// MyClass
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Base/PSO/PipelineManager/PipelineManager.h"
#include "Base/TextureManager/TextureManager.h"
#include "Base/WinApp/WinApp.h"
#include "lib/Math/MyMath.h"
#include "Base/Model/Model.h"

struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
};

struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

// 3Dオブジェクト
class Object3d {

public: // メンバ関数
	Object3d() = default;
	~Object3d() = default;

	void Init(DirectXCommon* dxCommon);

	void Update();

	void Draw();

	// void PreDraw();

private:
	// DxCommon
	DirectXCommon* dxCommon_ = nullptr;
	// パイプライン
	PipelineManager* pipelineManager_ = nullptr;
	// ウィンドウズアプリケーション
	WinApp* winApp_ = nullptr;

	// モデル
	//Model* model_ = nullptr;

	std::unique_ptr<Model> model_ = nullptr;

	// 座標変換用
	// Sprite用のTransformationMatrix用のリソースを作る
	ComPtr<ID3D12Resource> transformationMatrixResource;
	TransformationMatrix* transformationMatrixData = nullptr;


	Transform transform;
	Transform cameraTransform;
};