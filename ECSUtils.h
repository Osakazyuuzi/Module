/////////////////////////////////////////////////////////////////////////
// �t�@�C�����FECSUtils.h
/////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_ECSUTILS_H___
#define ___CORESYSTEM_MODULE_ECSUTILS_H___

/////////////////
// �C���N���[�h //
/////////////////
#include <bitset>

namespace ECSUntils
{
	//--- �R���|�[�l���g�̍ő��ސ�
	constexpr std::size_t MAX_COMPONENT_COUNT = 128;

	//--- �G���e�B�e�B�̏��
	enum class EntityState {
		Active,		// �A�N�e�B�u�i�X�V�E�`�拤�ɍs����j
		Inactive,	// ��A�N�e�B�u�i�X�V�E�`�拤�ɍs���Ȃ��j
		Destroyed};	// ���݂��Ȃ�

	//--- �G���e�B�e�B�̎��ʃr�b�g
	constexpr std::size_t ENTITY_IDENTIFIER_BIT = 32;
	constexpr std::size_t ENTITY_UNIQUEID_BIT = ENTITY_IDENTIFIER_BIT / 2;	// ���j�[�NID�̃r�b�g��(��ʃr�b�g)
	constexpr std::size_t ENTITY_VERSION_BIT = ENTITY_IDENTIFIER_BIT - ENTITY_UNIQUEID_BIT;	// �o�[�W�����ԍ��̃r�b�g���i���ʃr�b�g�j

	//--- �ʖ��G�C���A�X
	using IdentifierBit = std::bitset<ECSUntils::ENTITY_IDENTIFIER_BIT>;
	using ComponentBit = std::bitset<ECSUntils::MAX_COMPONENT_COUNT>;

	//--- �G���e�B�e�B�̎��ʃr�b�g�Ǘ��֐�
	/*
	[�֐��T�v]
	Entity�����ʂ���Bitset�̏�ʃr�b�g��ID��ݒ肷��

	[����]
	std::bitset<ENTITY_IDENTIFIER_BIT>&		bit		�G���e�B�e�B�������ʃr�b�g
	std::size_t								id		�ݒ肷��ID
	*/
	void SetUniqueID(_Out_ std::bitset<ENTITY_IDENTIFIER_BIT>& bit, _In_ std::size_t id)
	{
		// ���ʃr�b�g�݂̂��c��
		bit &= std::bitset<ENTITY_IDENTIFIER_BIT>((1ull << ENTITY_VERSION_BIT) - 1);

		// ��ʃr�b�g�ɉ��ʃr�b�g�����炵���l���Z�b�g����
		bit |= std::bitset<ENTITY_IDENTIFIER_BIT>(id << ENTITY_VERSION_BIT);
	}
	/*
	[�֐��T�v]
	Entity�����ʂ���Bitset�̉��ʃr�b�g�Ƀo�[�W������ݒ肷��

	[����]
	std::bitset<ENTITY_IDENTIFIER_BIT>&		bit			�G���e�B�e�B�������ʃr�b�g
	std::size_t								version		�ݒ肷��o�[�W����
	*/
	void SetVersion(_Out_ std::bitset<ENTITY_IDENTIFIER_BIT>& bit, _In_ std::size_t version)
	{
		// ��ʃr�b�g�݂̂��c��
		bit &= std::bitset<ENTITY_IDENTIFIER_BIT>(~((1ull << ENTITY_VERSION_BIT) - 1));

		// ���ʃr�b�g�ɒl���Z�b�g����
		bit |= std::bitset<ENTITY_IDENTIFIER_BIT>(version);
	}
}

#endif //!___CORESYSTEM_MODULE_ECSUTILS_H___