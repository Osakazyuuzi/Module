/////////////////////////////////////////////////////////////////////////
// ファイル名：ECSUtils.h
/////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_ECSUTILS_H___
#define ___CORESYSTEM_MODULE_ECSUTILS_H___

/////////////////
// インクルード //
/////////////////
#include <bitset>

namespace ECSUtils
{
	//--- コンポーネントの最大種類数
	constexpr std::size_t MAX_COMPONENT_COUNT = 128;

	//--- エンティティの状態
	enum class EntityState {
		Active,		// アクティブ（更新・描画共に行われる）
		Inactive,	// 非アクティブ（更新・描画共に行われない）
		Destroyed};	// 存在しない

	//--- エンティティの識別ビット
	constexpr std::size_t ENTITY_IDENTIFIER_BIT = 64;
	constexpr std::size_t ENTITY_INDEX_BIT = ENTITY_IDENTIFIER_BIT / 2;	// 管理インデックスのビット数(上位ビット)
	constexpr std::size_t ENTITY_VERSION_BIT = ENTITY_IDENTIFIER_BIT - ENTITY_INDEX_BIT;	// バージョン番号のビット数（下位ビット）

	//--- 別名エイリアス
	using IdentifierBit = std::bitset<ECSUtils::ENTITY_IDENTIFIER_BIT>;
	using ComponentBit = std::bitset<ECSUtils::MAX_COMPONENT_COUNT>;
	using IndexBit = std::bitset<ENTITY_INDEX_BIT>;
	using VersionBit = std::bitset<ENTITY_VERSION_BIT>;

	//--- エンティティの識別ビット管理関数
	/*
	[関数概要]
	Entityを識別するBitsetの上位ビットにインデックスを設定する

	[引数]
	IdentifierBit&		bit		エンティティが持つ識別ビット
	std::size_t								index		設定するインデックス
	*/
	void SetIndex(_Out_ IdentifierBit& bit, _In_ std::size_t index)
	{
		// 下位ビットのみを残す
		bit &= std::bitset<ENTITY_IDENTIFIER_BIT>((1ull << ENTITY_VERSION_BIT) - 1);

		// 上位ビットに下位ビット分ずらした値をセットする
		bit |= std::bitset<ENTITY_IDENTIFIER_BIT>(index << ENTITY_VERSION_BIT);
	}
	/*
	[関数概要]
	Entityを識別するBitsetの下位ビットにバージョンを設定する

	[引数]
	IdentifierBit&		bit			エンティティが持つ識別ビット
	std::size_t								version		設定するバージョン
	*/
	void SetVersion(_Out_ IdentifierBit& bit, _In_ std::size_t version)
	{
		// 上位ビットのみを残す
		bit &= std::bitset<ENTITY_IDENTIFIER_BIT>(~((1ull << ENTITY_VERSION_BIT) - 1));

		// 下位ビットに値をセットする
		bit |= std::bitset<ENTITY_IDENTIFIER_BIT>(version);
	}

	/*
	[関数概要]
	Entityを識別するBitsetの上位ビットからインデックスを取得する

	[引数]
	const IdentifierBit& bit		エンティティが持つ識別ビット

	[戻り値]
	IndexBit		取得したインデックス
	*/
	IndexBit GetIndex(const IdentifierBit& bit)
	{
		return IndexBit((bit >> ENTITY_VERSION_BIT).to_ullong());
	}

	/*
	[関数概要]
	Entityを識別するBitsetの下位ビットからバージョンを取得する

	[引数]
	const IdentifierBit& bit		エンティティが持つ識別ビット

	[戻り値]
	VersionBit		取得したバージョン
	*/
	VersionBit GetVersion(const IdentifierBit& bit)
	{
		return VersionBit(bit.to_ullong() & ((1ull << ENTITY_VERSION_BIT) - 1));
	}
}

#endif //!___CORESYSTEM_MODULE_ECSUTILS_H___
