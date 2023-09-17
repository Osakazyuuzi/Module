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
	ECSUntils::IdentifierBit	m_identifierBit;	// 識別ビット
	ECSUntils::ComponentBit		m_componentBit;		// コンポーネント所持ビット
	ECSUntils::EntityState		m_state;			// エンティティの状態
};

#endif //!___CORESYSTEM_MODULE_ENTITY_H___