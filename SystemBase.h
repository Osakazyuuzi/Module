/////////////////////////////////////////////////////////////////////////
// �t�@�C�����FSystemBase.h
/////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_SYSTEMBASE_H___
#define ___CORESYSTEM_MODULE_SYSTEMBASE_H___

/////////////////
// �C���N���[�h //
/////////////////
#include <vector>

//////////////////////////
// �I���W�i���C���N���[�h //
//////////////////////////
#include "ECSUtils.h"

class World;

class SystemBase 
{
public:

	/*
	[�֐��T�v]
	�R���X�g���N�^

	[����]
	std::size_t		updateOrder		�X�V���ԍ�
	*/
	SystemBase(World* world, std::size_t updateOrder)
		: m_world(world)
		, m_updateOrder(updateOrder)
	{}

	/*
	[�֐��T�v]
	����������
	*/
	virtual void Init() {}

	/*
	[�֐��T�v]
	�I������
	*/
	virtual void Uninit() {}

	/*
	[�֐��T�v]
	�X�V����

	[����]
	float deltaTime		���s�ɂ��������t���[������(ms)
	*/
	virtual void Update(float deltaTime) {}

	/*
	[�֐��T�v]
	�`�揈��
	*/
	virtual void Draw() {}

	/*
	[�֐��T�v]
	�X�V���ԍ����擾����

	[�߂�l]
	std::size_t	�X�V���ԍ�
	*/
	std::size_t GetUpdateOrder() const noexcept { return m_updateOrder; }

protected:
	std::size_t m_updateOrder;		// �X�V���ԍ�
	World* m_world;		// �������Ă��郏�[���h
	std::bitset<ECSUtils::MAX_COMPONENT_COUNT> m_signature;	// �K�v�Ƃ���R���|�[�l���g
};

#endif //!___CORESYSTEM_MODULE_SYSTEMBASE_H___