#pragma once

#include <memory>
#include <cstdint>
#include <string>
#include <format>
#include <cassert>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "dxguid.lib")

// MyClass
#include "ComPtr.h"
#include "DirectXCommon.h"
#include "WinApp.h"
#include "PipelineManager.h"
#include "Mesh.h"
#include "Input.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "TextureManager.h"

#include "Logger.h"

/// <summary>
/// システム
/// </summary>
class System {
public: // 静的メンバ関数

	/// <summary>
	/// システム全体の初期化
	/// </summary>
	/// <param name="title">タイトル</param>
	/// <param name="width">ウィンドウの高さ</param>
	/// <param name="height">ウィンドウの幅</param>
	static void Initialize(const char* title, int width, int height);

	/// <summary>
	/// Windowsのメッセージを処理する
	/// </summary>
	/// <returns>1: ゲーム終了 0: ゲーム継続</returns>
	static bool ProcessMessage();

	/// <summary>
	/// フレーム開始処理
	/// </summary>
	static void BeginFrame();

	/// <summary>
	/// フレーム終了処理
	/// </summary>
	static void EndFrame();


	static void CreateMesh();

	static void DrawTriangle();
};