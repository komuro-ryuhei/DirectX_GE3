#include "Sprite.h"

#include "imgui/imgui.h"

// getter
const Vector2& Sprite::GetPosition() const { return position_; }
float Sprite::GetRotation() const { return rotation_; }
const Vector4& Sprite::GetColor() const { return materialData->color; }
const Vector2& Sprite::GetSize() const { return size_; }
const Vector2& Sprite::GetAnchorPoint() const { return anchorPoint_; }
const bool& Sprite::GetIsFilpX() const { return isFlipX_; }
const bool& Sprite::GetIsFilpY() const { return isFlipY_; }
const Vector2& Sprite::GetTextureLeftTop() const { return textureLeftTop_; }
const Vector2& Sprite::GetTextureSize() const { return textureSize_; }
// setter
void Sprite::SetPosition(const Vector2& position) { position_ = position; }
void Sprite::SetRotation(float rotation) { rotation_ = rotation; }
void Sprite::SetColor(const Vector4& color) { materialData->color = color; }
void Sprite::SetSize(const Vector2& size) { size_ = size; }
void Sprite::SetTexture(const std::string& textureFilePath) { textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath); }

void Sprite::SetAnchorPoint(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint; }

void Sprite::SetIsFlipX(const bool& isFlipX) { isFlipX_ = isFlipX; }

void Sprite::SetIsFlipY(const bool& isFlipY) { isFlipY_ = isFlipY; }

void Sprite::SetTextureLeftTop(const Vector2& textureLeftTop) { textureLeftTop_ = textureLeftTop; }

void Sprite::SetTextureSize(const Vector2& textureSize) { textureSize_ = textureSize; }

void Sprite::Init(DirectXCommon* dxCommon, PipelineManager* pipelineManager, const std::string& textureFilePath) {

	dxCommon_ = dxCommon;
	pipelineManager_ = pipelineManager;

	textureFilePath_ = textureFilePath;

	pipelineManager_->CreatePSO(dxCommon_);

	// リソース作成
	vertexResource = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * 4);
	indexResource = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(uint32_t) * 6);

	// VertexBufferViewを設定
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress(); // 先頭のアドレスを使用
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;                    // 6頂点のサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);                      // 1頂点のサイズ
	// IndexBufferViewを設定
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress(); // 先頭のアドレスを使用
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;                     // 6頂点のサイズ
	// インデックスはuint32_tとする
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
	    {0.0f, 0.0f, 0.0f},
	    {0.0f, 0.0f, 0.0f},
	};

	uvTransform = {
	    {1.0f, 1.0f, 1.0f},
	    {0.0f, 0.0f, 0.0f},
	    {0.0f, 0.0f, 0.0f},
	};

	textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);

	AdjustTextureSize();
}

void Sprite::Update() {

	// ヴァーテックスリソースにデータを書き込む
	vertexData[0].position = {0.0f, 1.0f, 0.0f, 1.0f}; // 左下
	vertexData[0].texcoord = {0.0f, 1.0f};
	vertexData[0].normal = {0.0f, 0.0f, -1.0f};

	vertexData[1].position = {0.0f, 0.0f, 0.0f, 1.0f}; // 左上
	vertexData[1].texcoord = {0.0f, 0.0f};
	vertexData[1].normal = {0.0f, 0.0f, -1.0f};

	vertexData[2].position = {1.0f, 1.0f, 0.0f, 1.0f}; // 右下
	vertexData[2].texcoord = {1.0f, 1.0f};
	vertexData[2].normal = {0.0f, 0.0f, -1.0f};

	vertexData[3].position = {1.0f, 0.0f, 0.0f, 1.0f}; // 右上
	vertexData[3].texcoord = {1.0f, 0.0f};
	vertexData[3].normal = {0.0f, 0.0f, -1.0f};

	// インデックスリソースにデータを書き込む
	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;

	indexData[3] = 1;
	indexData[4] = 3;
	indexData[5] = 2;

	transform.translate = {position_.x, position_.y, 0.0f};
	transform.scale = {size_.x, size_.y, 1.0f};
	transform.rotate = {0.0f, 0.0f, rotation_};

	float left = 0.0f - anchorPoint_.x;
	float right = 1.0f - anchorPoint_.x;
	float top = 0.0f - anchorPoint_.y;
	float bottom = 1.0f - anchorPoint_.y;

	// 左右反転
	if (isFlipX_) {
		left = -left;
		right = -right;
	}
	// 上下反転
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}

	vertexData[0].position = {left, bottom, 0.0f, 1.0f};  // 左下
	vertexData[1].position = {left, top, 0.0f, 1.0f};     // 左上
	vertexData[2].position = {right, bottom, 0.0f, 1.0f}; // 右下
	vertexData[3].position = {right, top, 0.0f, 1.0f};    // 右上

	const DirectX::TexMetadata& metaData = TextureManager::GetInstance()->GetMetaData(textureFilePath_);
	float tex_left = textureLeftTop_.x / metaData.width;
	float tex_right = (textureLeftTop_.x + textureSize_.x) / metaData.width;
	float tex_top = textureLeftTop_.y / metaData.height;
	float tex_bottom = (textureLeftTop_.y + textureSize_.y) / metaData.height;

	// 頂点リソースにデータを書き込む
	vertexData[0].texcoord = {tex_left, tex_bottom};
	vertexData[1].texcoord = {tex_left, tex_top};
	vertexData[2].texcoord = {tex_right, tex_bottom};
	vertexData[3].texcoord = {tex_right, tex_top};

	Matrix4x4 worldMatrix = MyMath::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 viewMatrix = MyMath::MakeIdentity4x4();
	Matrix4x4 projectionMatrix = MyMath::MakeOrthographicMatrix(0.0f, 0.0f, float(winApp_->GetWindowWidth()), float(winApp_->GetWindowHeight()), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = MyMath::Multiply(worldMatrix, MyMath::Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData->WVP = worldViewProjectionMatrix;
	transformationMatrixData->World = worldMatrix;

	Matrix4x4 uvTransformMatrix = MyMath::MakeScaleMatrix(uvTransform.scale);
	uvTransformMatrix = MyMath::Multiply(uvTransformMatrix, MyMath::MakeRotateZMatrix(uvTransform.rotate.z));
	uvTransformMatrix = MyMath::Multiply(uvTransformMatrix, MyMath::MakeTranslateMatrix(uvTransform.translate));
	materialData->uvTransform = uvTransformMatrix;
}

void Sprite::Draw() {

	ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();

	// 描画用のDescriptorHeapの設定
	/*ID3D12DescriptorHeap* descriptorHeaps[] = {srvDescriptorHeap.Get()};
	commandList->SetDescriptorHeaps(1, descriptorHeaps);*/

	// マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	// VBVを設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
	// IBVを設定
	commandList->IASetIndexBuffer(&indexBufferView);
	// TransformationMatrixCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());
	//
	commandList->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(textureFilePath_));
	// commandList->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);
	// Spriteの描画
	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Sprite::PreDraw() {

	ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();

	// ルートシグネチャをセット
	commandList->SetGraphicsRootSignature(pipelineManager_->GetRootSignature());
	// グラフィックスパイプラインステートをセット
	commandList->SetPipelineState(pipelineManager_->GetGraphicsPipelineState());

	// プリミティブトポロジーをセット
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Sprite::AdjustTextureSize() {

	// テクスチャメタデータを取得
	const DirectX::TexMetadata& metaData = TextureManager::GetInstance()->GetMetaData(textureFilePath_);

	textureSize_.x = static_cast<float>(metaData.width);
	textureSize_.y = static_cast<float>(metaData.height);
	// 画像サイズをテクスチャサイズに合わせる
	size_ = textureSize_;
}

void Sprite::ImGuiDebug() {

	//
	ImGui::Begin("Sprite");

	ImGui::DragFloat3("transfoem", &position_.x, 1.0f);

	ImGui::End();
}