#include "MyGame.h"

#include "Engine/Base/System/System.h"

// シーン
enum class Scene {
	kUnknown = 0,

	kTitle,
	kGame,
};
// 現在のシーン
Scene scene = Scene::kUnknown;

void MyGame::Run() {

	scene = Scene::kTitle;

	titleScene_ = new TitleScene();
	titleScene_->Init(System::GetDXCommon(), System::GetPipelineManager(), System::GetWinApp());

	/*scene_ = std::make_unique<TitleScene>();
	scene_->Init(System::GetDXCommon(), System::GetPipelineManager(), System::GetWinApp());*/

	auto ChangeScene = [&]() {
		switch (scene) {
		case Scene::kTitle:
			if (titleScene_->IsFinished()) {
				// シーン変更
				scene = Scene::kGame;
				// 旧シーンの開放
				delete titleScene_;
				titleScene_ = nullptr;
				// シーンの生成と初期化
				gameScene_ = new GameScene();
				gameScene_->Init();
			}
			break;
		case Scene::kGame:
			if (gameScene_->IsFinished()) {
				// シーン変更
				scene = Scene::kTitle;
				// 旧シーンの開放
				delete gameScene_;
				gameScene_ = nullptr;
				// シーンの生成と初期化
				titleScene_ = new TitleScene();
				titleScene_->Init(System::GetDXCommon(), System::GetPipelineManager(), System::GetWinApp());
			}
			break;
		}
	};

	auto UpdateScene = [&]() {
		switch (scene) {
		case Scene::kTitle:
			titleScene_->Update();
			break;
		case Scene::kGame:
			gameScene_->Update();
			break;
		}
	};

	auto DrawScene = [&]() {
		switch (scene) {
		case Scene::kTitle:
			titleScene_->Draw();
			break;
		case Scene::kGame:
			gameScene_->Draw();
			break;
		}
	};

	// ×が押されるまでループ
	while (System::ProcessMessage() == 0) {

		// フレームの開始
		System::BeginFrame();

		System::Update();

		/*scene_->Update();

		scene_->Draw();*/

		// シーン切り替え
		ChangeScene();
		// 現在シーンの更新
		UpdateScene();

		// 現在シーンの描画
		DrawScene();

		// フレームの終了
		System::EndFrame();
	}

	// 各種解放
	delete titleScene_;
	delete gameScene_;

	System::Finalize();
}