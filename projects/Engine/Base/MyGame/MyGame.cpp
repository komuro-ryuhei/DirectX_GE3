#include "MyGame.h"

#include "Engine/Base/System/System.h"

const char kWindowTitle[] = "LE2B_12_コムロ_リュウヘイ";

void MyGame::Run() {

	System::Initialize(kWindowTitle, 1280, 720);

	// シーンファクトリーを作成
	sceneFactory_ = std::make_unique<SceneFactory>();

	// シーンマネージャーを作成し、シーンファクトリーを設定
	sceneManager_ = std::make_unique<SceneManager>();
	sceneManager_->SetSceneFactory(sceneFactory_.get());

	// 初期シーンを設定
	sceneManager_->ChangeScene("TITLE");

	// ×が押されるまでループ
	while (System::ProcessMessage() == 0) {

		// フレームの開始
		System::BeginFrame();

		sceneManager_->Update(System::GetDxCommon(), System::GetPipelineManager());
		sceneManager_->Draw();

		// フレームの終了
		System::EndFrame();
	}

	System::Finalize();
}