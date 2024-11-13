
#pragma once

#include "Base/PSO/Compiler/Compiler.h"

// MyClass
#include "Base/PSO/BlendState/BlendState.h"
#include "Base/PSO/Inputlayout/InputLayout.h"
#include "Base/PSO/RootSignature/RootSignature.h"
#include "Base/PSO/RasterizerState/RasterizerState.h"
#include "Base/PSO/BlendState/BlendState.h"

#include "lib/ComPtr/ComPtr.h"
#include "Base/DirectXCommon/DirectXCommon.h"

/// <summary>
/// パイプライン管理
/// </summary>
class PipelineManager {
public:
	PipelineManager() = default;
	~PipelineManager() = default;

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static PipelineManager* GetInstance();

	/// <summary>
	/// ShaderをCompile
	/// </summary>
	void ShaderCompile();

	/// <summary>
	/// PSOを生成
	/// </summary>
	void CreatePSO(DirectXCommon* dXCommon);

	void PSOSetting(DirectXCommon* dXCommon);

	ID3D12RootSignature* GetRootSignature() const;
	ID3D12PipelineState* GetGraphicsPipelineState() const;

private:
	std::unique_ptr<Compiler> compiler_ = std::make_unique<Compiler>();
	std::unique_ptr<RootSignature> rootSignature_ = std::make_unique<RootSignature>();
	std::unique_ptr<InputLayout> inputLayout_ = std::make_unique<InputLayout>();
	std::unique_ptr<RasterizerState> rasterizer_ = std::make_unique<RasterizerState>();
	std::unique_ptr<BlendState> blendState_ = std::make_unique<BlendState>();

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;

	ComPtr<IDxcBlob> vsBlob;
	ComPtr<IDxcBlob> psBlob;

private:
	PipelineManager(const PipelineManager&) = delete;
	const PipelineManager& operator=(const PipelineManager&) = delete;
};
