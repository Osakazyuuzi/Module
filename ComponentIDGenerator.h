////////////////////////////////////////////////////////////////////////////////
// �t�@�C����: ComponentIDGenerator.h
////////////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_COMPONENTIDGENERATOR_H___
#define ___CORESYSTEM_MODULE_COMPONENTIDGENERATOR_H___

/////////////////
// �C���N���[�h //
/////////////////
#include <unordered_map>
#include <typeindex>
#include <mutex>

//////////////////////////
// �I���W�i���C���N���[�h //
//////////////////////////
#include "ComponentBase.h"

class ComponentIDGenerator
{
public:

	/*
	[�֐��T�v]
	Component�̎�ނɕR�Â���ID���擾����B
	�o�^����ĂȂ���ΐV�K�œo�^����B�i�ǂݎ��A�������݁j

	[�e���v���[�g����]
	TComponent		�R���|�[�l���g�̌^

	[�߂�l]
	std::size_t		�R���|�[�l���g�ɕR�Â���ID
	*/
	template<typename TComponent>
	static std::size_t GetID() {
		static_assert(std::is_base_of<ComponentBase, TComponent>::value, "ComponentBase���p�����Ȃ���΂Ȃ�Ȃ��B");

		const std::type_index typeIndex(typeid(TComponent));

		std::lock_guard<std::mutex> lock(m_mutex);

		auto it = m_componentIDMap.find(typeIndex);
		if (it != m_componentIDMap.end()) {
			// ID�����Ƀ��X�g�ɑ��݂���ꍇ�A����ID��Ԃ�
			return it->second;
		}
		else {
			// ID�����X�g�ɑ��݂��Ȃ��ꍇ�A�V����ID�𐶐����ă��X�g�ɒǉ�
			const std::size_t newID = m_componentIDMap.size();
			m_componentIDMap[typeIndex] = newID;
			return newID;
		}
	}

private:
	static std::mutex m_mutex;
	static std::unordered_map<std::type_index, std::size_t> m_componentIDMap;	// �R���|�[�l���gID���X�g
};

#endif //!___CORESYSTEM_MODULE_COMPONENTIDGENERATOR_H___