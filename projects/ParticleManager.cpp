#include "ParticleManager.h"

void ParticleManager::Init(DirectXCommon* dxCommon, SrvManager* srvManager) {

	//
	dxCommon_ = dxCommon;
	srvManager_ = srvManager;

	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

	// 頂点データの初期化
	VertexData* vertexData = nullptr;
	// 書き込むアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	// 頂点データにリソースをコピー
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());

	// 座標
	modelData.vertices.push_back({
	    .position = {1.0f, 1.0f, 0.0f, 1.0f},
          .texcoord = {0.0f, 0.0f},
          .normal = {0.0f, 0.0f, 1.0f}
    });
	modelData.vertices.push_back({
	    .position = {-1.0f, 1.0f, 0.0f, 1.0f},
          .texcoord = {1.0f, 0.0f},
          .normal = {0.0f, 0.0f, 1.0f}
    });
	modelData.vertices.push_back({
	    .position = {1.0f, -1.0f, 0.0f, 1.0f},
          .texcoord = {0.0f, 1.0f},
          .normal = {0.0f, 0.0f, 1.0f}
    });
	modelData.vertices.push_back({
	    .position = {1.0f, -1.0f, 0.0f, 1.0f},
          .texcoord = {0.0f, 1.0f},
          .normal = {0.0f, 0.0f, 1.0f}
    });
	modelData.vertices.push_back({
	    .position = {-1.0f, 1.0f, 0.0f, 1.0f},
          .texcoord = {1.0f, 0.0f},
          .normal = {0.0f, 0.0f, 1.0f}
    });
	modelData.vertices.push_back({
	    .position = {-1.0f, -1.0f, 0.0f, 1.0f},
          .texcoord = {1.0f, 1.0f},
          .normal = {0.0f, 0.0f, 1.0f}
    });

	// 頂点リソースの作成
	 vertexResource = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * modelData.vertices.size());

	 // VBVの作成
	 // リソースの先頭のアドレスから使う
	 vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	 // 使用するリソースのサイズは頂点3つ分のサイズ
	 vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	 // 1頂点当あたりのサイズ
	 vertexBufferView.StrideInBytes = sizeof(VertexData);

	 // 頂点リソースに頂点データを書き込む
	 vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	 // 頂点データにリソースをコピー
	 std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());
}