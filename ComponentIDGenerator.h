////////////////////////////////////////////////////////////////////////////////
// ファイル名: ComponentIDGenerator.h
////////////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_COMPONENTIDGENERATOR_H___
#define ___CORESYSTEM_MODULE_COMPONENTIDGENERATOR_H___

/////////////////
// インクルード //
/////////////////
#include <unordered_map>
#include <typeindex>
#include <mutex>

//////////////////////////
// オリジナルインクルード //
//////////////////////////
#include "ComponentBase.h"

class ComponentIDGenerator
{
public:

	/*
	[関数概要]
	Componentの種類に紐づいたIDを取得する。
	登録されてなければ新規で登録する。（読み取り、書き込み）

	[テンプレート引数]
	TComponent		コンポーネントの型

	[戻り値]
	std::size_t		コンポーネントに紐づいたID
	*/
	template<typename TComponent>
	static std::size_t GetID() {
		static_assert(std::is_base_of<ComponentBase, TComponent>::value, "ComponentBaseを継承しなければならない。");

		const std::type_index typeIndex(typeid(TComponent));

		std::lock_guard<std::mutex> lock(m_mutex);

		auto it = m_componentIDMap.find(typeIndex);
		if (it != m_componentIDMap.end()) {
			// IDが既にリストに存在する場合、そのIDを返す
			return it->second;
		}
		else {
			// IDがリストに存在しない場合、新しいIDを生成してリストに追加
			const std::size_t newID = m_componentIDMap.size();
			m_componentIDMap[typeIndex] = newID;
			return newID;
		}
	}

private:
	static std::mutex m_mutex;
	static std::unordered_map<std::type_index, std::size_t> m_componentIDMap;	// コンポーネントIDリスト
};

#endif //!___CORESYSTEM_MODULE_COMPONENTIDGENERATOR_H___