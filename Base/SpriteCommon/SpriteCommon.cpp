#include "SpriteCommon.h"

void SpriteCommon::Initialize(DirectXCommon* dxCommon) {

	dxCommon_ = dxCommon;

	pipelineManager_ = std::make_unique<PipelineManager>();
	/*rootSignature_ = std::make_unique<RootSignature>();
	inputLayout_ = std::make_unique<InputLayout>();
	rasterizer_ = std::make_unique<RasterizerState>();*/

	CreateGraphicsPipeline();
}

void SpriteCommon::CreateRootSignature() {

	rootSignature_->Create(dxCommon_);
}

void SpriteCommon::CreateInputLayout() {

	inputLayout_->Setting();
}

void SpriteCommon::CreateRasterizer() {

	rasterizer_->Setting();
}

void SpriteCommon::CreateGraphicsPipeline() {
	
	/*CreateRootSignature();

	CreateInputLayout();

	CreateRasterizer();*/


	pipelineManager_->PSOSetting(dxCommon_);
}

void SpriteCommon::PreDraw() {

	ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();

	// ルートシグネチャをセット
	commandList->SetGraphicsRootSignature(pipelineManager_->GetRootSignature());
	// グラフィックスパイプラインステートをセット
	commandList->SetPipelineState(pipelineManager_->GetGraphicsPipelineState());

	// プリミティブトポロジーをセット
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}