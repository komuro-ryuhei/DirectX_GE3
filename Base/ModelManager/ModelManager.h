#pragma once

// C++
#include <map>
#include <string>
#include <memory>

// MyClass
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Base/Model/Model.h"

class ModelManager {

public:
	ModelManager() = default;
	~ModelManager() = default;

	static ModelManager* GetInstance();

	void Init(DirectXCommon* dxCommon);

	void Finalize();

	void LoadModel(const std::string& filaPath);
	Model* FindModel(const std::string& filePath);

	void SetModel(const std::string& filePath);

private:
	ModelManager(ModelManager&) = delete;
	ModelManager& operator=(ModelManager&) = delete;

public:
	static ModelManager* instance;

private:

	DirectXCommon* dxCommon_ = nullptr;

	// モデルデータ
	std::map<std::string, std::unique_ptr<Model>> models_;
};