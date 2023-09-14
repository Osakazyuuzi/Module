/////////////////////////////////////////////////////////////////////////
// ファイル名：Mouse.h
/////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_MOUSE_H___
#define ___CORESYSTEM_MODULE_MOUSE_H___

/////////////////
// インクルード //
/////////////////
#include <shared_mutex>

//////////////////////////
// オリジナルインクルード //
//////////////////////////
#include "Math.h"

class Mouse {
public:

	/*
	[関数概要]
	インスタンス取得

	[戻り値]
	Mouse&	インスタンスの参照
	*/
	static Mouse& GetInstance() {
		static Mouse instance;
		return instance;
	}

	/*
	[関数概要]
	マウス座標を更新する（書き込み専用）

	[引数]
	int		x		X座標
	int		y		Y座標
	*/
	void UpdateMouseMove(int x, int y) {

		// 占有ロックをかける
		std::unique_lock<std::shared_mutex> lock(m_mutex);

		// マウス座標を更新する
		m_position = { x, y };
	}

	/*
	[関数概要]
	ホイール回転量を更新する（書き込み専用）

	[引数]
	float	wheelDelta		回転量
	*/
	void UpdateWheelDelta(float wheelDelta) {

		// 占有ロックをかける
		std::unique_lock<std::shared_mutex> lock(m_mutex);

		// ホイール回転量を更新する
		m_wheelDelta = wheelDelta;
	}

	/*
	[関数概要]
	マウス座標を取得する（読み取り専用）

	[戻り値]
	Float2		マウス座標
	*/
	const Float2 GetPos() {

		// 共有ロックをかける
		std::shared_lock<std::shared_mutex> lock(m_mutex);

		// マウス座標を返す
		return m_position;
	}

	/*
	[関数概要]
	マウス座標Xを取得する（読み取り専用）

	[戻り値]
	float		マウス座標X
	*/
	const float GetPosX() {

		// 共有ロックをかける
		std::shared_lock<std::shared_mutex> lock(m_mutex);

		// マウス座標Xを返す
		return m_position.x;
	}

	/*
	[関数概要]
	マウス座標Yを取得する（読み取り専用）

	[戻り値]
	float		マウス座標Y
	*/
	const float GetPosY() {

		// 共有ロックをかける
		std::shared_lock<std::shared_mutex> lock(m_mutex);

		// マウス座標Yを返す
		return m_position.y;
	}

	/*
	[関数概要]
	ホイール回転量を取得する（読み取り専用）

	[戻り値]
	float			ホイール回転量
	*/
	const float GetWheelDelta() {

		// 共有ロックをかける
		std::shared_lock<std::shared_mutex> lock(m_mutex);


		// ホイール回転量を返す
		return m_wheelDelta;
	}

private:
	std::shared_mutex m_mutex;	// 共有
	Float2 m_position;			// カーソル座標
	float  m_wheelDelta;		// ホイール回転量
};


#endif //!___CORESYSTEM_MODULE_MOUSE_H___