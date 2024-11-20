#pragma once

// C++
#include "fstream"
#include "sstream"
#include <d3d12.h>
#include <vector>

// MyClass
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Base/PSO/PipelineManager/PipelineManager.h"
#include "Base/TextureManager/TextureManager.h"
#include "Base/WinApp/WinApp.h"
#include "lib/Math/MyMath.h"

struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

struct MaterialData {
	std::string textureFilePath;
	uint32_t textureIndex = 0;
};

struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
};

struct Material {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};

class Model {

public:
	void Init(DirectXCommon* dxCommon, const std::string& directoryPath, const std::string& filename);

	void Draw();

public: // メンバ関数
	Model() = default;
	~Model() = default;
	Model(const Model&) = delete;
	const Model& operator=(const Model&) = delete;

public:
	//// getter
	// const Vector3& GetScale() const;
	// const Vector3& GetRotate() const;
	// const Vector3& GetTranslate() const;
	//// setter
	// void SetScale(const Vector3& scale);
	// void SetRotate(const Vector3& Rotate);
	// void SetTranslate(const Vector3& Translate);

private:
	// mtlファイルの読み取り
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	// .objファイルの読み取り
	static ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;

	// Objファイルのデータ
	ModelData modelData;

	// バッファーリソース
	ComPtr<ID3D12Resource> vertexResource;
	// バッファーリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;
	// バッファーリソースの使い道を補足するバッファービュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	// マテリアルリソース
	ComPtr<ID3D12Resource> materialResource;
	// マテリアルリソース内のデータを指すポインタ
	Material* materialData = nullptr;
};