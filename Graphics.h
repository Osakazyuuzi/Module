/////////////////////////////////////////////////////////////////////////
// ファイル名：Graphics.h
/////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_GRAPHICS_H___
#define ___CORESYSTEM_MODULE_GRAPHICS_H___

/////////////////
// インクルード //
/////////////////
#include <d3d11.h>

////////////
// リンク //
////////////
#pragma comment(lib, "d3d11.lib")

class Graphics {
public:
	// ブレンドモードの種類
	enum BlendMode
	{
		BLEND_NONE,
		BLEND_ALPHA,
		BLEND_ADD,
		BLEND_ADDALPHA,
		BLEND_SUB,
		BLEND_SCREEN,
		BLEND_MAX
	};

	// サンプラーステートの種類
	enum SamplerState
	{
		SAMPLER_LINEAR,
		SAMPLER_POINT,
		SAMPLER_MAX
	};

	// デプスモードの種類
	enum DepthMode
	{
		DEPTH_DISABLE,
		DEPTH_ENABLE,
		DEPTH_MAX
	};

	/*
	[関数概要]
	初期化処理

	[引数]
	HWND		handle			ウィンドウハンドル			
	UINT		width			ウィンドウサイズ（幅）
	UINT		height			ウィンドウサイズ（高さ）
	bool		fullscreen		フルスクリーンフラグ

	[戻り値]
	bool		成功したらtrue, 失敗したらfalseを返す
	*/
	bool Startup(HWND handle, UINT width, UINT height, bool fullscreen);

	/*
	[関数概要]
	終了処理
	*/
	void Shutdown();

	/*
	[関数概要]
	フロントバッファとバックバッファを入れ替える
	*/
	void Swap();

	/*
	[関数概要]
	デバイスを取得する

	[戻り値]
	ID3D11Device*	デバイス
	*/
	ID3D11Device* GetDevice();

	/*
	[関数概要]
	デバイスコンテキストを取得する

	[戻り値]
	ID3D11DeviceContext*	デバイスコンテキスト
	*/
	ID3D11DeviceContext* GetContext();

	/*
	[関数概要]
	スワップチェーンを取得する

	[戻り値]
	IDGISwapChain*	スワップチェーン
	*/
	IDXGISwapChain* GetSwapChain();

	/*
	[関数概要]
	カリングモードを設定する

	[引数]
	D3D11_CULL_MODE		cull	カリングモード
	*/
	void SetCullingMode(D3D11_CULL_MODE cull);

	/*
	[関数概要]
	デプスモードを設定する

	[引数]
	DepthMode	enable		デプスモード
	*/
	void SetDepthMode(DepthMode enable);

	/*
	[関数概要]
	ブレンドモードを設定する

	[引数]
	BlendMode	blend	ブレンドモード
	*/
	void SetBlendMode(BlendMode blend);

	/*
	[関数概要]
	サンプラーステートモードを設定する

	[引数]
	SamplerState	state	サンプラーステートモード
	*/
	void SetSamplerState(SamplerState state);

private:
	ID3D11Device*				m_pDevice = nullptr;				// デバイス
	ID3D11DeviceContext*		m_pContext = nullptr;				// デバイスコンテキスト
	IDXGISwapChain*				m_pSwapChain = nullptr;				// スワップチェーン
	ID3D11RasterizerState*		m_pRasterizerState[3] = {};			// ラスタライザステート
	ID3D11BlendState*			m_pBlendState[BLEND_MAX] = {};		// ブレンドステート
	ID3D11SamplerState*			m_pSamplerState[SAMPLER_MAX] = {};	// サンプラーステート
	ID3D11DepthStencilState*	m_pDepthState[DEPTH_MAX] = {};		// デプスステンシルステート
};

static Graphics g_graphics;

#endif //!___CORESYSTEM_MODULE_GRAPHICS_H___