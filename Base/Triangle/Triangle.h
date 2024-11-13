#pragma once

// MyClass
#include "lib/ComPtr/ComPtr.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Base/Mesh/Mesh.h"
#include "Base/PSO/PipelineManager/PipelineManager.h"
#include "Base/WinApp/WinApp.h"

// C++
#include <memory>

class Triangle {

public:
	void Initialize(DirectXCommon* dxCommon, PipelineManager* pipelineManager);

	void Draw();

private:
	// DirectXCommon
	DirectXCommon* dxCommon_ = nullptr;
	// Pipeline
	PipelineManager* pipelineManager_ = nullptr;

	// Mesh
	std::unique_ptr<Mesh> mesh_ = nullptr;
};