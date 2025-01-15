#include "MyGame.h"

#include "Engine/Base/System/System.h"

const char kWindowTitle[] = "LE2B_12_コムロ_リュウヘイ";

void MyGame::Run() {

	System::Initialize(kWindowTitle, 1280, 720);

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