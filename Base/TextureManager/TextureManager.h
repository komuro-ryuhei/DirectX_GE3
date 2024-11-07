#pragma once

// C++
#include <DirectXTex.h>
#include <d3d12.h>
#include <string>

// MyClass
#include "ComPtr.h"
#include "DirectXCommon.h"

// テクスチャマネージャー
class TextureManager {

public:

	// シングルトンインスタンスの取得
	static TextureManager* GetInstance();

	void Init();
	// 終了処理
	void Finalize();

public:
	/// <summary>
	/// テクスチャファイルの読み込み
	/// </summary>
	void LoadTexture(ID3D12Device* device, ID3D12DescriptorHeap* srvDescriptorHeap, const std::string& filePath);

	/// <summary>
	/// テクスチャ作成
	/// </summary>
	ComPtr<ID3D12Resource> CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);

	// SRVインデックスの開始番号
	uint32_t GetTextureIndexByFilePath(const std::string& filePath);

	/// <summary>
	/// ハンドル取得
	/// </summary>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);

	// テクスチャ番号からGPUハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(uint32_t textureIndex);

public:
	static TextureManager* instance;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = delete;
	TextureManager& operator=(TextureManager&) = delete;

private:
	// テクスチャ1枚分のデータ
	struct TextureData {
		std::string filePath;
		DirectX::TexMetadata metaData;
		ComPtr<ID3D12Resource> resource;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

	// テクスチャデータ
	std::vector<TextureData> textureDatas;

	// SRVインデックスの開始番号
	static uint32_t kSRVIndexTop_;
};