/////////////////////////////////////////////////////////////////////////
// �t�@�C�����FWindow.cpp
/////////////////////////////////////////////////////////////////////////
#include "Window.h"

/////////////////
// �C���N���[�h //
/////////////////
#include <iostream>

/*
[�֐��T�v]
����������

[����]
HINSTANCE	hInstance		�C���X�^���X�n���h��
std::string windowTitle		�E�B���h�E�^�C�g����
UINT		width			�E�B���h�E�T�C�Y�i���j
UINT		height			�E�B���h�E�T�C�Y�i�����j

[�߂�l]
bool		����������true, ���s������false��Ԃ�
*/
bool Window::Startup(HINSTANCE hInstance, std::string windowTitle, UINT width, UINT height)
{
	// �����o�ɏ���n��
	m_hInstance = hInstance;
	m_windowTitle = windowTitle;
	m_width = width;
	m_height = height;

	// �E�B���h�N���X���̐ݒ�
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_windowTitle.c_str();

	// �E�B���h�E�N���X���̓o�^
	RegisterClassEx(&wc);

	// �E�B���h�E����ʂ̒����ɔz�u
	int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - m_width / 2;
	int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - m_height / 2;

	// �N���C�A���g�̈�̃T�C�Y
	RECT rc;
	rc.left = centerScreenX;
	rc.top = centerScreenY;
	rc.right = rc.left + width;
	rc.bottom = rc.top + height;

	// �E�B���h�E�̃T�C�Y���v�Z
	AdjustWindowRect(&rc, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	// �E�B���h�E���쐬
	m_handle = CreateWindowEx(
		0,	// �G�N�X�e���V�����X�^�C��
		m_windowTitle.c_str(),	// �E�B���h�E�N���X��
		m_windowTitle.c_str(),	// �E�B���h�E�^�C�g����
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,	// �E�B���h�E�X�^�C��
		rc.left,			// �E�B���h�E�̈ʒu X
		rc.top,				// �E�B���h�E�̈ʒu Y
		rc.right - rc.left,	// �E�B���h�E�̕� X
		rc.bottom - rc.top,	// �E�B���h�E�̕� Y
		NULL,				// �e�E�B���h�E
		NULL,				// ���j���[�n���h��
		this->m_hInstance,	// �C���X�^���X�n���h��
		NULL				// �E�B���h�E�쐬���̃p�����[�^�[
	);

	// �G���[�`�F�b�N
	if (m_handle == NULL) {
		// �G���[���b�Z�[�W���R���\�[���ɏo�͂���
		std::cerr << "Failed to CreateWindow!"
			<< "\nFile: " << __FILE__
			<< "\nLine: " << __LINE__ << std::endl;

		// ���s�����̂�false��Ԃ�
		return false;
	}

	// �E�B���h�E�̕\��
	ShowWindow(m_handle, SW_SHOW);
	UpdateWindow(m_handle);

	// �E�B���h�E���쐬�ł����̂�true��Ԃ�
	return true;
}

/*
[�֐��T�v]
�I������
*/
void Window::Shutdown()
{
	if (this->m_handle != NULL)
	{
		UnregisterClass(m_windowTitle.c_str(), m_hInstance);
		DestroyWindow(m_handle);
	}
}

/*
[�֐��T�v]
�E�B���h�E�v���V�[�W��

[����]
HWND		hWnd		�E�B���h�E�n���h��
UINT		msg			���b�Z�[�W
WPARAM		wParam		��ʃr�b�g
LPARAM		lParam		���ʃr�b�g

[�߂�l]
LRESULT
*/
LRESULT Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}