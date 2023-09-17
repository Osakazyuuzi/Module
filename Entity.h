/////////////////////////////////////////////////////////////////////////
// �t�@�C�����FEntity.h
/////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_ENTITY_H___
#define ___CORESYSTEM_MODULE_ENTITY_H___

/////////////////
// �C���N���[�h //
/////////////////
#include <bitset>
#include <array>

//////////////////////////
// �I���W�i���C���N���[�h //
//////////////////////////
#include "ECSUtils.h"

struct Entity {
	ECSUntils::IdentifierBit	m_identifierBit;	// ���ʃr�b�g
	ECSUntils::ComponentBit		m_componentBit;		// �R���|�[�l���g�����r�b�g
	ECSUntils::EntityState		m_state;			// �G���e�B�e�B�̏��
};

#endif //!___CORESYSTEM_MODULE_ENTITY_H___