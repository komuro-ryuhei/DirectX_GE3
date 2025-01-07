#pragma once

#include "Engine/Base/System/System.h"

#include "Game/Scenes/TitleScene.h"
#include "Game/Scenes/ExplanationScene.h"
#include "Game/Scenes/GameScene.h"
#include "Game/Scenes/ClearScene.h"

#include <memory>

class MyGame {

public:

	void Run();

private:
	std::unique_ptr<TitleScene> scene_ = nullptr;

	TitleScene* titleScene_ = nullptr;
	ExplanationScene* explanationScene_ = nullptr;
	GameScene* gameScene_ = nullptr;
	ClearScene* clearScene_ = nullptr;
};