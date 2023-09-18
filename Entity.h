/////////////////////////////////////////////////////////////////////////
// ファイル名：Entity.h
/////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_ENTITY_H___
#define ___CORESYSTEM_MODULE_ENTITY_H___

/////////////////
// インクルード //
/////////////////
#include <bitset>
#include <array>

//////////////////////////
// オリジナルインクルード //
//////////////////////////
#include "ECSUtils.h"

struct Entity {
	ECSUtils::ComponentBit		m_componentBit;		// コンポーネント所持ビット
	ECSUtils::IdentifierBit		m_identifierBit;	// 識別ビット
	ECSUtils::EntityState		m_state;			// エンティティの状態
};

#endif //!___CORESYSTEM_MODULE_ENTITY_H___
