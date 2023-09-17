////////////////////////////////////////////////////////////////////////////////
// �t�@�C����: EntityIDGenerator.h
////////////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_ENTITYIDGENERATOR_H___
#define ___CORESYSTEM_MODULE_ENTITYIDGENERATOR_H___

/////////////////
// �C���N���[�h //
/////////////////
#include <atomic>

class EntityIDGenerator {
public:

	/*
	[�֐��T�v]
	�V�K��ID���o�͂���

	[�߂�l]
	std::size_t		�V����ID
	*/
	template<typename TComponent>
	static std::size_t GetID() {
		return ++m_entityIdCounter;
	}

private:
	static std::atomic<std::size_t> m_entityIdCounter;
};

#endif //!___CORESYSTEM_MODULE_ENTITYIDGENERATOR_H___