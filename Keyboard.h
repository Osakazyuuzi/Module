/////////////////////////////////////////////////////////////////////////
// ファイル名：Keyboard.h
/////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_KEYBOARD_H___
#define ___CORESYSTEM_MODULE_KEYBOARD_H___

/////////////////
// インクルード //
/////////////////
#include <array>
#include <Windows.h>
#include <shared_mutex>
#include <iostream>

class Keyboard {
public:

	/*
	[関数概要]
	コンストラクタ
	*/
	Keyboard() 
		: m_triggerState{}
		, m_pressState{}
		, m_releaseState{}
	{}
	
	/*
	[関数概要]
	インスタンス取得

	[戻り値]
	Keyboard&	インスタンスの参照
	*/
	static Keyboard& GetInstance() {
		static Keyboard instance;
		return instance;
	}

	/*
	[関数概要]
	キー情報を更新する（書き込み専用）
	
	[引数]
	BYTE	key			キー
	bool	isPress		キーの状態
	*/
	void UpdateKeyState(BYTE key, bool isPress) {

		// キー番号が256以上だった場合
		if (key >= 256) {
			// エラーメッセージをコンソールに出力する
			std::cerr << "Failed to UpdateKeyState: "
				<< "\nFile: " << __FILE__
				<< "\nLine: " << __LINE__ << std::endl;

			return;
		}

		// 占有ロックをかける
		std::unique_lock<std::shared_mutex> lock(m_mutex);

		// 押されているが、押した情報がなかった場合
		if (isPress && !m_pressState[key]) {
			// 押された瞬間としてフラグを立てる
			m_triggerState[key] = true;
		}

		// 押されていないが、押した情報がある場合
		if (!isPress && m_pressState[key]) {
			// 離した瞬間としてフラグを立てる
			m_releaseState[key] = true;
		}

		// 現在の状態をフラグに反映させる
		m_pressState[key] = isPress;
	}

	/*
	[関数概要]
	指定のキーが押された瞬間かどうかを返す（読み取り専用）
	
	[引数]
	BYTE	key		キー

	[戻り値]
	bool	押された瞬間ならtrue, そうでなければfalseを返す
	*/
	bool IsKeyTrigger(BYTE key) {

		// 共有ロックをかける
		std::shared_lock<std::shared_mutex> lock(m_mutex);

		// トリガー状態を戻り値用に保持
		bool trigger = m_triggerState[key];

		// トリガー状態をリセット
		m_triggerState[key] = false;

		// トリガー状態を返す
		return trigger;
	}

	/*
	[関数概要]
	指定のキーが押されているかどうかを返す（読み取り専用）

	[引数]
	BYTE	key		キー

	[戻り値]
	bool	押されているならtrue, そうでなければfalseを返す
	*/
	bool IsKeyPress(BYTE key) {

		// 共有ロックをかける
		std::shared_lock<std::shared_mutex> lock(m_mutex);

		// プレス状態を返す
		return m_pressState[key];
	}

	/*
	[関数概要]
	指定のキーを離した瞬間かどうかを返す（読み取り専用）

	[引数]
	BYTE	key		キー

	[戻り値]
	bool	離した瞬間ならtrue, そうでなければfalseを返す
	*/
	bool IsKeyRelease(BYTE key) {

		// 共有ロックをかける
		std::shared_lock<std::shared_mutex> lock(m_mutex);

		// リリース状態を戻り値用に保持
		bool release = m_releaseState[key];

		// リリース状態をリセット
		m_releaseState[key] = false;

		// リリース状態を返す
		return release;
	}

private:
	std::shared_mutex m_mutex;	// 共有
	std::array<bool, 256> m_triggerState;		// 押し始めたキーのテーブル
	std::array<bool, 256> m_pressState;			// 押し続けているキーのテーブル
	std::array<bool, 256> m_releaseState;		// 離した瞬間のキーのテーブル
};

#endif //!___CORESYSTEM_MODULE_KEYBOARD_H___