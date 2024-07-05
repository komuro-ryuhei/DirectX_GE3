#include "PipelineManager.h"

ID3D12RootSignature* PipelineManager::GetRootsignature() const { return rootSignature_->GetRootSignature(); }

ID3D12PipelineState* PipelineManager::GetGraphicsPipelineState() const { return graphicsPipelineState.Get(); }

PipelineManager* PipelineManager::GetInstance() {
	static PipelineManager instance;
	return &instance;
}

void PipelineManager::ShaderCompile() {

	// Shaderをコンパイルする
	vsBlob = compiler_->CompileShader(L"Object3D.VS.hlsl",
		L"vs_6_0", compiler_->GetDxcUtils(), compiler_->GetCompiler(), compiler_->GetIncludeHandler());
	assert(vsBlob != nullptr);

	psBlob = compiler_->CompileShader(L"Object3D.PS.hlsl",
		L"ps_6_0", compiler_->GetDxcUtils(), compiler_->GetCompiler(), compiler_->GetIncludeHandler());
	assert(psBlob != nullptr);
}

void PipelineManager::CreatePSO(DirectXCommon* dXCommon) {
	HRESULT hr;

	graphicsPipelineStateDesc.pRootSignature = rootSignature_->GetRootSignature(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayout_->GetInputLayout(); // InputLayout
	graphicsPipelineStateDesc.VS = { vsBlob->GetBufferPointer(),
	vsBlob->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc.PS = { psBlob->GetBufferPointer(),
	psBlob->GetBufferSize() }; // PixelShader
	graphicsPipelineStateDesc.BlendState = blendState_->GetBlendDesc(); // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizer_->GetRasterizerDesc(); // RasterizerState

	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	// 利用するトポロジ(形状)のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// どのように画面に色を打ち込むのか設定
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// 実際に生成
	hr = dXCommon->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(hr));
}

void PipelineManager::PSOSetting(DirectXCommon* dXCommon) {

	compiler_->Initialize();

	ShaderCompile();

	rootSignature_->Create(dXCommon);

	inputLayout_->Setting();

	rasterizer_->Setting();

	blendState_->Setting();

	CreatePSO(dXCommon);
}