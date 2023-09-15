/////////////////////////////////////////////////////////////////////////
// ファイル名：Graphics.cpp
/////////////////////////////////////////////////////////////////////////
#include "Graphics.h"

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
bool Graphics::Startup(HWND handle, UINT width, UINT height, bool fullscreen)
{
	HRESULT	hr = E_FAIL;
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));						// ゼロクリア
	sd.BufferDesc.Width = width;						// バックバッファの幅
	sd.BufferDesc.Height = height;						// バックバッファの高さ
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// バックバッファフォーマット(R,G,B,A)
	sd.SampleDesc.Count = 1;							// マルチサンプルの数
	sd.BufferDesc.RefreshRate.Numerator = 1000;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// バックバッファの使用方法
	sd.BufferCount = 1;									// バックバッファの数
	sd.OutputWindow = handle;							// 関連付けるウインドウ
	sd.Windowed = fullscreen ? FALSE : TRUE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// ドライバの種類
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,	// GPUで描画
		D3D_DRIVER_TYPE_WARP,		// 高精度(低速
		D3D_DRIVER_TYPE_REFERENCE,	// CPUで描画
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	UINT createDeviceFlags = 0;
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

	// 機能レベル
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,		// DirectX11.1対応GPUレベル
		D3D_FEATURE_LEVEL_11_0,		// DirectX11対応GPUレベル
		D3D_FEATURE_LEVEL_10_1,		// DirectX10.1対応GPUレベル
		D3D_FEATURE_LEVEL_10_0,		// DirectX10対応GPUレベル
		D3D_FEATURE_LEVEL_9_3,		// DirectX9.3対応GPUレベル
		D3D_FEATURE_LEVEL_9_2,		// DirectX9.2対応GPUレベル
		D3D_FEATURE_LEVEL_9_1		// Direct9.1対応GPUレベル
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
	{
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,					// ディスプレイデバイスのアダプタ（NULLの場合最初に見つかったアダプタ）
			driverType,				// デバイスドライバのタイプ
			NULL,					// ソフトウェアラスタライザを使用する場合に指定する
			createDeviceFlags,		// デバイスフラグ
			featureLevels,			// 機能レベル
			numFeatureLevels,		// 機能レベル数
			D3D11_SDK_VERSION,		// 
			&sd,					// スワップチェインの設定
			&m_pSwapChain,			// IDXGIDwapChainインタフェース	
			&m_pDevice,				// ID3D11Deviceインタフェース
			&featureLevel,		// サポートされている機能レベル
			&m_pContext);		// デバイスコンテキスト
		if (SUCCEEDED(hr)) {
			break;
		}
	}
	if (FAILED(hr)) {
		return hr;
	}


	//--- カリング設定
	D3D11_RASTERIZER_DESC rasterizer = {};
	D3D11_CULL_MODE cull[] = {
		D3D11_CULL_NONE,
		D3D11_CULL_FRONT,
		D3D11_CULL_BACK
	};
	rasterizer.FillMode = D3D11_FILL_SOLID;
	rasterizer.FrontCounterClockwise = true;
	for (int i = 0; i < 3; ++i)
	{
		rasterizer.CullMode = cull[i];
		hr = m_pDevice->CreateRasterizerState(&rasterizer, &m_pRasterizerState[i]);
		if (FAILED(hr)) { return hr; }
	}
	SetCullingMode(D3D11_CULL_NONE);

	//--- アルファブレンディング
	// https://pgming-ctrl.com/directx11/blend/
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	D3D11_BLEND blend[BLEND_MAX][2] = {
		{D3D11_BLEND_ONE, D3D11_BLEND_ZERO},
		{D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA},
		{D3D11_BLEND_ONE, D3D11_BLEND_ONE},
		{D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE},
		{D3D11_BLEND_ZERO, D3D11_BLEND_INV_SRC_COLOR},
		{D3D11_BLEND_INV_DEST_COLOR, D3D11_BLEND_ONE},
	};
	for (int i = 0; i < BLEND_MAX; ++i)
	{
		blendDesc.RenderTarget[0].SrcBlend = blend[i][0];
		blendDesc.RenderTarget[0].DestBlend = blend[i][1];
		hr = m_pDevice->CreateBlendState(&blendDesc, &m_pBlendState[i]);
		if (FAILED(hr)) { return hr; }
	}
	SetBlendMode(BLEND_ALPHA);

	// デプスステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	// 深度テスト
	m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthState[DEPTH_ENABLE]);//深度有効ステート
	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthState[DEPTH_DISABLE]);//深度無効ステート

	SetDepthMode(DEPTH_ENABLE);

	// サンプラー
	D3D11_SAMPLER_DESC samplerDesc = {};
	D3D11_FILTER filter[] = {
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_FILTER_MIN_MAG_MIP_POINT,
	};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	for (int i = 0; i < SAMPLER_MAX; ++i)
	{
		samplerDesc.Filter = filter[i];
		hr = m_pDevice->CreateSamplerState(&samplerDesc, &m_pSamplerState[i]);
		if (FAILED(hr)) { return hr; }
	}
	SetSamplerState(SAMPLER_LINEAR);

	return S_OK;
}

/*
[関数概要]
終了処理
*/
void Graphics::Shutdown()
{
	for (int i = 0; i < SAMPLER_MAX; ++i)
		m_pSamplerState[i]->Release();
	for (int i = 0; i < BLEND_MAX; ++i)
		m_pBlendState[i]->Release();
	for (int i = 0; i < 3; ++i)
		m_pRasterizerState[i]->Release();

	m_pContext->ClearState();
	if (m_pContext) { m_pContext->Release(); m_pContext = nullptr; }

	m_pSwapChain->SetFullscreenState(false, NULL);
	if (m_pSwapChain) { m_pSwapChain->Release(); m_pSwapChain = nullptr; }

	if (m_pDevice) { m_pDevice->Release(); m_pDevice = nullptr; }
}

/*
[関数概要]
フロントバッファとバックバッファを入れ替える
*/
void Graphics::Swap()
{
	m_pSwapChain->Present(0, 0);
}

/*
[関数概要]
デバイスを取得する

[戻り値]
ID3D11Device*	デバイス
*/
ID3D11Device* Graphics::GetDevice()
{
	return m_pDevice;
}

/*
[関数概要]
デバイスコンテキストを取得する

[戻り値]
ID3D11DeviceContext*	デバイスコンテキスト
*/
ID3D11DeviceContext* Graphics::GetContext()
{
	return m_pContext;
}

/*
[関数概要]
スワップチェーンを取得する

[戻り値]
IDGISwapChain*	スワップチェーン
*/
IDXGISwapChain* Graphics::GetSwapChain()
{
	return m_pSwapChain;
}

/*
[関数概要]
カリングモードを設定する

[引数]
D3D11_CULL_MODE		cull	カリングモード
*/
void Graphics::SetCullingMode(D3D11_CULL_MODE cull)
{
	switch (cull)
	{
	case D3D11_CULL_NONE: m_pContext->RSSetState(m_pRasterizerState[0]); break;
	case D3D11_CULL_FRONT: m_pContext->RSSetState(m_pRasterizerState[1]); break;
	case D3D11_CULL_BACK: m_pContext->RSSetState(m_pRasterizerState[2]); break;
	}
}

/*
[関数概要]
デプスモードを設定する

[引数]
DepthMode	enable		デプスモード
*/
void Graphics::SetDepthMode(DepthMode enable)
{
	if (enable == DEPTH_ENABLE)
		m_pContext->OMSetDepthStencilState(m_pDepthState[DEPTH_ENABLE], NULL);
	else
		m_pContext->OMSetDepthStencilState(m_pDepthState[DEPTH_DISABLE], NULL);
}

/*
[関数概要]
ブレンドモードを設定する

[引数]
BlendMode	blend	ブレンドモード
*/
void Graphics::SetBlendMode(BlendMode blend)
{
	if (blend < 0 || blend >= BLEND_MAX) return;
	FLOAT blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	m_pContext->OMSetBlendState(m_pBlendState[blend], blendFactor, 0xffffffff);
}

/*
[関数概要]
サンプラーステートモードを設定する

[引数]
SamplerState	state	サンプラーステートモード
*/
void Graphics::SetSamplerState(SamplerState state)
{
	if (state < 0 || state >= SAMPLER_MAX) return;
	m_pContext->PSSetSamplers(0, 1, &m_pSamplerState[state]);
}