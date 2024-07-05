#pragma once

#include "ComPtr.h"

#include <memory>

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
	static bool ProcessMesage();

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