/////////////////////////////////////////////////////////////////////////
// �t�@�C�����FGraphics.h
/////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_GRAPHICS_H___
#define ___CORESYSTEM_MODULE_GRAPHICS_H___

/////////////////
// �C���N���[�h //
/////////////////
#include <d3d11.h>

////////////
// �����N //
////////////
#pragma comment(lib, "d3d11.lib")

class Graphics {
public:
	// �u�����h���[�h�̎��
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

	// �T���v���[�X�e�[�g�̎��
	enum SamplerState
	{
		SAMPLER_LINEAR,
		SAMPLER_POINT,
		SAMPLER_MAX
	};

	// �f�v�X���[�h�̎��
	enum DepthMode
	{
		DEPTH_DISABLE,
		DEPTH_ENABLE,
		DEPTH_MAX
	};

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
	bool Startup(HWND handle, UINT width, UINT height, bool fullscreen);

	/*
	[�֐��T�v]
	�I������
	*/
	void Shutdown();

	/*
	[�֐��T�v]
	�t�����g�o�b�t�@�ƃo�b�N�o�b�t�@�����ւ���
	*/
	void Swap();

	/*
	[�֐��T�v]
	�f�o�C�X���擾����

	[�߂�l]
	ID3D11Device*	�f�o�C�X
	*/
	ID3D11Device* GetDevice();

	/*
	[�֐��T�v]
	�f�o�C�X�R���e�L�X�g���擾����

	[�߂�l]
	ID3D11DeviceContext*	�f�o�C�X�R���e�L�X�g
	*/
	ID3D11DeviceContext* GetContext();

	/*
	[�֐��T�v]
	�X���b�v�`�F�[�����擾����

	[�߂�l]
	IDGISwapChain*	�X���b�v�`�F�[��
	*/
	IDXGISwapChain* GetSwapChain();

	/*
	[�֐��T�v]
	�J�����O���[�h��ݒ肷��

	[����]
	D3D11_CULL_MODE		cull	�J�����O���[�h
	*/
	void SetCullingMode(D3D11_CULL_MODE cull);

	/*
	[�֐��T�v]
	�f�v�X���[�h��ݒ肷��

	[����]
	DepthMode	enable		�f�v�X���[�h
	*/
	void SetDepthMode(DepthMode enable);

	/*
	[�֐��T�v]
	�u�����h���[�h��ݒ肷��

	[����]
	BlendMode	blend	�u�����h���[�h
	*/
	void SetBlendMode(BlendMode blend);

	/*
	[�֐��T�v]
	�T���v���[�X�e�[�g���[�h��ݒ肷��

	[����]
	SamplerState	state	�T���v���[�X�e�[�g���[�h
	*/
	void SetSamplerState(SamplerState state);

private:
	ID3D11Device*				m_pDevice = nullptr;				// �f�o�C�X
	ID3D11DeviceContext*		m_pContext = nullptr;				// �f�o�C�X�R���e�L�X�g
	IDXGISwapChain*				m_pSwapChain = nullptr;				// �X���b�v�`�F�[��
	ID3D11RasterizerState*		m_pRasterizerState[3] = {};			// ���X�^���C�U�X�e�[�g
	ID3D11BlendState*			m_pBlendState[BLEND_MAX] = {};		// �u�����h�X�e�[�g
	ID3D11SamplerState*			m_pSamplerState[SAMPLER_MAX] = {};	// �T���v���[�X�e�[�g
	ID3D11DepthStencilState*	m_pDepthState[DEPTH_MAX] = {};		// �f�v�X�X�e���V���X�e�[�g
};

static Graphics g_graphics;

#endif //!___CORESYSTEM_MODULE_GRAPHICS_H___