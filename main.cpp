#include <Windows.h>
#include "System.h"

const char kWindowTitle[] = "LE2B_11_コムロ_リュウヘイ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	System::Initialize(kWindowTitle, 1280, 720);

	// ×が押されるまでループ
	while (System::ProcessMesage() == 0) {

		// フレームの開始
		System::BeginFrame();

		System::DrawTriangle();

		// フレームの終了
		System::EndFrame();
	}

	return 0;
}