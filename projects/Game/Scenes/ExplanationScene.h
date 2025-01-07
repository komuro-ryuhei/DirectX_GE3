#pragma once

// MyClass
#include "Engine/Base/DirectXCommon/DirectXCommon.h"
#include "Engine/Base/PSO/PipelineManager/PipelineManager.h"
#include "Engine/Base/WinApp/WinApp.h"

#include "Engine/Base/2d/Sprite/Sprite.h"
#include "Engine/lib/Input/Input.h"

#include <memory>

class ExplanationScene {

public:
	void Init(DirectXCommon* dxCommon, PipelineManager* pipelineManager, WinApp* winApp);

	void Update();

	void Draw();

	bool IsFinished() const;

private:
	DirectXCommon* dxCommon_ = nullptr;
	PipelineManager* pipelineManager_ = nullptr;
	WinApp* winApp_ = nullptr;

	std::unique_ptr<Sprite> sprite_ = nullptr;
	std::unique_ptr<Input> input_ = nullptr;

private:
	bool isFinished_ = false;
};