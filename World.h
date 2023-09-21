/////////////////////////////////////////////////////////////////////////
// ファイル名：World.h
/////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_WORLD_H___
#define ___CORESYSTEM_MODULE_WORLD_H___

/////////////////
// インクルード //
/////////////////
#include <iostream>
#include <vector>
#include <cassert>
#include <string>
#include <memory>
#include <algorithm>
#include <deque>
#include <queue>

//////////////////////////
// オリジナルインクルード //
//////////////////////////
#include "Entity.h"
#include "ComponentBase.h"
#include "ComponentIDGenerator.h"
#include "ComponentStorage.h"
#include "SystemBase.h"

class World {
private:
	std::string m_worldName;	// ワールド名
	std::vector<Entity> m_entities;		// エンティティのデータ配列
	std::deque<std::size_t> m_freeIndices;	// 未使用のインデックス保持配列
	std::size_t m_capacity;	// エンティティの容量
	ComponentStorage m_componentStorage;	// コンポーネントストレージ
private:
	/*
	[関数概要]
	メモリの容量を再割り当てする

	[引数]
	std::size_t	newCapacity	再割り当てする容量
	*/
	void ResizeCapacity(std::size_t newCapacity) {
		// 現在の容量より大きいサイズを設定しようとしているかチェック
		if (newCapacity < m_capacity) {
			// エラーメッセージをコンソールに出力する
			std::cerr << "Failed to ResizeCapacity!"
				<< "\nFile: " << __FILE__
				<< "\nLine: " << __LINE__ << std::endl;

			// 例外をスロー
			throw std::runtime_error("Failed to ResizeCapacity!");
		}

		// エンティティ配列のサイズを設定する
		m_entities.resize(newCapacity);

		// コンポーネントストレージの各コンポーネント配列のサイズを再割り当てする
		std::apply([newCapacity](auto&... componentType) {
			(..., componentType.resize(newCapacity));
			}, m_componentStorage);

		// 新しく割り当てられたエンティティを初期化する
		for (std::size_t i = m_capacity; i < newCapacity; i++) {
		
			// エンティティを取得する
			Entity& e = m_entities[i];

			// 自身が持つコンポーネントデータの場所（要素番号）を設定する
			ECSUtils::SetIndex(e.m_identifierBit, i);

			// エンティティのバージョンを初期化する
			ECSUtils::SetVersion(e.m_identifierBit, 0);

			// コンポーネント所持フラグを初期化する
			e.m_componentBit.reset();

			// エンティティは生きていないものとする
			e.m_state = ECSUtils::EntityState::Destroyed;

			// 未使用インデックスとして保存する
			m_freeIndices.push_back(i);
		}

		// 容量の大きさを設定しなおす
		m_capacity = newCapacity;
	}

	/*
	[関数概要]
	メモリの容量を再割り当てが必要か判定し、必要ならば再割り当てを行う
	*/
	void ResizeIfNeeded()
	{
		// 新しく作成されたEntityを保持するスペースがある場合スキップする
		if (m_freeIndices.size() > 0) return;

		// 一定値大きくする
		ResizeCapacity((m_capacity + 10) * 2);
	}

public:
	/*
	[関数概要]
	コンストラクタ
	*/
	World(std::string worldName)
		: m_worldName(worldName)
		, m_capacity(0)
	{
		// 初期化の段階で割り当てする
		ResizeCapacity(100);
	}

	/*
	[関数概要]
	エンティティの存在を確認する（読み取り）

	[引数]
	ECSUtils::IdentifierBit	エンティティの識別ビット

	[戻り値]
	bool	存在するならtrue, しないならfalseを返す
	*/
	bool ExistEntity(ECSUtils::IdentifierBit bit)
	{
		Entity& e = m_entities[ECSUtils::GetIndex(bit).to_ullong()];
		return ((e.m_identifierBit == bit) && e.m_state != ECSUtils::EntityState::Destroyed);
	}

	/*
	[関数概要]
	エンティティを取得する（読み取り）

	[引数]
	ECSUtils::IdentifierBit	エンティティの識別ビット
	*/
	Entity& GetEntity(ECSUtils::IdentifierBit bit)
	{
		if (!ExistEntity(bit)) {
			// エラーメッセージをコンソールに出力する
			std::cerr << "Failed to GetEntity! : Not found entity."
				<< "\nFile: " << __FILE__
				<< "\nLine: " << __LINE__ << std::endl;

			// 例外をスロー
			throw std::runtime_error("Failed to GetEntity! : Not found entity.");
		}

		// エンティティを返す
		return m_entities[ECSUtils::GetIndex(bit).to_ullong()];
	}

	/*
	[関数概要]
	コンポーネントを所持しているかを返す

	[テンプレート引数]
	typename TComp	コンポーネント

	[引数]
	ECSUtils::IdentifierBit bit		エンティティの識別ビット

	[戻り値]
	bool	所持していたらtrue,所持していなければfalseを返す
	*/
	template <typename TComp>
	bool HasComponent(ECSUtils::IdentifierBit bit)
	{
		// エンティティが存在するかを確認する
		if (!ExistEntity(bit)) {
			// エラーメッセージをコンソールに出力する
			std::cerr << "Failed to HasComponent! : Not found entity."
				<< "\nFile: " << __FILE__
				<< "\nLine: " << __LINE__ << std::endl;

			// 例外をスロー
			throw std::runtime_error("Failed to HasComponent! : Not found entity.");
		}

		// 所持判定結果を返す
		return m_entities[ECSUtils::GetIndex(bit).to_ullong()].m_componentBit.test(ComponentIDGenerator::GetID<TComp>());
	}

	/*
	[関数概要]
	コンポーネントを取得する

	[テンプレート引数]
	typename TComp	コンポーネント

	[引数]
	ECSUtils::IdentifierBit bit		エンティティの識別ビット

	[戻り値]
	TComp&		コンポーネントの参照
	*/
	template <typename TComp>
	TComp& GetComponent(ECSUtils::IdentifierBit bit) noexcept {

		// コンポーネントを所持しているかを確認する
		if (!HasComponent<TComp>(bit)) {
			// エラーメッセージをコンソールに出力する
			std::cerr << "Failed to GetComponent! : Not found entity."
				<< "\nFile: " << __FILE__
				<< "\nLine: " << __LINE__ << std::endl;

			// 例外をスロー
			throw std::runtime_error("Failed to GetComponent! : Not found entity.");
		}

		// コンポーネントを返す
		return std::get<std::vector<TComp>>(m_componentStorage)[ECSUtils::GetIndex(bit).to_ullong()];
	}

	/*
	[関数概要]
	エンティティ・コンポーネントを全て削除する
	*/
	void Clear()
	{
		// 全てのエンティティを削除する
		m_entities.clear();
	}

private:
	/*
	[関数概要]
	エンティティを作成し、識別ビットを返す
	※必要に応じてメモリの再割り当てが行われる。（書き込み）

	[戻り値]
	ECSUtils::IdentifierBit	エンティティの識別ビット
	*/
	ECSUtils::IdentifierBit CreateEntity()
	{
		// メモリが利用可能か判断し、
		// 必要に応じてメモリを再割り当てする
		ResizeIfNeeded();

		// 利用可能なインデックスを取得する
		std::size_t freeIndex = m_freeIndices.front();

		// 作成したエンティティを初期化する
		Entity& e(m_entities[freeIndex]);

		// バージョンを更新する
		ECSUtils::SetVersion(e.m_identifierBit,
			ECSUtils::GetVersion(e.m_identifierBit).to_ullong() + 1);

		// 生きている判定にする
		e.m_state = ECSUtils::EntityState::Active;

		// 作成したので使用したインデックスを除外する
		m_freeIndices.pop_front();

		// 新しく作成されたエンティティの識別ビットを返す
		return e.m_identifierBit;
	}

	/*
	[関数概要]
	エンティティを削除する（読み取り書き込み）

	[引数]
	ECSUtils::IdentifierBit	エンティティの識別ビット
	*/
	void RemoveEntity(ECSUtils::IdentifierBit bit)
	{
		// 存在するかチェックする
		if (!ExistEntity(bit)) {
			// エラーメッセージをコンソールに出力する
			std::cerr << "Failed to RemoveEntity!"
				<< "\nFile: " << __FILE__
				<< "\nLine: " << __LINE__ << std::endl;

			// 例外をスロー
			throw std::runtime_error("Failed to RemoveEntity!");
		}

		// エンティティを取得する
		Entity& e = GetEntity(bit);

		// エンティティのインデックスを取得
		std::size_t entityIndex = ECSUtils::GetIndex(e.m_identifierBit).to_ullong();

		// 所持判定を下げる
		e.m_componentBit.reset();

		// 死んだ判定にする
		e.m_state = ECSUtils::EntityState::Destroyed;

		// 利用可能なインデックスとして登録する
		m_freeIndices.push_front(entityIndex);
	}

	/*
	[関数概要]
	コンポーネントを追加する

	[テンプレート引数]
	TComp	ComponentBaseクラスを継承したコンポーネントの型

	[引数]
	ECSUtils::IdentifierBit bit		エンティティの識別ビット
	*/
	template <typename TComp>
	void AddComponent(ECSUtils::IdentifierBit bit) 
	{
		// エンティティが存在するかチェックする
		if (!ExistEntity(bit)) {
			// エラーメッセージをコンソールに出力する
			std::cerr << "Failed to AddComponent! : Not found entity."
				<< "\nFile: " << __FILE__
				<< "\nLine: " << __LINE__ << std::endl;

			// 例外をスロー
			throw std::runtime_error("Failed to AddComponent! : Not found entity.");
		}

		// コンポーネントIDを取得する
		std::size_t compId = ComponentIDGenerator::GetID<TComp>();

		// エンティティのIndexを取得する
		std::size_t entityIndex = ECSUtils::GetIndex(bit).to_ullong();

		// 既に所持判定である場合
		if (GetEntity(bit).m_componentBit.test(compId)) {
			// エラーメッセージをコンソールに出力する
			std::cerr << "Failed to AddComponent! : Already has component."
				<< "\nFile: " << __FILE__
				<< "\nLine: " << __LINE__ << std::endl;

			// 例外をスロー
			throw std::runtime_error("Failed to AddComponent! : Already has component.");
		}

		// コンポーネントIDのフラグを立て、所持していることにする
		GetEntity(bit).m_componentBit.set(compId);

		// 指定の位置のデータを初期化する
		// TODO:tupleの型確認
		// もしここでエラーが起きたらComponentStorage.hで型を登録していない
		auto& comp = std::get<std::vector<TComp>>(m_componentStorage)[entityIndex];
		comp.~TComp();			// デストラクタ
		new (&comp) TComp{};	// コンストラクタ
	}

	/*
	[関数概要]
	コンポーネントを除外する

	[テンプレート引数]
	TComp	ComponentBaseクラスを継承したコンポーネントの型

	[引数]
	ECSUtils::IdentifierBit bit		エンティティの識別ビット
	*/
	template <typename TComp>
	void RemoveComponent(ECSUtils::IdentifierBit bit)
	{
		// エンティティが存在するかどうかチェックする
		if (!ExistEntity(bit)) {
			// エラーメッセージをコンソールに出力する
			std::cerr << "Failed to RemoveComponent! : Not found entity."
				<< "\nFile: " << __FILE__
				<< "\nLine: " << __LINE__ << std::endl;

			// 例外をスロー
			throw std::runtime_error("Failed to RemoveComponent! : Not found entity.");
		}

		// 所持していないことにする
		m_entities[ECSUtils::GetIndex(bit).to_ullong()].m_componentBit[ComponentIDGenerator::GetID<TComp>()] = false;
	}

private:
	//=== コマンド
	enum eCommand {
		eCreateEntity,
		eRemoveEntity,
		eAddComponent,
		eSetComponent,
		eRemoveComponent};
	// コマンドベースクラス
	struct Command { 
		// 実行関数
		virtual void Execute(World* world) = 0;
	};
	// エンティティ作成コマンドクラス
	struct CommandCreateEntity : public Command {
		// 実行関数
		void Execute(World* world) override {
			// エンティティを生成して識別ビットを保存する
			world->m_setEntityIdentifierBit = world->CreateEntity();
		}
	};
	// エンティティ削除コマンドクラス
	struct CommandRemoveEntity : public Command {
		CommandRemoveEntity(ECSUtils::IdentifierBit identifierBit) 
			: m_identifierBit(identifierBit) {}
		ECSUtils::IdentifierBit m_identifierBit;	// 削除するエンティティ識別ビット
		// 実行関数
		void Execute(World* world) override {
			// 識別ビットから特定のエンティティを削除する
			world->RemoveEntity(m_identifierBit);
		}
	};
	// コンポーネント追加コマンドクラス
	template <typename T> struct CommandAddComponent : public Command {
		using TComp = T;	// 追加する型
		// 実行関数
		void Execute(World* world) override {
			// 保存されている識別ビットにコンポーネントを追加する
			world->AddComponent<TComp>(m_setEntityIdentifierBit);
		}
	};
	// コンポーネント設定コマンドクラス
	template <typename T> struct CommandSetComponent : public Command {
		using TComp = T;	// セットする型
		CommandSetComponent(T comp) 
			: m_component(comp) {}
		TComp m_component;	// セットするコンポーネントデータ
		// 実行関数
		void Execute(World* world) override {
			// 保存されている識別ビットのコンポーネントにデータを代入する
			world->GetComponent<TComp>(m_setEntityIdentifierBit) = m_component;
		}
	};
	// コンポーネント削除コマンドクラス
	template <typename T> struct CommandRemoveComponent : public Command {
		using TComp = T;	// 削除する型
		CommandRemoveComponent(ECSUtils::IdentifierBit identifierBit)
			: m_identifierBit(identifierBit) {}
		ECSUtils::IdentifierBit m_identifierBit;	// 削除するエンティティ識別ビット
		// 実行関数
		void Execute(World* world) override {
			// 識別ビットから特定のエンティティの特定のコンポーネントを削除する
			world->RemoveComponent<TComp>(m_identifierBit);
		}
	};
public:
	void AddCommandCreateEntity() {
		// アクセスを同期
		std::lock_guard<std::mutex> lock(m_commandMutex);
		// コマンドを追加
		m_commandList.push(std::make_unique<CommandCreateEntity>());
	}

	void AddCommandRemoveEntity(ECSUtils::IdentifierBit identifierBit) {
		// アクセスを同期
		std::lock_guard<std::mutex> lock(m_commandMutex);
		// コマンドを追加
		m_commandList.push(std::make_unique<CommandRemoveEntity>(identifierBit));
	}

	template <typename T>
	void AddCommandAddComponent() {
		// アクセスを同期
		std::lock_guard<std::mutex> lock(m_commandMutex);
		// コマンドを追加
		m_commandList.push(std::make_unique<CommandAddComponent<T>>());
	}

	template <typename T>
	void SetCommandSetComponent(T comp) {
		// アクセスを同期
		std::lock_guard<std::mutex> lock(m_commandMutex);
		// コマンドを追加
		m_commandList.push(std::make_unique<CommandSetComponent<T>>(comp));
	}

	/*
	[関数概要]
	キューの中をコマンドをすべて実行する
	*/
	void CommandExecute() {
		// 全てのコマンドを反復する
		while (!m_commandList.empty()) {
			// 先頭のコマンドを取得する
			auto& command = m_commandList.front();
			// コマンドを実行する
			command->Execute(this);
			// 実行したコマンドを削除する
			m_commandList.pop();
		}
	}

private:
	std::mutex m_commandMutex;
	std::queue<std::unique_ptr<Command>> m_commandList;	// コマンドリスト
	ECSUtils::IdentifierBit m_setEntityIdentifierBit;	// 直前に作成したエンティティの識別ビット
private:
	//=== システム管理
	std::vector<std::shared_ptr<SystemBase>> m_systemList;	// システムリスト
public:
	/*
	[関数概要]
	システムの追加

	[テンプレート引数]
	TSystem		システムクラス

	[引数]
	std::size_t	updateOrder		更新順番号
	*/
	template <typename TSystem>
	void AddSystem(std::size_t updateOrder)
	{
		// SystemBaseを継承しているかチェックする
		static_assert(std::is_base_of<SystemBase, TSystem>::value, "SystemBaseを継承する必要があります。");
		// 追加する
		m_systemList.push_back(std::make_shared<TSystem>(this, updateOrder));
		// 初期化処理を呼ぶ
		m_systemList.back()->Init();
	}

	/*
	[関数概要]
	システムを更新順にソートする
	*/
	void SortSystem() {
		// 更新番号順にソート
		std::sort(m_systemList.begin(), m_systemList.end(),
			[](const std::shared_ptr<SystemBase>& a, const std::shared_ptr<SystemBase>& b) {
				return a->GetUpdateOrder() < b->GetUpdateOrder();
			});
	}

	/*
	[関数概要]
	システムの更新

	[引数]
	float		deltaTime
	*/
	void Update(float deltaTime) {
		// 更新
		for (std::shared_ptr<SystemBase>& system : m_systemList)
			system->Update(deltaTime);
	}

	/*
	[関数概要]
	システムの描画
	*/
	void Draw() {
		// 更新
		for (std::shared_ptr<SystemBase>& system : m_systemList)
			system->Draw();
	}
};

#endif //!___CORESYSTEM_MODULE_WORLD_H___