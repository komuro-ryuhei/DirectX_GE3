#pragma once

// MyClass
#include "PipelineManager.h"
#include "DirectXCommon.h"
#include "ComPtr.h"

/// <summary>
/// スプライト共通部
/// </summary>
class SpriteCommon {

public: // メンバ関数

	// 初期化
	void Initialize(DirectXCommon* dxCommon);


	// 描画前処理
	void PreDraw();


	DirectXCommon* GetDxCommon() const { return dxCommon_; }

private:

	DirectXCommon* dxCommon_;

	// パイプライン
	std::unique_ptr<PipelineManager> pipelineManager_;

	// ルートシグネチャ
	std::unique_ptr<RootSignature> rootSignature_;
	// インプットレイアウト
	std::unique_ptr<InputLayout> inputLayout_;
	// ラスタライザ
	std::unique_ptr<RasterizerState> rasterizer_;

private:
	// ルートシグネチャの作成
	void CreateRootSignature();
	// インプットレイアウトの作成
	void CreateInputLayout();
	// ラスタライザ
	void CreateRasterizer();
	// グラフィックスパイプラインの作成
	void CreateGraphicsPipeline();
};