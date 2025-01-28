#include "MyGame.h"

#include "Engine/Base/System/System.h"

const char kWindowTitle[] = "LE2B_12_コムロ_リュウヘイ";

void MyGame::Run() {

	System::Initialize(kWindowTitle, 1280, 720);

	sceneFactory_ = new SceneFactory();

	sceneManager_ = std::make_unique<SceneManager>();
	sceneManager_->SetSceneFactory(sceneFactory_);

	sceneManager_->ChangeScene("TITLE");

	// ×が押されるまでループ
	while (System::ProcessMessage() == 0) {

		// フレームの開始
		System::BeginFrame();

		/*scene_->Update();

		scene_->Draw();*/

		sceneManager_->Update(System::GetDxCommon(), System::GetPipelineManager(), System::GetInput());
		sceneManager_->Draw();

		// フレームの終了
		System::EndFrame();
	}

	System::Finalize();
}