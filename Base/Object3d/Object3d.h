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

struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
};

struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

// 3Dオブジェクト
class Object3d {

public: // メンバ関数
	Object3d() = default;
	~Object3d() = default;

	void Init(DirectXCommon* dxCommon);

	void Update();

	void Draw();

	void PreDraw();

private:
	// mtlファイルの読み取り
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	// .objファイルの読み取り
	static ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

private:
	// DxCommon
	DirectXCommon* dxCommon_ = nullptr;
	// パイプライン
	PipelineManager* pipelineManager_ = nullptr;
	// ウィンドウズアプリケーション
	WinApp* winApp_ = nullptr;

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

	// 座標変換用
	// Sprite用のTransformationMatrix用のリソースを作る
	ComPtr<ID3D12Resource> transformationMatrixResource;
	TransformationMatrix* transformationMatrixData = nullptr;


	Transform transform;
	Transform cameraTransform;
};