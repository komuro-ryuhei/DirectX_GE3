#include "SceneFactory.h"

std::unique_ptr<IScene> SceneFactory::CreateScene(const std::string& sceneName) {

	if (sceneName == "TITLE") {
		return std::make_unique<TitleScene>();
	} else if (sceneName == "GAME") {
		return std::make_unique<GameScene>();
	} else if (sceneName == "CLEAR") {
		return std::make_unique<ClearScene>();
	} else if (sceneName == "EXPLANATION") {
		return std::make_unique<ExplanationScene>();
	}
	return nullptr;
}