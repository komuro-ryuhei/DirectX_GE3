#pragma once

#include <d3d12.h>

#include "lib/Vector/Vector.h"

// MyClass
#include "lib/ComPtr/ComPtr.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "lib/Math/MyMath.h"

/// <summary>
/// メッシュ
/// </summary>
class Mesh {
public:

	// 前方宣言
	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};

	struct DirectionalLight {
		Vector4 color;
		Vector3 direction;
		float intensity;
	};

	ComPtr<ID3D12Resource> CreateVertexResource(DirectXCommon* dXCommon, size_t sizeInBytes);

	void CreateVertexBufferView();

	void CreateMaterialResource();

	void WriteDateForResource();

	void LightSetting(DirectXCommon* dXCommon);

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

	// Light用のマテリアルリソースを作る
	ComPtr<ID3D12Resource> materialResourceLight;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewLight{};

	DirectionalLight* directionalLightData = nullptr;
};