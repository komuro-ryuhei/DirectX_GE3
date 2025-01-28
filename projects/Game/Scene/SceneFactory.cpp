#include "SceneFactory.h"

IScene* SceneFactory::CreateScene(const std::string& sceneName) {

	IScene* newScene = nullptr;

	// 次のシーン生成
	if (sceneName == "TITLE") {
		newScene = new TitleScene();
	} else if (sceneName == "GAME") {
		newScene = new GameScene();
	}

	return newScene;
}