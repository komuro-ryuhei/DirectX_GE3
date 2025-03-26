#include "Mesh.h"

#include "externals/imgui/imgui.h"

D3D12_VERTEX_BUFFER_VIEW Mesh::GetVBV() const { return vertexBufferView; }

ID3D12Resource* Mesh::GetMateialResource() const { return materialResource_.Get(); }

ID3D12Resource* Mesh::GetLightResource() const { return materialResourceLight.Get(); }

ComPtr<ID3D12Resource> Mesh::CreateVertexResource(DirectXCommon* dxCommon, size_t sizeInBytes) {
	HRESULT hr;

	dxCommon_ = dxCommon;

	// 頂点リソース用のヒープの設定
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	// 頂点リソースの設定
	// バッファリソース
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeInBytes; // リソースのサイズ。今回はVector4を3頂点分
	// バッファの場合はこれらは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 実際に頂点リソースを作る
	hr = dxCommon_->GetDevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource));
	assert(SUCCEEDED(hr));

	return vertexResource;
}

void Mesh::CreateVertexBufferView() {

	// リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(Vector4) * 3;
	// 1頂点当あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(Vector4);
}

void Mesh::CreateMaterialResource() {

	materialResource_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(Vector4));
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = Vector4(1.0, 1.0f, 1.0f, 1.0f);
}

void Mesh::WriteDateForResource() {

	// 頂点リソースにデータを書き込む
	Vector4* vertexData = nullptr;
	// 書き込むアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	// 左下
	vertexData[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
	// 上
	vertexData[1] = {0.0f, 0.5f, 0.0f, 1.0f};
	// 右下
	vertexData[2] = {0.5f, -0.5f, 0.0f, 1.0f};

	CreateMaterialResource();
}

void Mesh::LightSetting(DirectXCommon* dXCommon) {

	dxCommon_ = dXCommon;

	// Light用のマテリアルリソースを作る
	materialResourceLight = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(DirectionalLight));

	// リソース用の先頭のアドレスから使う
	vertexBufferViewLight.BufferLocation = materialResourceLight->GetGPUVirtualAddress();

	// 使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferViewLight.SizeInBytes = sizeof(DirectionalLight);
	// 1頂点当たりのサイズ
	vertexBufferViewLight.StrideInBytes = sizeof(DirectionalLight);

	materialResourceLight->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));

	directionalLightData->color = {1.0f, 1.0f, 1.0f, 1.0f};
	directionalLightData->direction = {0.0f, -1.0f, 0.0f};
	directionalLightData->intensity = 1.0f;
}

void Mesh::ImGuiDebug() {

	ImGui::Begin("Light");

	ImGui::ColorEdit4("LightColor", &directionalLightData->color.x);
	ImGui::DragFloat3("direction", &directionalLightData->direction.x, 0.01f);
	ImGui::DragFloat("intencity", &directionalLightData->intensity, 0.01f);

	ImGui::End();
}