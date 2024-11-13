
#pragma once

#include <d3d12.h>

#include "lib/ComPtr/ComPtr.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "lib/Logger/Logger.h"

class RootSignature {
public:
	/// <summary>
	/// 生成
	/// </summary>
	void Create(DirectXCommon* dxCommon);

	// getter
	ID3D12RootSignature* GetRootSignature() const;

private:
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	ComPtr<ID3DBlob> signatureBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;

	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
};