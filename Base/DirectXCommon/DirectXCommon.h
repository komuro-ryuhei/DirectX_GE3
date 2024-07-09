#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>

#include <cassert>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include "ComPtr.h"
#include "WinApp.h"
#include "Compiler.h"

/// <summary>
/// DirectXCommon
/// </summary>
class DirectXCommon {
public: // 静的メンバ変数

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static DirectXCommon* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp);

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	/// <summary>
	/// レンダーターゲットのクリア
	/// </summary>
	void CrearRenderTargets();


	/// <summary>
	/// getter
	/// </summary>
	ID3D12Device* GetDevice() const;

	ID3D12GraphicsCommandList* GetCommandList() const;


	D3D12_VIEWPORT GetViewPort() const;

	D3D12_RECT GetScissor() const;

private: // メンバ変数

	// ウィンドウサイズ
	uint32_t kWindowWidth_;
	uint32_t kWindowHeight_;

	// ウィンドウズアプリケーション
	WinApp* winApp_;


	// DiretcX
	ComPtr<IDXGIFactory7> dxgiFactory_;
	ComPtr<IDXGIAdapter4> useAdapter_;
	ComPtr<ID3D12Device> device_;
	// command
	ComPtr<ID3D12CommandQueue> commandQueue_;
	ComPtr<ID3D12CommandAllocator> commandAllocator_;
	ComPtr<ID3D12GraphicsCommandList> commandList_;

	// スワップチェイン
	ComPtr<IDXGISwapChain4> swapChain_;
	ComPtr<ID3D12Resource> swapChainResources[2] = { nullptr };

	// ディスクリプターヒープ
	ComPtr<ID3D12DescriptorHeap> rtvHeap_;

	// RTVを2つ作るのでディスクリプタを2つ用意
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];

	ComPtr<ID3D12InfoQueue> infoQueue_ = nullptr;

	ComPtr<ID3D12Fence> fence_;
	uint64_t fenceValue = 0;
	HANDLE fenceEvent;

	D3D12_RESOURCE_BARRIER barrier{};


	// ビューポート
	D3D12_VIEWPORT viewPort{};
	// シザー矩形
	D3D12_RECT scissorRect{};

private: // メンバ関数

	DirectXCommon() = default;
	~DirectXCommon() = default;
	DirectXCommon(const DirectXCommon&) = delete;
	const DirectXCommon& operator=(const DirectXCommon&) = delete;

	/// <summary>
	/// デバイス関連初期化
	/// </summary>
	void InitializeDXGIDevice();

	/// <summary>
	/// コマンド関連初期化
	/// </summary>
	void InitializeCommand();

	/// <summary>
	/// スワップチェーンの作成
	/// </summary>
	void CreateSwapChain();

	/// <summary>
	/// ディスクリプターヒープの生成
	/// </summary>
	ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
		ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heaptype, UINT numDescriptors, bool shaderVisible);

	/// <summary>
	/// レンダーターゲットの生成
	/// </summary>
	void CreateRenderTargets();

	/// <summary>
	/// バリアを張る
	/// </summary>
	void TransitionBarrier();

	/// <summary>
	/// フェンスの作成
	/// </summary>
	void CreateFence();

	/// <summary>
	/// シザリング矩形の初期化
	/// </summary>
	void InitializeScissoringRect();
};