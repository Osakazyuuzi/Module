////////////////////////////////////////////////////////////////////////////////
// �t�@�C����: ComponentStorage.h
////////////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_COMPONENTSTORAGE_H___
#define ___CORESYSTEM_MODULE_COMPONENTSTORAGE_H___

/////////////////
// �C���N���[�h //
/////////////////
#include <vector>
#include <tuple>

//////////////////////////
// �I���W�i���C���N���[�h //
//////////////////////////
#include "Json/json.hpp"
// �e��R���|�[�l���g
#include "ComponentBase.h"	// ��


using ComponentStorage = std::tuple<
//��F	std::vector<ComponentType>,
	std::vector<ComponentBase>	// ��
>;

// TODO:json�ł̃R���|�[�l���g�X�g���[�W�̃V���A���C�Y�ƃf�V���A���C�Y�̊֐��쐬

#endif //!___CORESYSTEM_MODULE_COMPONENTSTORAGE_H___