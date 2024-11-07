#pragma once

// MyClass
#include "ComPtr.h"
#include "DirectXCommon.h"
#include "Mesh.h"
#include "PipelineManager.h"
#include "WinApp.h"

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