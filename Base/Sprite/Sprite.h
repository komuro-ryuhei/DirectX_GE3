#pragma once

#include <d3d12.h>

// MyClass
#include "DirectXCommon.h"
#include "WinApp.h"
#include "SpriteCommon.h"
#include "TextureManager.h"
#include "Vector.h"
#include "Matrix.h"
#include "MyMath.h"
#include "ComPtr.h"
#include "Mesh.h"

// 前方宣言
class SpriteCommon;

struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

//struct Material {
//	Vector4 color;
//	int32_t enableLighting;
//	float padding[3];
//	Matrix4x4 uvTransform;
//};

struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
};

/// <summary>
/// スプライト
/// </summary>
class Sprite {

public:

	// 初期化
	void Initialize(SpriteCommon* spriteCommon, DirectXCommon* dxCommon, std::string textureFilePath);

	// 更新
	void Update();

	// 描画
	void Draw();


private: //メンバ変数

	// DirectX
	DirectXCommon* dxCommon_ = nullptr;
	// ウィンドウズアプリケーション
	WinApp* winApp_ = nullptr;

	// スプライト共通部
	SpriteCommon* spriteCommon_ = nullptr;

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

	// DescriptorSize
	uint32_t descriptorSizeSRV;
	// RTV
	ID3D12DescriptorHeap* rtvDescriptorHeap;
	// SRV用のヒープでディスクリプタの数は128
	ID3D12DescriptorHeap* srvDescriptorHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;

	// テクスチャ番号
	uint32_t textureIndex = 0;

	const std::string uvTexture = "./Resources/uvChecker.png";
};