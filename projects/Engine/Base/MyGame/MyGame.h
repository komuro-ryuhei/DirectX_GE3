#pragma once

#include "Engine/Base/System/System.h"

#include "Game/Scenes/GameScene.h"
#include "Game/Scenes/TitleScene.h"

#include <memory>

class MyGame {

public:

	void Run();

private:
	std::unique_ptr<TitleScene> scene_ = nullptr;
};