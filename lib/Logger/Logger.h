#pragma once
#include <windows.h>
#include <string>

/*==================================================================================*/
// ログ関数

class Logger {

public: // 静的メンバ関数

	static void Log(const std::string& message);

	// void Log(const std::wstring& message);

	static std::wstring ConvertString(const std::string& str);

	static std::string ConvertString(const std::wstring& str);
};

