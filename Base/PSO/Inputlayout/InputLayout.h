#pragma once

#include <d3d12.h>

/// <summary>
/// インプットレイアウトの設定
/// </summary>
class InputLayout {
public:
	/// <summary>
	/// 設定
	/// </summary>
	void Setting();

	// getter
	D3D12_INPUT_LAYOUT_DESC GetInputLayout() const;

private:
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
};

