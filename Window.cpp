/////////////////////////////////////////////////////////////////////////
// ファイル名：Window.cpp
/////////////////////////////////////////////////////////////////////////
#include "Window.h"

/////////////////
// インクルード //
/////////////////
#include <iostream>

//////////////////////////
// オリジナルインクルード //
//////////////////////////
#include "Keyboard.h"

/*
[関数概要]
初期化処理

[引数]
HINSTANCE	hInstance		インスタンスハンドル
std::string windowTitle		ウィンドウタイトル名
UINT		width			ウィンドウサイズ（幅）
UINT		height			ウィンドウサイズ（高さ）

[戻り値]
bool		成功したらtrue, 失敗したらfalseを返す
*/
bool Window::Startup(HINSTANCE hInstance, std::string windowTitle, UINT width, UINT height)
{
	// メンバに情報を渡す
	m_hInstance = hInstance;
	m_windowTitle = windowTitle;
	m_width = width;
	m_height = height;

	// ウィンドクラス情報の設定
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

	// ウィンドウクラス情報の登録
	RegisterClassEx(&wc);

	// ウィンドウを画面の中央に配置
	int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - m_width / 2;
	int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - m_height / 2;

	// クライアント領域のサイズ
	RECT rc;
	rc.left = centerScreenX;
	rc.top = centerScreenY;
	rc.right = rc.left + width;
	rc.bottom = rc.top + height;

	// ウィンドウのサイズを計算
	AdjustWindowRect(&rc, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	// ウィンドウを作成
	m_handle = CreateWindowEx(
		0,	// エクステンションスタイル
		m_windowTitle.c_str(),	// ウィンドウクラス名
		m_windowTitle.c_str(),	// ウィンドウタイトル名
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,	// ウィンドウスタイル
		rc.left,			// ウィンドウの位置 X
		rc.top,				// ウィンドウの位置 Y
		rc.right - rc.left,	// ウィンドウの幅 X
		rc.bottom - rc.top,	// ウィンドウの幅 Y
		NULL,				// 親ウィンドウ
		NULL,				// メニューハンドル
		this->m_hInstance,	// インスタンスハンドル
		NULL				// ウィンドウ作成時のパラメーター
	);

	// エラーチェック
	if (m_handle == NULL) {
		// エラーメッセージをコンソールに出力する
		std::cerr << "Failed to CreateWindow!"
			<< "\nFile: " << __FILE__
			<< "\nLine: " << __LINE__ << std::endl;

		// 失敗したのでfalseを返す
		return false;
	}

	// ウィンドウの表示
	ShowWindow(m_handle, SW_SHOW);
	UpdateWindow(m_handle);

	// ウィンドウが作成できたのでtrueを返す
	return true;
}

/*
[関数概要]
終了処理
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
[関数概要]
ウィンドウプロシージャ

[引数]
HWND		hWnd		ウィンドウハンドル
UINT		msg			メッセージ
WPARAM		wParam		上位ビット
LPARAM		lParam		下位ビット

[戻り値]
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
	// キーボードが押された場合
	case WM_KEYDOWN:
	{
		Keyboard::GetInstance().UpdateKeyState(wParam, true);
		break;
	}
	// キーボードが押されなくなった場合
	case WM_KEYUP:
	{
		Keyboard::GetInstance().UpdateKeyState(wParam, false);
		break;
	}
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}
