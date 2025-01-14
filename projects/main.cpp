#include <Windows.h>
#include "Engine/Base/System/System.h"
#include "Engine/Base/MyGame/MyGame.h"

const char kWindowTitle[] = "LE2B_12_コムロ_リュウヘイ_Neratte Taose!";

struct LeakChecker {
	~LeakChecker()
	{
		// リソースリークチェック
		ComPtr<IDXGIDebug1> debug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		}
	}
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	System::Initialize(kWindowTitle, 1280, 720);

	MyGame game;

	game.Run();

	return 0;
}