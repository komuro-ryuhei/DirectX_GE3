#pragma once

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public:
	void Init();

	void Update();

	void Draw();

	bool IsFinished() const;

private:

	bool isFinished_ = false;
};