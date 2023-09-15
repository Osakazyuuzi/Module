/////////////////////////////////////////////////////////////////////////
// �t�@�C�����FGraphics.cpp
/////////////////////////////////////////////////////////////////////////
#include "Graphics.h"

/*
[�֐��T�v]
����������

[����]
HWND		handle			�E�B���h�E�n���h��
UINT		width			�E�B���h�E�T�C�Y�i���j
UINT		height			�E�B���h�E�T�C�Y�i�����j
bool		fullscreen		�t���X�N���[���t���O

[�߂�l]
bool		����������true, ���s������false��Ԃ�
*/
bool Graphics::Startup(HWND handle, UINT width, UINT height, bool fullscreen)
{
	HRESULT	hr = E_FAIL;
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));						// �[���N���A
	sd.BufferDesc.Width = width;						// �o�b�N�o�b�t�@�̕�
	sd.BufferDesc.Height = height;						// �o�b�N�o�b�t�@�̍���
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// �o�b�N�o�b�t�@�t�H�[�}�b�g(R,G,B,A)
	sd.SampleDesc.Count = 1;							// �}���`�T���v���̐�
	sd.BufferDesc.RefreshRate.Numerator = 1000;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �o�b�N�o�b�t�@�̎g�p���@
	sd.BufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	sd.OutputWindow = handle;							// �֘A�t����E�C���h�E
	sd.Windowed = fullscreen ? FALSE : TRUE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// �h���C�o�̎��
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,	// GPU�ŕ`��
		D3D_DRIVER_TYPE_WARP,		// �����x(�ᑬ
		D3D_DRIVER_TYPE_REFERENCE,	// CPU�ŕ`��
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	UINT createDeviceFlags = 0;
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

	// �@�\���x��
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,		// DirectX11.1�Ή�GPU���x��
		D3D_FEATURE_LEVEL_11_0,		// DirectX11�Ή�GPU���x��
		D3D_FEATURE_LEVEL_10_1,		// DirectX10.1�Ή�GPU���x��
		D3D_FEATURE_LEVEL_10_0,		// DirectX10�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_3,		// DirectX9.3�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_2,		// DirectX9.2�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_1		// Direct9.1�Ή�GPU���x��
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
	{
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,					// �f�B�X�v���C�f�o�C�X�̃A�_�v�^�iNULL�̏ꍇ�ŏ��Ɍ��������A�_�v�^�j
			driverType,				// �f�o�C�X�h���C�o�̃^�C�v
			NULL,					// �\�t�g�E�F�A���X�^���C�U���g�p����ꍇ�Ɏw�肷��
			createDeviceFlags,		// �f�o�C�X�t���O
			featureLevels,			// �@�\���x��
			numFeatureLevels,		// �@�\���x����
			D3D11_SDK_VERSION,		// 
			&sd,					// �X���b�v�`�F�C���̐ݒ�
			&m_pSwapChain,			// IDXGIDwapChain�C���^�t�F�[�X	
			&m_pDevice,				// ID3D11Device�C���^�t�F�[�X
			&featureLevel,		// �T�|�[�g����Ă���@�\���x��
			&m_pContext);		// �f�o�C�X�R���e�L�X�g
		if (SUCCEEDED(hr)) {
			break;
		}
	}
	if (FAILED(hr)) {
		return hr;
	}


	//--- �J�����O�ݒ�
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

	//--- �A���t�@�u�����f�B���O
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

	// �f�v�X�X�e���V���X�e�[�g�ݒ�
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	// �[�x�e�X�g
	m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthState[DEPTH_ENABLE]);//�[�x�L���X�e�[�g
	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthState[DEPTH_DISABLE]);//�[�x�����X�e�[�g

	SetDepthMode(DEPTH_ENABLE);

	// �T���v���[
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
[�֐��T�v]
�I������
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
[�֐��T�v]
�t�����g�o�b�t�@�ƃo�b�N�o�b�t�@�����ւ���
*/
void Graphics::Swap()
{
	m_pSwapChain->Present(0, 0);
}

/*
[�֐��T�v]
�f�o�C�X���擾����

[�߂�l]
ID3D11Device*	�f�o�C�X
*/
ID3D11Device* Graphics::GetDevice()
{
	return m_pDevice;
}

/*
[�֐��T�v]
�f�o�C�X�R���e�L�X�g���擾����

[�߂�l]
ID3D11DeviceContext*	�f�o�C�X�R���e�L�X�g
*/
ID3D11DeviceContext* Graphics::GetContext()
{
	return m_pContext;
}

/*
[�֐��T�v]
�X���b�v�`�F�[�����擾����

[�߂�l]
IDGISwapChain*	�X���b�v�`�F�[��
*/
IDXGISwapChain* Graphics::GetSwapChain()
{
	return m_pSwapChain;
}

/*
[�֐��T�v]
�J�����O���[�h��ݒ肷��

[����]
D3D11_CULL_MODE		cull	�J�����O���[�h
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
[�֐��T�v]
�f�v�X���[�h��ݒ肷��

[����]
DepthMode	enable		�f�v�X���[�h
*/
void Graphics::SetDepthMode(DepthMode enable)
{
	if (enable == DEPTH_ENABLE)
		m_pContext->OMSetDepthStencilState(m_pDepthState[DEPTH_ENABLE], NULL);
	else
		m_pContext->OMSetDepthStencilState(m_pDepthState[DEPTH_DISABLE], NULL);
}

/*
[�֐��T�v]
�u�����h���[�h��ݒ肷��

[����]
BlendMode	blend	�u�����h���[�h
*/
void Graphics::SetBlendMode(BlendMode blend)
{
	if (blend < 0 || blend >= BLEND_MAX) return;
	FLOAT blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	m_pContext->OMSetBlendState(m_pBlendState[blend], blendFactor, 0xffffffff);
}

/*
[�֐��T�v]
�T���v���[�X�e�[�g���[�h��ݒ肷��

[����]
SamplerState	state	�T���v���[�X�e�[�g���[�h
*/
void Graphics::SetSamplerState(SamplerState state)
{
	if (state < 0 || state >= SAMPLER_MAX) return;
	m_pContext->PSSetSamplers(0, 1, &m_pSamplerState[state]);
}