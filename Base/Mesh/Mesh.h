#pragma once

#include <d3d12.h>

#include "Vector.h"

#include "DirectXCommon.h"
#include "ComPtr.h"

/// <summary>
/// メッシュ
/// </summary>
class Mesh {
public:

	ComPtr<ID3D12Resource> CreateVertexResource(DirectXCommon* dXCommon, size_t sizeInBytes);

	void CreateVertexBufferView();

	void WriteDateForResource();


	D3D12_VERTEX_BUFFER_VIEW GetVBV() const;

private:

	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	ComPtr<ID3D12Resource> vertexResource = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
};

