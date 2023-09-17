////////////////////////////////////////////////////////////////////////////////
// ファイル名: EntityIDGenerator.h
////////////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_ENTITYIDGENERATOR_H___
#define ___CORESYSTEM_MODULE_ENTITYIDGENERATOR_H___

/////////////////
// インクルード //
/////////////////
#include <atomic>

class EntityIDGenerator {
public:

	/*
	[関数概要]
	新規のIDを出力する

	[戻り値]
	std::size_t		新しいID
	*/
	template<typename TComponent>
	static std::size_t GetID() {
		return ++m_entityIdCounter;
	}

private:
	static std::atomic<std::size_t> m_entityIdCounter;
};

#endif //!___CORESYSTEM_MODULE_ENTITYIDGENERATOR_H___