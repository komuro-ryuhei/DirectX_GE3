#include "Sprite.h"

void Sprite::Initialize(SpriteCommon* spriteCommon, DirectXCommon* dxCommon, std::string textureFilePath) {

	winApp_ = new WinApp();

	// 引数で受け取ってメンバ変数に記録
	this->spriteCommon_ = spriteCommon;
	dxCommon_ = dxCommon;

	spriteCommon_->Initialize(dxCommon_);

	// リソース作成
	vertexResource = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(VertexData) * 6);
	indexResource = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(uint32_t) * 6);
	// VertexBufferViewを設定
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress(); // 先頭のアドレスを使用
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 6;                    // 6頂点のサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);                      // 1頂点のサイズ
	// IndexBufferViewを設定
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress(); // 先頭のアドレスを使用
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;                     // 6頂点のサイズ
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	// Resourceに書き込むためのアドレスをDataに割り当てる
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	// マテリアルリソース作成
	materialResource = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(Material));
	// Resourceに書き込むためのアドレスをDataに割り当てる
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	// マテリアルの初期値を書き込む
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData->enableLighting = false;
	materialData->uvTransform = MyMath::MakeIdentity4x4();
	// 座標変換用
	transformationMatrixResource = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(TransformationMatrix));
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));
	// 単位行列を書き込む
	transformationMatrixData->WVP = MyMath::MakeIdentity4x4();
	transformationMatrixData->World = MyMath::MakeIdentity4x4();

	transform = {
	    {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        {0.0f, 1.0f, 1.0f}
    };
	cameraTransform = {
	    {1.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f}
    };

	// SRV
	descriptorSizeSRV = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvDescriptorHeap = dxCommon_->CreateDescriptorHeap(dxCommon_->GetDevice(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, descriptorSizeSRV, true);

	textureSrvHandleCPU = dxCommon_->GetCPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 1);
	textureSrvHandleGPU = dxCommon_->GetGPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 1);

	// Textureを読んで転送する
	DirectX::ScratchImage mipImages = TextureManager::GetInstance()->LoadTexture(dxCommon_->GetDevice(), srvDescriptorHeap, uvTexture);
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	Microsoft::WRL::ComPtr<ID3D12Resource> textureResource = dxCommon_->CreateTextureResource(dxCommon_->GetDevice(), metadata);
	dxCommon_->UploadTextureData(textureResource.Get(), mipImages);

	// metaDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	// SRVの生成
	dxCommon_->GetDevice()->CreateShaderResourceView(textureResource.Get(), &srvDesc, textureSrvHandleCPU);

	// 
	textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);
}

void Sprite::Update() {

	// ヴァーテックスリソースにデータを書き込む
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	vertexData[0].position = {0.0f, 360.0f, 0.0f, 1.0f}; // 左下
	vertexData[0].texcoord = {0.0f, 1.0f};
	vertexData[0].normal = {0.0f, 0.0f, -1.0f};

	vertexData[1].position = {0.0f, 0.0f, 0.0f, 1.0f}; // 左上
	vertexData[1].texcoord = {0.0f, 0.0f};
	vertexData[1].normal = {0.0f, 0.0f, -1.0f};

	vertexData[2].position = {640.0f, 360.0f, 0.0f, 1.0f}; // 右下
	vertexData[2].texcoord = {1.0f, 1.0f};
	vertexData[2].normal = {0.0f, 0.0f, -1.0f};

	vertexData[3].position = {640.0f, 0.0f, 0.0f, 1.0f}; // 右上
	vertexData[3].texcoord = {1.0f, 0.0f};
	vertexData[3].normal = {0.0f, 0.0f, -1.0f};

	// インデックスリソースにデータを書き込む
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;

	indexData[3] = 1;
	indexData[4] = 3;
	indexData[5] = 2;

	Matrix4x4 worldMatrix = MyMath::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 viewMatrix = MyMath::MakeIdentity4x4();
	Matrix4x4 projectionMatrix = MyMath::MakeOrthographicMatrix(0.0f, 0.0f, float(winApp_->GetWidth()), float(winApp_->GetHeight()), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = MyMath::Multiply(worldMatrix, MyMath::Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData->WVP = worldViewProjectionMatrix;
	transformationMatrixData->World = worldMatrix;
}

void Sprite::Draw() {

	ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();

	// 描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = {srvDescriptorHeap};
	commandList->SetDescriptorHeaps(1, descriptorHeaps);

	spriteCommon_->PreDraw();

	// マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	// VBVを設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
	// IBVを設定
	commandList->IASetIndexBuffer(&indexBufferView);
	// TransformationMatrixCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());
	commandList->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(textureIndex));
	// Spriteの描画
	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}