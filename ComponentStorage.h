////////////////////////////////////////////////////////////////////////////////
// ファイル名: ComponentStorage.h
////////////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_COMPONENTSTORAGE_H___
#define ___CORESYSTEM_MODULE_COMPONENTSTORAGE_H___

/////////////////
// インクルード //
/////////////////
#include <vector>
#include <tuple>

//////////////////////////
// オリジナルインクルード //
//////////////////////////
#include "Json/json.hpp"
// 各種コンポーネント
#include "ComponentBase.h"	// 仮


using ComponentStorage = std::tuple<
//例：	std::vector<ComponentType>,
	std::vector<ComponentBase>	// 仮
>;

// TODO:jsonでのコンポーネントストレージのシリアライズとデシリアライズの関数作成

#endif //!___CORESYSTEM_MODULE_COMPONENTSTORAGE_H___