/////////////////////////////////////////////////////////////////////////
// �t�@�C�����FWorld.h
/////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_WORLD_H___
#define ___CORESYSTEM_MODULE_WORLD_H___

/////////////////
// �C���N���[�h //
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
// �I���W�i���C���N���[�h //
//////////////////////////
#include "Entity.h"
#include "ComponentBase.h"
#include "ComponentIDGenerator.h"
#include "ComponentStorage.h"
#include "SystemBase.h"

class World {
private:
	std::string m_worldName;	// ���[���h��
	std::vector<Entity> m_entities;		// �G���e�B�e�B�̃f�[�^�z��
	std::deque<std::size_t> m_freeIndices;	// ���g�p�̃C���f�b�N�X�ێ��z��
	std::size_t m_capacity;	// �G���e�B�e�B�̗e��
	ComponentStorage m_componentStorage;	// �R���|�[�l���g�X�g���[�W
private:
	/*
	[�֐��T�v]
	�������̗e�ʂ��Ċ��蓖�Ă���

	[����]
	std::size_t	newCapacity	�Ċ��蓖�Ă���e��
	*/
	void ResizeCapacity(std::size_t newCapacity) {
		// ���݂̗e�ʂ��傫���T�C�Y��ݒ肵�悤�Ƃ��Ă��邩�`�F�b�N
		if (newCapacity < m_capacity) {
			// �G���[���b�Z�[�W���R���\�[���ɏo�͂���
			std::cerr << "Failed to ResizeCapacity!"
				<< "\nFile: " << __FILE__
				<< "\nLine: " << __LINE__ << std::endl;

			// ��O���X���[
			throw std::runtime_error("Failed to ResizeCapacity!");
		}

		// �G���e�B�e�B�z��̃T�C�Y��ݒ肷��
		m_entities.resize(newCapacity);

		// �R���|�[�l���g�X�g���[�W�̊e�R���|�[�l���g�z��̃T�C�Y���Ċ��蓖�Ă���
		std::apply([newCapacity](auto&... componentType) {
			(..., componentType.resize(newCapacity));
			}, m_componentStorage);

		// �V�������蓖�Ă�ꂽ�G���e�B�e�B������������
		for (std::size_t i = m_capacity; i < newCapacity; i++) {
		
			// �G���e�B�e�B���擾����
			Entity& e = m_entities[i];

			// ���g�����R���|�[�l���g�f�[�^�̏ꏊ�i�v�f�ԍ��j��ݒ肷��
			ECSUtils::SetIndex(e.m_identifierBit, i);

			// �G���e�B�e�B�̃o�[�W����������������
			ECSUtils::SetVersion(e.m_identifierBit, 0);

			// �R���|�[�l���g�����t���O������������
			e.m_componentBit.reset();

			// �G���e�B�e�B�͐����Ă��Ȃ����̂Ƃ���
			e.m_state = ECSUtils::EntityState::Destroyed;

			// ���g�p�C���f�b�N�X�Ƃ��ĕۑ�����
			m_freeIndices.push_back(i);
		}

		// �e�ʂ̑傫����ݒ肵�Ȃ���
		m_capacity = newCapacity;
	}

	/*
	[�֐��T�v]
	�������̗e�ʂ��Ċ��蓖�Ă��K�v�����肵�A�K�v�Ȃ�΍Ċ��蓖�Ă��s��
	*/
	void ResizeIfNeeded()
	{
		// �V�����쐬���ꂽEntity��ێ�����X�y�[�X������ꍇ�X�L�b�v����
		if (m_freeIndices.size() > 0) return;

		// ���l�傫������
		ResizeCapacity((m_capacity + 10) * 2);
	}

public:
	/*
	[�֐��T�v]
	�R���X�g���N�^
	*/
	World(std::string worldName)
		: m_worldName(worldName)
		, m_capacity(0)
	{
		// �������̒i�K�Ŋ��蓖�Ă���
		ResizeCapacity(100);
	}

	/*
	[�֐��T�v]
	�G���e�B�e�B�̑��݂��m�F����i�ǂݎ��j

	[����]
	ECSUtils::IdentifierBit	�G���e�B�e�B�̎��ʃr�b�g

	[�߂�l]
	bool	���݂���Ȃ�true, ���Ȃ��Ȃ�false��Ԃ�
	*/
	bool ExistEntity(ECSUtils::IdentifierBit bit)
	{
		Entity& e = m_entities[ECSUtils::GetIndex(bit).to_ullong()];
		return ((e.m_identifierBit == bit) && e.m_state != ECSUtils::EntityState::Destroyed);
	}

	/*
	[�֐��T�v]
	�G���e�B�e�B���擾����i�ǂݎ��j

	[����]
	ECSUtils::IdentifierBit	�G���e�B�e�B�̎��ʃr�b�g
	*/
	Entity& GetEntity(ECSUtils::IdentifierBit bit)
	{
		if (!ExistEntity(bit)) {
			// �G���[���b�Z�[�W���R���\�[���ɏo�͂���
			std::cerr << "Failed to GetEntity! : Not found entity."
				<< "\nFile: " << __FILE__
				<< "\nLine: " << __LINE__ << std::endl;

			// ��O���X���[
			throw std::runtime_error("Failed to GetEntity! : Not found entity.");
		}

		// �G���e�B�e�B��Ԃ�
		return m_entities[ECSUtils::GetIndex(bit).to_ullong()];
	}

	/*
	[�֐��T�v]
	�R���|�[�l���g���������Ă��邩��Ԃ�

	[�e���v���[�g����]
	typename TComp	�R���|�[�l���g

	[����]
	ECSUtils::IdentifierBit bit		�G���e�B�e�B�̎��ʃr�b�g

	[�߂�l]
	bool	�������Ă�����true,�������Ă��Ȃ����false��Ԃ�
	*/
	template <typename TComp>
	bool HasComponent(ECSUtils::IdentifierBit bit)
	{
		// �G���e�B�e�B�����݂��邩���m�F����
		if (!ExistEntity(bit)) {
			// �G���[���b�Z�[�W���R���\�[���ɏo�͂���
			std::cerr << "Failed to HasComponent! : Not found entity."
				<< "\nFile: " << __FILE__
				<< "\nLine: " << __LINE__ << std::endl;

			// ��O���X���[
			throw std::runtime_error("Failed to HasComponent! : Not found entity.");
		}

		// �������茋�ʂ�Ԃ�
		return m_entities[ECSUtils::GetIndex(bit).to_ullong()].m_componentBit.test(ComponentIDGenerator::GetID<TComp>());
	}

	/*
	[�֐��T�v]
	�R���|�[�l���g���擾����

	[�e���v���[�g����]
	typename TComp	�R���|�[�l���g

	[����]
	ECSUtils::IdentifierBit bit		�G���e�B�e�B�̎��ʃr�b�g

	[�߂�l]
	TComp&		�R���|�[�l���g�̎Q��
	*/
	template <typename TComp>
	TComp& GetComponent(ECSUtils::IdentifierBit bit) noexcept {

		// �R���|�[�l���g���������Ă��邩���m�F����
		if (!HasComponent<TComp>(bit)) {
			// �G���[���b�Z�[�W���R���\�[���ɏo�͂���
			std::cerr << "Failed to GetComponent! : Not found entity."
				<< "\nFile: " << __FILE__
				<< "\nLine: " << __LINE__ << std::endl;

			// ��O���X���[
			throw std::runtime_error("Failed to GetComponent! : Not found entity.");
		}

		// �R���|�[�l���g��Ԃ�
		return std::get<std::vector<TComp>>(m_componentStorage)[ECSUtils::GetIndex(bit).to_ullong()];
	}

	/*
	[�֐��T�v]
	�G���e�B�e�B�E�R���|�[�l���g��S�č폜����
	*/
	void Clear()
	{
		// �S�ẴG���e�B�e�B���폜����
		m_entities.clear();
	}

private:
	/*
	[�֐��T�v]
	�G���e�B�e�B���쐬���A���ʃr�b�g��Ԃ�
	���K�v�ɉ����ă������̍Ċ��蓖�Ă��s����B�i�������݁j

	[�߂�l]
	ECSUtils::IdentifierBit	�G���e�B�e�B�̎��ʃr�b�g
	*/
	ECSUtils::IdentifierBit CreateEntity()
	{
		// �����������p�\�����f���A
		// �K�v�ɉ����ă��������Ċ��蓖�Ă���
		ResizeIfNeeded();

		// ���p�\�ȃC���f�b�N�X���擾����
		std::size_t freeIndex = m_freeIndices.front();

		// �쐬�����G���e�B�e�B������������
		Entity& e(m_entities[freeIndex]);

		// �o�[�W�������X�V����
		ECSUtils::SetVersion(e.m_identifierBit,
			ECSUtils::GetVersion(e.m_identifierBit).to_ullong() + 1);

		// �����Ă��锻��ɂ���
		e.m_state = ECSUtils::EntityState::Active;

		// �쐬�����̂Ŏg�p�����C���f�b�N�X�����O����
		m_freeIndices.pop_front();

		// �V�����쐬���ꂽ�G���e�B�e�B�̎��ʃr�b�g��Ԃ�
		return e.m_identifierBit;
	}

	/*
	[�֐��T�v]
	�G���e�B�e�B���폜����i�ǂݎ�菑�����݁j

	[����]
	ECSUtils::IdentifierBit	�G���e�B�e�B�̎��ʃr�b�g
	*/
	void RemoveEntity(ECSUtils::IdentifierBit bit)
	{
		// ���݂��邩�`�F�b�N����
		if (!ExistEntity(bit)) {
			// �G���[���b�Z�[�W���R���\�[���ɏo�͂���
			std::cerr << "Failed to RemoveEntity!"
				<< "\nFile: " << __FILE__
				<< "\nLine: " << __LINE__ << std::endl;

			// ��O���X���[
			throw std::runtime_error("Failed to RemoveEntity!");
		}

		// �G���e�B�e�B���擾����
		Entity& e = GetEntity(bit);

		// �G���e�B�e�B�̃C���f�b�N�X���擾
		std::size_t entityIndex = ECSUtils::GetIndex(e.m_identifierBit).to_ullong();

		// ���������������
		e.m_componentBit.reset();

		// ���񂾔���ɂ���
		e.m_state = ECSUtils::EntityState::Destroyed;

		// ���p�\�ȃC���f�b�N�X�Ƃ��ēo�^����
		m_freeIndices.push_front(entityIndex);
	}

	/*
	[�֐��T�v]
	�R���|�[�l���g��ǉ�����

	[�e���v���[�g����]
	TComp	ComponentBase�N���X���p�������R���|�[�l���g�̌^

	[����]
	ECSUtils::IdentifierBit bit		�G���e�B�e�B�̎��ʃr�b�g
	*/
	template <typename TComp>
	void AddComponent(ECSUtils::IdentifierBit bit) 
	{
		// �G���e�B�e�B�����݂��邩�`�F�b�N����
		if (!ExistEntity(bit)) {
			// �G���[���b�Z�[�W���R���\�[���ɏo�͂���
			std::cerr << "Failed to AddComponent! : Not found entity."
				<< "\nFile: " << __FILE__
				<< "\nLine: " << __LINE__ << std::endl;

			// ��O���X���[
			throw std::runtime_error("Failed to AddComponent! : Not found entity.");
		}

		// �R���|�[�l���gID���擾����
		std::size_t compId = ComponentIDGenerator::GetID<TComp>();

		// �G���e�B�e�B��Index���擾����
		std::size_t entityIndex = ECSUtils::GetIndex(bit).to_ullong();

		// ���ɏ�������ł���ꍇ
		if (GetEntity(bit).m_componentBit.test(compId)) {
			// �G���[���b�Z�[�W���R���\�[���ɏo�͂���
			std::cerr << "Failed to AddComponent! : Already has component."
				<< "\nFile: " << __FILE__
				<< "\nLine: " << __LINE__ << std::endl;

			// ��O���X���[
			throw std::runtime_error("Failed to AddComponent! : Already has component.");
		}

		// �R���|�[�l���gID�̃t���O�𗧂āA�������Ă��邱�Ƃɂ���
		GetEntity(bit).m_componentBit.set(compId);

		// �w��̈ʒu�̃f�[�^������������
		// TODO:tuple�̌^�m�F
		// ���������ŃG���[���N������ComponentStorage.h�Ō^��o�^���Ă��Ȃ�
		auto& comp = std::get<std::vector<TComp>>(m_componentStorage)[entityIndex];
		comp.~TComp();			// �f�X�g���N�^
		new (&comp) TComp{};	// �R���X�g���N�^
	}

	/*
	[�֐��T�v]
	�R���|�[�l���g�����O����

	[�e���v���[�g����]
	TComp	ComponentBase�N���X���p�������R���|�[�l���g�̌^

	[����]
	ECSUtils::IdentifierBit bit		�G���e�B�e�B�̎��ʃr�b�g
	*/
	template <typename TComp>
	void RemoveComponent(ECSUtils::IdentifierBit bit)
	{
		// �G���e�B�e�B�����݂��邩�ǂ����`�F�b�N����
		if (!ExistEntity(bit)) {
			// �G���[���b�Z�[�W���R���\�[���ɏo�͂���
			std::cerr << "Failed to RemoveComponent! : Not found entity."
				<< "\nFile: " << __FILE__
				<< "\nLine: " << __LINE__ << std::endl;

			// ��O���X���[
			throw std::runtime_error("Failed to RemoveComponent! : Not found entity.");
		}

		// �������Ă��Ȃ����Ƃɂ���
		m_entities[ECSUtils::GetIndex(bit).to_ullong()].m_componentBit[ComponentIDGenerator::GetID<TComp>()] = false;
	}

private:
	//=== �R�}���h
	enum eCommand {
		eCreateEntity,
		eRemoveEntity,
		eAddComponent,
		eSetComponent,
		eRemoveComponent};
	// �R�}���h�x�[�X�N���X
	struct Command { 
		// ���s�֐�
		virtual void Execute(World* world) = 0;
	};
	// �G���e�B�e�B�쐬�R�}���h�N���X
	struct CommandCreateEntity : public Command {
		// ���s�֐�
		void Execute(World* world) override {
			// �G���e�B�e�B�𐶐����Ď��ʃr�b�g��ۑ�����
			world->m_setEntityIdentifierBit = world->CreateEntity();
		}
	};
	// �G���e�B�e�B�폜�R�}���h�N���X
	struct CommandRemoveEntity : public Command {
		CommandRemoveEntity(ECSUtils::IdentifierBit identifierBit) 
			: m_identifierBit(identifierBit) {}
		ECSUtils::IdentifierBit m_identifierBit;	// �폜����G���e�B�e�B���ʃr�b�g
		// ���s�֐�
		void Execute(World* world) override {
			// ���ʃr�b�g�������̃G���e�B�e�B���폜����
			world->RemoveEntity(m_identifierBit);
		}
	};
	// �R���|�[�l���g�ǉ��R�}���h�N���X
	template <typename T> struct CommandAddComponent : public Command {
		using TComp = T;	// �ǉ�����^
		// ���s�֐�
		void Execute(World* world) override {
			// �ۑ�����Ă��鎯�ʃr�b�g�ɃR���|�[�l���g��ǉ�����
			world->AddComponent<TComp>(m_setEntityIdentifierBit);
		}
	};
	// �R���|�[�l���g�ݒ�R�}���h�N���X
	template <typename T> struct CommandSetComponent : public Command {
		using TComp = T;	// �Z�b�g����^
		CommandSetComponent(T comp) 
			: m_component(comp) {}
		TComp m_component;	// �Z�b�g����R���|�[�l���g�f�[�^
		// ���s�֐�
		void Execute(World* world) override {
			// �ۑ�����Ă��鎯�ʃr�b�g�̃R���|�[�l���g�Ƀf�[�^��������
			world->GetComponent<TComp>(m_setEntityIdentifierBit) = m_component;
		}
	};
	// �R���|�[�l���g�폜�R�}���h�N���X
	template <typename T> struct CommandRemoveComponent : public Command {
		using TComp = T;	// �폜����^
		CommandRemoveComponent(ECSUtils::IdentifierBit identifierBit)
			: m_identifierBit(identifierBit) {}
		ECSUtils::IdentifierBit m_identifierBit;	// �폜����G���e�B�e�B���ʃr�b�g
		// ���s�֐�
		void Execute(World* world) override {
			// ���ʃr�b�g�������̃G���e�B�e�B�̓���̃R���|�[�l���g���폜����
			world->RemoveComponent<TComp>(m_identifierBit);
		}
	};
public:
	void AddCommandCreateEntity() {
		// �A�N�Z�X�𓯊�
		std::lock_guard<std::mutex> lock(m_commandMutex);
		// �R�}���h��ǉ�
		m_commandList.push(std::make_unique<CommandCreateEntity>());
	}

	void AddCommandRemoveEntity(ECSUtils::IdentifierBit identifierBit) {
		// �A�N�Z�X�𓯊�
		std::lock_guard<std::mutex> lock(m_commandMutex);
		// �R�}���h��ǉ�
		m_commandList.push(std::make_unique<CommandRemoveEntity>(identifierBit));
	}

	template <typename T>
	void AddCommandAddComponent() {
		// �A�N�Z�X�𓯊�
		std::lock_guard<std::mutex> lock(m_commandMutex);
		// �R�}���h��ǉ�
		m_commandList.push(std::make_unique<CommandAddComponent<T>>());
	}

	template <typename T>
	void SetCommandSetComponent(T comp) {
		// �A�N�Z�X�𓯊�
		std::lock_guard<std::mutex> lock(m_commandMutex);
		// �R�}���h��ǉ�
		m_commandList.push(std::make_unique<CommandSetComponent<T>>(comp));
	}

	/*
	[�֐��T�v]
	�L���[�̒����R�}���h�����ׂĎ��s����
	*/
	void CommandExecute() {
		// �S�ẴR�}���h�𔽕�����
		while (!m_commandList.empty()) {
			// �擪�̃R�}���h���擾����
			auto& command = m_commandList.front();
			// �R�}���h�����s����
			command->Execute(this);
			// ���s�����R�}���h���폜����
			m_commandList.pop();
		}
	}

private:
	std::mutex m_commandMutex;
	std::queue<std::unique_ptr<Command>> m_commandList;	// �R�}���h���X�g
	ECSUtils::IdentifierBit m_setEntityIdentifierBit;	// ���O�ɍ쐬�����G���e�B�e�B�̎��ʃr�b�g
private:
	//=== �V�X�e���Ǘ�
	std::vector<std::shared_ptr<SystemBase>> m_systemList;	// �V�X�e�����X�g
public:
	/*
	[�֐��T�v]
	�V�X�e���̒ǉ�

	[�e���v���[�g����]
	TSystem		�V�X�e���N���X

	[����]
	std::size_t	updateOrder		�X�V���ԍ�
	*/
	template <typename TSystem>
	void AddSystem(std::size_t updateOrder)
	{
		// SystemBase���p�����Ă��邩�`�F�b�N����
		static_assert(std::is_base_of<SystemBase, TSystem>::value, "SystemBase���p������K�v������܂��B");
		// �ǉ�����
		m_systemList.push_back(std::make_shared<TSystem>(this, updateOrder));
		// �������������Ă�
		m_systemList.back()->Init();
	}

	/*
	[�֐��T�v]
	�V�X�e�����X�V���Ƀ\�[�g����
	*/
	void SortSystem() {
		// �X�V�ԍ����Ƀ\�[�g
		std::sort(m_systemList.begin(), m_systemList.end(),
			[](const std::shared_ptr<SystemBase>& a, const std::shared_ptr<SystemBase>& b) {
				return a->GetUpdateOrder() < b->GetUpdateOrder();
			});
	}

	/*
	[�֐��T�v]
	�V�X�e���̍X�V

	[����]
	float		deltaTime
	*/
	void Update(float deltaTime) {
		// �X�V
		for (std::shared_ptr<SystemBase>& system : m_systemList)
			system->Update(deltaTime);
	}

	/*
	[�֐��T�v]
	�V�X�e���̕`��
	*/
	void Draw() {
		// �X�V
		for (std::shared_ptr<SystemBase>& system : m_systemList)
			system->Draw();
	}
};

#endif //!___CORESYSTEM_MODULE_WORLD_H___