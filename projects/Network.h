#pragma once

#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS

#include <cpprest/http_client.h>
#include <string>
#include <vector>

using namespace web;
using namespace web::http;
using namespace web::http::client;

class Network {
public:
	Network(const std::wstring& serverUrl);

	pplx::task<void> Login(const std::wstring& username, const std::wstring& password);
	pplx::task<int> PostScore(int score);
	pplx::task<std::vector<int>> GetRanking();

private:
	std::wstring serverUrl;
	std::wstring authToken;
};