#pragma once

#include "Compiler.h"
#include "RootSignature.h"
#include "Inputlayout.h"
#include "RasterizerState.h"
#include "BlendState.h"

#include "DirectXCommon.h"
#include "ComPtr.h"

/// <summary>
/// パイプライン管理
/// </summary>
class PipelineManager {
public:

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


	ID3D12RootSignature* GetRootsignature() const;
	ID3D12PipelineState* GetGraphicsPipelineState() const;

private:
	Compiler* compiler_ = new Compiler();
	RootSignature* rootSignature_ = new RootSignature();
	InputLayout* inputLayout_ = new InputLayout();
	RasterizerState* rasterizer_ = new RasterizerState();
	BlendState* blendState_ = new BlendState();

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;

	ComPtr<IDxcBlob> vsBlob;
	ComPtr<IDxcBlob> psBlob;

private:

	PipelineManager() = default;
	~PipelineManager() = default;
	PipelineManager(const PipelineManager&) = delete;
	const PipelineManager& operator=(const PipelineManager&) = delete;
};