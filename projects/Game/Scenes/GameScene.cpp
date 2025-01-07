#include "GameScene.h"

#include "Engine/Base/System/System.h"

bool GameScene::IsFinished() const { return isFinished_; }

void GameScene::Init() {
	//	
	System::GameInit();
}

void GameScene::Update() {
	
	System::GameUpdate();

	if (System::isFinished_) {
		isFinished_ = true;
	}
}

void GameScene::Draw() { System::Draw(); }