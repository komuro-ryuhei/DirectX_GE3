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

	scene_ = std::make_unique<TitleScene>();
	scene_->Init(System::GetDXCommon(), System::GetPipelineManager(), System::GetWinApp());

	// ×が押されるまでループ
	while (System::ProcessMessage() == 0) {

		// フレームの開始
		System::BeginFrame();

		System::Update();

		scene_->Update();

		scene_->Draw();

		// フレームの終了
		System::EndFrame();
	}

	System::Finalize();
}