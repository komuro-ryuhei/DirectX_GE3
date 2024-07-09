#include "System.h"

#include "DirectXCommon.h"
#include "WinApp.h"
#include "PipelineManager.h"
#include "Mesh.h"
#include "Input.h"

#include "Logger.h"

#include <cstdint>
#include <string>
#include <format>
#include <cassert>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "dxguid.lib")

/*==================================================================================*/
// システム全体の初期化

std::unique_ptr<Mesh> mesh_ = nullptr;

// WindowsAPI
WinApp* winApp_ = nullptr;
// DirectXCommon
DirectXCommon* dxCommon_ = nullptr;
// Pipeline
PipelineManager* pipelineManager_ = nullptr;
// Input
Input* input_ = nullptr;

void System::Initialize(const char* title, int width, int height) {

	winApp_ = WinApp::GetInstance();
	
	mesh_ = std::make_unique<Mesh>();

	// ゲームウインドウの作成
	std::string titleWithVersion = std::string(title);
	auto&& titleString = StringUtility::ConvertString(titleWithVersion);
	winApp_->CreateGameWindow(
		titleString.c_str(), WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME), width,
		height);

	// DirectXの初期化処理
	dxCommon_ = DirectXCommon::GetInstance();
	dxCommon_->Initialize(winApp_);

	// piprlineの初期化
	pipelineManager_ = PipelineManager::GetInstance();
	pipelineManager_->PSOSetting(dxCommon_);

	// Inputの初期化
	input_ = Input::GetInstance();
	input_->Initialize(winApp_);
}

bool System::ProcessMesage() { return winApp_->ProcessMessage(); }

void System::BeginFrame() {

	// DirectX描画前処理
	dxCommon_->PreDraw();

	input_->Update();
}

void System::EndFrame() {

	// DirectX描画終了
	dxCommon_->PostDraw();
}

void System::CreateMesh() {

	mesh_->CreateVertexResource(dxCommon_, sizeof(Vector4) * 3);
	mesh_->CreateVertexBufferView();
	mesh_->WriteDateForResource();
}

void System::DrawTriangle() {

	CreateMesh();

	ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();

	D3D12_VIEWPORT viewPort = dxCommon_->GetViewPort();
	D3D12_RECT scissor = dxCommon_->GetScissor();

	commandList->RSSetViewports(1, &viewPort); // Viewportを設定
	commandList->RSSetScissorRects(1, &scissor); // Scissorを設定

	commandList->SetGraphicsRootSignature(pipelineManager_->GetRootsignature());
	commandList->SetPipelineState(pipelineManager_->GetGraphicsPipelineState());
	D3D12_VERTEX_BUFFER_VIEW vbv = mesh_->GetVBV();
	commandList->IASetVertexBuffers(0, 1, &vbv);

	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	commandList->DrawInstanced(3, 1, 0, 0);
}