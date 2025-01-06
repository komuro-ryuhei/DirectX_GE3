#include "GameScene.h"

#include "Engine/Base/System/System.h"

bool GameScene::GetIsFinished() const { return false; }

void GameScene::Init() {
	//	
	System::GameInit();
}

void GameScene::Update() {
	
	isFinished_ = System::IsFinished();
	System::GameUpdate();
}

void GameScene::Draw() { System::Draw(); }