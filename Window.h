/////////////////////////////////////////////////////////////////////////
// ファイル名：Window.h
/////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_WINDOW_H___
#define ___CORESYSTEM_MODULE_WINDOW_H___

/////////////////
// インクルード //
/////////////////
#include <Windows.h>
#include <string>

class Window {
public:
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
	bool Startup(HINSTANCE hInstance, std::string windowTitle, UINT width, UINT height);

	/*
	[関数概要]
	終了処理
	*/
	void Shutdown();

	/*
	[関数概要]
	ウィンドウハンドルを取得する

	[戻り値]
	HWND	ウィンドウハンドル
	*/
	HWND GetHWND() { return m_handle; }

private:

	/*
	[関数概要]
	ウィンドウプロシージャ

	[引数]
	HWND		handle		ウィンドウハンドル
	UINT		msg			メッセージ
	WPARAM		wParam		上位ビット
	LPARAM		lParam		下位ビット

	[戻り値]
	LRESULT
	*/
	static LRESULT CALLBACK WndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_handle = nullptr;			// ウィンドウハンドル
	HINSTANCE m_hInstance = nullptr;	// アプリケーションインスタンスハンドル
	std::string m_windowTitle;			// ウィンドウタイトル
	UINT m_width;						// ウィンドウの横幅
	UINT m_height;						// ウィンドウの縦幅
};


#endif //!___CORESYSTEM_MODULE_WINDOW_H___