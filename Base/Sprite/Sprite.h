#pragma once

// C++
#include <d3d12.h>

// MyClass
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Base/PSO/PipelineManager/PipelineManager.h"
#include "Base/WinApp/WinApp.h"
#include "lib/Math/MyMath.h"

struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
};

struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

class Sprite {

public:
	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};

	Sprite() = default;
	~Sprite() = default;

	void Init(DirectXCommon* dxCommon, PipelineManager* pipelineManager);

	void Update();

	void Draw();

	void PreDraw();

private:
	// DxCommon
	DirectXCommon* dxCommon_ = nullptr;
	// パイプライン
	PipelineManager* pipelineManager_ = nullptr;
	// ウィンドウズアプリケーション
	WinApp* winApp_ = nullptr;

	// バッファーリソース
	ComPtr<ID3D12Resource> vertexResource;
	ComPtr<ID3D12Resource> indexResource;
	// バッファーリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;
	uint32_t* indexData = nullptr;
	// バッファーリソースの使い道を補足するバッファービュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;

	// マテリアルリソース
	ComPtr<ID3D12Resource> materialResource;
	// マテリアルリソース内のデータを指すポインタ
	Material* materialData = nullptr;
	// 座標変換用
	// Sprite用のTransformationMatrix用のリソースを作る
	ComPtr<ID3D12Resource> transformationMatrixResource;
	TransformationMatrix* transformationMatrixData = nullptr;

	Transform transform;
	Transform cameraTransform;

	Transform uvTransform;

	// RTV
	ID3D12DescriptorHeap* rtvDescriptorHeap;
	// SRV用のヒープでディスクリプタの数は128
	ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;
};