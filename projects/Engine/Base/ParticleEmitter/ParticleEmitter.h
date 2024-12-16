#pragma once

// MyClass
#include "Engine/Base/ParticleManager/ParticleManager.h"

class ParticleEmitter {

public:
	void Init(const std::string& name, Vector3 translate, uint32_t count);

	void Update();

	void Emit();

private:
	std::string name_;

	Vector3 translate_;

	uint32_t count_;

	const float kDeltaTime = 1.0f / 60.0f;

	float frequency;
	float frequencyTime;
};