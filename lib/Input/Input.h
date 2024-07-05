#pragma once

#include <windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include "dinput.h"
#include <cassert>

#include "WinApp.h"
#include "ComPtr.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

/*==================================================================================*/

/// <summary>
/// 入力
/// </summary>
class Input {

public: // メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static Input* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:

	ComPtr<IDirectInput8> directInput = nullptr;
	ComPtr<IDirectInputDevice8> keyboard;
};