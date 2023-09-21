/////////////////////////////////////////////////////////////////////////
// ファイル名：SystemBase.h
/////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_SYSTEMBASE_H___
#define ___CORESYSTEM_MODULE_SYSTEMBASE_H___

/////////////////
// インクルード //
/////////////////
#include <vector>

//////////////////////////
// オリジナルインクルード //
//////////////////////////
#include "ECSUtils.h"

class World;

class SystemBase 
{
public:

	/*
	[関数概要]
	コンストラクタ

	[引数]
	std::size_t		updateOrder		更新順番号
	*/
	SystemBase(World* world, std::size_t updateOrder)
		: m_world(world)
		, m_updateOrder(updateOrder)
	{}

	/*
	[関数概要]
	初期化処理
	*/
	virtual void Init() {}

	/*
	[関数概要]
	終了処理
	*/
	virtual void Uninit() {}

	/*
	[関数概要]
	更新処理

	[引数]
	float deltaTime		実行にかかったフレーム時間(ms)
	*/
	virtual void Update(float deltaTime) {}

	/*
	[関数概要]
	描画処理
	*/
	virtual void Draw() {}

	/*
	[関数概要]
	更新順番号を取得する

	[戻り値]
	std::size_t	更新順番号
	*/
	std::size_t GetUpdateOrder() const noexcept { return m_updateOrder; }

protected:
	std::size_t m_updateOrder;		// 更新順番号
	World* m_world;		// 所属しているワールド
	std::bitset<ECSUtils::MAX_COMPONENT_COUNT> m_signature;	// 必要とするコンポーネント
};

#endif //!___CORESYSTEM_MODULE_SYSTEMBASE_H___