/////////////////////////////////////////////////////////////////////////
// �t�@�C�����FWindow.h
/////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_WINDOW_H___
#define ___CORESYSTEM_MODULE_WINDOW_H___

/////////////////
// �C���N���[�h //
/////////////////
#include <Windows.h>
#include <string>

class Window {
public:
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
	bool Startup(HINSTANCE hInstance, std::string windowTitle, UINT width, UINT height);

	/*
	[�֐��T�v]
	�I������
	*/
	void Shutdown();

	/*
	[�֐��T�v]
	�E�B���h�E�n���h�����擾����

	[�߂�l]
	HWND	�E�B���h�E�n���h��
	*/
	HWND GetHWND() { return m_handle; }

private:

	/*
	[�֐��T�v]
	�E�B���h�E�v���V�[�W��

	[����]
	HWND		handle		�E�B���h�E�n���h��
	UINT		msg			���b�Z�[�W
	WPARAM		wParam		��ʃr�b�g
	LPARAM		lParam		���ʃr�b�g

	[�߂�l]
	LRESULT
	*/
	static LRESULT CALLBACK WndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_handle = nullptr;			// �E�B���h�E�n���h��
	HINSTANCE m_hInstance = nullptr;	// �A�v���P�[�V�����C���X�^���X�n���h��
	std::string m_windowTitle;			// �E�B���h�E�^�C�g��
	UINT m_width;						// �E�B���h�E�̉���
	UINT m_height;						// �E�B���h�E�̏c��
};


#endif //!___CORESYSTEM_MODULE_WINDOW_H___