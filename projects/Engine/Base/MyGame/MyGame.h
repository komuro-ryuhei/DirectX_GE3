#pragma once

#include "Game/Scene/GameScene.h"
#include <memory>

class MyGame {

public:

	void Run();

private:
	std::unique_ptr<GameScene> scene_ = nullptr;
};