#pragma once

#include <d3d12.h>

#include "Vector.h"

// MyClass
#include "ComPtr.h"
#include "DirectXCommon.h"
#include "lib/Math/MyMath.h"

// 前方宣言
struct Material {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};

/// <summary>
/// メッシュ
/// </summary>
class Mesh {
public:
	ComPtr<ID3D12Resource> CreateVertexResource(DirectXCommon* dXCommon, size_t sizeInBytes);

	void CreateVertexBufferView();

	void CreateMaterialResource();

	void WriteDateForResource();

	D3D12_VERTEX_BUFFER_VIEW GetVBV() const;
	ID3D12Resource* GetMateialResource() const;

private:
	DirectXCommon* dxCommon_;

	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	ComPtr<ID3D12Resource> vertexResource = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	ComPtr<ID3D12Resource> materialResource_;
	// マテリアルリソース内のデータを指すポインタ
	Material* materialData = nullptr;
};