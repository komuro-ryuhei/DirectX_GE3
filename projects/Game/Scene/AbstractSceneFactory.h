#pragma once

#include "Game/Scene/IScene.h"

#include <string>

class AbstractSceneFactory {

public:
	// 仮想デストラクタ
	virtual ~AbstractSceneFactory() = default;
	// シーン生成
	virtual IScene* CreateScene(const std::string& sceneName) = 0;
};