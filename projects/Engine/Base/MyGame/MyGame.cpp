#include "MyGame.h"

void MyGame::Run() {

	// ×が押されるまでループ
	while (System::ProcessMessage() == 0) {

		// フレームの開始
		System::BeginFrame();

		System::Update();

		System::Draw();

		// フレームの終了
		System::EndFrame();
	}

	System::Finalize();
}