/////////////////////////////////////////////////////////////////////////
// ファイル名：ECSUtils.h
/////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_ECSUTILS_H___
#define ___CORESYSTEM_MODULE_ECSUTILS_H___

/////////////////
// インクルード //
/////////////////
#include <bitset>

namespace ECSUntils
{
	//--- コンポーネントの最大種類数
	constexpr std::size_t MAX_COMPONENT_COUNT = 128;

	//--- エンティティの状態
	enum class EntityState {
		Active,		// アクティブ（更新・描画共に行われる）
		Inactive,	// 非アクティブ（更新・描画共に行われない）
		Destroyed};	// 存在しない

	//--- エンティティの識別ビット
	constexpr std::size_t ENTITY_IDENTIFIER_BIT = 32;
	constexpr std::size_t ENTITY_UNIQUEID_BIT = ENTITY_IDENTIFIER_BIT / 2;	// ユニークIDのビット数(上位ビット)
	constexpr std::size_t ENTITY_VERSION_BIT = ENTITY_IDENTIFIER_BIT - ENTITY_UNIQUEID_BIT;	// バージョン番号のビット数（下位ビット）

	//--- 別名エイリアス
	using IdentifierBit = std::bitset<ECSUntils::ENTITY_IDENTIFIER_BIT>;
	using ComponentBit = std::bitset<ECSUntils::MAX_COMPONENT_COUNT>;

	//--- エンティティの識別ビット管理関数
	/*
	[関数概要]
	Entityを識別するBitsetの上位ビットにIDを設定する

	[引数]
	std::bitset<ENTITY_IDENTIFIER_BIT>&		bit		エンティティが持つ識別ビット
	std::size_t								id		設定するID
	*/
	void SetUniqueID(_Out_ std::bitset<ENTITY_IDENTIFIER_BIT>& bit, _In_ std::size_t id)
	{
		// 下位ビットのみを残す
		bit &= std::bitset<ENTITY_IDENTIFIER_BIT>((1ull << ENTITY_VERSION_BIT) - 1);

		// 上位ビットに下位ビット分ずらした値をセットする
		bit |= std::bitset<ENTITY_IDENTIFIER_BIT>(id << ENTITY_VERSION_BIT);
	}
	/*
	[関数概要]
	Entityを識別するBitsetの下位ビットにバージョンを設定する

	[引数]
	std::bitset<ENTITY_IDENTIFIER_BIT>&		bit			エンティティが持つ識別ビット
	std::size_t								version		設定するバージョン
	*/
	void SetVersion(_Out_ std::bitset<ENTITY_IDENTIFIER_BIT>& bit, _In_ std::size_t version)
	{
		// 上位ビットのみを残す
		bit &= std::bitset<ENTITY_IDENTIFIER_BIT>(~((1ull << ENTITY_VERSION_BIT) - 1));

		// 下位ビットに値をセットする
		bit |= std::bitset<ENTITY_IDENTIFIER_BIT>(version);
	}
}

#endif //!___CORESYSTEM_MODULE_ECSUTILS_H___