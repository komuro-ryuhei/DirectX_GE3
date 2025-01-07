#include "TextureManager.h"

TextureManager* TextureManager::instance = nullptr;
uint32_t TextureManager::kSRVIndexTop_ = 1;

TextureManager* TextureManager::GetInstance() {
	if (instance == nullptr) {
		instance = new TextureManager;
	}
	return instance;
}

void TextureManager::Init(SrvManager* srvManager) {

	srvManager_ = srvManager;

	// SRVの数と同様
	textureDatas.reserve(DirectXCommon::kMaxSRVCount);
}

void TextureManager::Finalize() {

	delete instance;
	instance = nullptr;
}

void TextureManager::LoadTexture(DirectXCommon* dxCommon, const std::string& filePath) {

	// 読み込み済みテクスチャを検索
	// auto it = std::find_if(textureDatas.begin(), textureDatas.end(), [&](TextureData& textureData) { return textureData.filePath == filePath; });
	// if (it != textureDatas.end()) {
	//	// 読み込み済みなら早期リターン
	//	return;
	//}

	if (textureDatas.contains(filePath)) {
		return;
	}

	// テクスチャ枚数上限チェック
	// assert(textureDatas.size() + kSRVIndexTop_ < DirectXCommon::kMaxSRVCount);

	assert(srvManager_->CanAllocate());

	// テクスチャデータ読み込み
	// textureDatas.reserve(textureDatas.size() + 1);
	// textureDatas.emplace_back();
	// 追加したテクスチャデータの参照を取得する
	TextureData& textureData = textureDatas[filePath];

	// テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = StringUtility::ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	// ミニマップの作成
	DirectX::ScratchImage mipImage{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImage);
	assert(SUCCEEDED(hr));

	textureData.filePath = filePath;
	textureData.metaData = mipImage.GetMetadata();
	textureData.resource = CreateTextureResource(dxCommon->GetDevice(), textureData.metaData);

	textureData.srvIndex = srvManager_->Allocate();
	textureData.srvHandleCPU = srvManager_->GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.srvHandleGPU = srvManager_->GetGPUDescriptorHandle(textureData.srvIndex);

	// テクスチャデータの要素番号をSRVのインデックスとする
	// uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size() - 1) + kSRVIndexTop_;

	// textureData.srvHandleCPU = GetCPUDescriptorHandle(dxCommon->GetSrvDescriptorHeap(), dxCommon->GetDescriptorSizeSRV(), srvIndex);
	// textureData.srvHandleGPU = GetGPUDescriptorHandle(dxCommon->GetSrvDescriptorHeap(), dxCommon->GetDescriptorSizeSRV(), srvIndex);

	// D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	// srvDesc.Format = textureData.metaData.format;
	// srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	// srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	// srvDesc.Texture2D.MipLevels = UINT(textureData.metaData.mipLevels);

	// dxCommon->GetDevice()->CreateShaderResourceView(textureData.resource.Get(), &srvDesc, textureData.srvHandleCPU);

	srvManager_->CreateSRVforTexture2D(textureData.srvIndex, textureData.resource.Get(), textureData.metaData.format, static_cast<UINT>(textureData.metaData.mipLevels));

	UploadTextureData(textureData.resource.Get(), mipImage);

	textureData.srvIndex++;
}

ComPtr<ID3D12Resource> TextureManager::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata) {

	// metadaraを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);                             // Textureの幅
	resourceDesc.Height = UINT(metadata.height);                           // Textureの高さ
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);                   // mipmapの数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);            // 奥行き or 配列Textureの配列
	resourceDesc.Format = metadata.format;                                 // TextureのFormat
	resourceDesc.SampleDesc.Count = 1;                                     // サンプリングカウント
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension); // Textureの次元数

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;                        // 細かい設定を行う
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK; // WriteBackポリシーでCPUアクセス可能
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;          // プロセッサの近くに配置

	// Resourceの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
	    &heapProperties,                   // Heapの設定
	    D3D12_HEAP_FLAG_NONE,              // Heapの特殊な設定
	    &resourceDesc,                     // Resourceの設定
	    D3D12_RESOURCE_STATE_GENERIC_READ, // 初回のResourceState
	    nullptr,                           // Clear最速値
	    IID_PPV_ARGS(&resource));          // 作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));
	return resource;
}

void TextureManager::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages) {

	// Meta情報を取得
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	// 全MipMapについて
	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; ++mipLevel) {
		// MipMapLevelを指定して各Imageを取得
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		// Textureに転送
		HRESULT hr = texture->WriteToSubresource(
		    UINT(mipLevel),
		    nullptr,              // 全領域へコピー
		    img->pixels,          // 元データへアクセス
		    UINT(img->rowPitch),  // 1ラインサイズ
		    UINT(img->slicePitch) // 1枚サイズ
		);
		assert(SUCCEEDED(hr));
	}
}

uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& filePath) {

	// テクスチャが存在するかをチェック
	if (textureDatas.contains(filePath)) {
		// 存在する場合、そのテクスチャのインデックスを返す
		return textureDatas[filePath].srvIndex;
	}

	// 存在しない場合
	assert(0);
	return 0;
}

D3D12_CPU_DESCRIPTOR_HANDLE TextureManager::GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index) {

	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);

	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index) {

	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);

	return handleGPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(const std::string& filePath) {

	// 範囲外指定違反チェック
	assert(textureDatas.contains(filePath));

	TextureData& textureData = textureDatas[filePath];

	return textureData.srvHandleGPU;
}

const DirectX::TexMetadata& TextureManager::GetMetaData(const std::string& filePath) {

	// 範囲外指定違反チェック
	assert(textureDatas.contains(filePath));

	TextureData& textureData = textureDatas[filePath];

	return textureData.metaData;
}

uint32_t TextureManager::GetSrvIndex(const std::string& filePath) {

	// 指定されたファイルパスが存在するかチェック
	assert(textureDatas.contains(filePath));

	// 該当テクスチャデータを取得
	TextureData& textureData = textureDatas[filePath];
	return textureData.srvIndex;
}
