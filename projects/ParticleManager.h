#pragma once

// C++
#include <Random>

// MyClass
#include "Engine/Base/DirectXCommon/DirectXCommon.h"
#include "Engine/Base/SrvManager/SrvManager.h"
#include "Engine/lib/ComPtr/ComPtr.h"
#include "struct.h"

//struct ParticleGroup {
//	MaterialData 
//};

class ParticleManager {

public:

	void Init(DirectXCommon* dxCommon, SrvManager* srvManager);

	void Update();

	void Draw();


	void Emit();

private:

	DirectXCommon* dxCommon_ = nullptr;
	SrvManager* srvManager_ = nullptr;

	ModelData modelData;

	VertexData* vertexData = nullptr;

	ComPtr<ID3D12Resource> vertexResource;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

};