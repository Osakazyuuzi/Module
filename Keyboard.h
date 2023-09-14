/////////////////////////////////////////////////////////////////////////
// �t�@�C�����FKeyboard.h
/////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_KEYBOARD_H___
#define ___CORESYSTEM_MODULE_KEYBOARD_H___

/////////////////
// �C���N���[�h //
/////////////////
#include <array>
#include <Windows.h>
#include <shared_mutex>
#include <iostream>

class Keyboard {
public:

	/*
	[�֐��T�v]
	�R���X�g���N�^
	*/
	Keyboard() 
		: m_triggerState{}
		, m_pressState{}
		, m_releaseState{}
	{}
	
	/*
	[�֐��T�v]
	�C���X�^���X�擾

	[�߂�l]
	Keyboard&	�C���X�^���X�̎Q��
	*/
	static Keyboard& GetInstance() {
		static Keyboard instance;
		return instance;
	}

	/*
	[�֐��T�v]
	�L�[�����X�V����i�������ݐ�p�j
	
	[����]
	BYTE	key			�L�[
	bool	isPress		�L�[�̏��
	*/
	void UpdateKeyState(BYTE key, bool isPress) {

		// �L�[�ԍ���256�ȏゾ�����ꍇ
		if (key >= 256) {
			// �G���[���b�Z�[�W���R���\�[���ɏo�͂���
			std::cerr << "Failed to UpdateKeyState: "
				<< "\nFile: " << __FILE__
				<< "\nLine: " << __LINE__ << std::endl;

			return;
		}

		// ��L���b�N��������
		std::unique_lock<std::shared_mutex> lock(m_mutex);

		// ������Ă��邪�A��������񂪂Ȃ������ꍇ
		if (isPress && !m_pressState[key]) {
			// �����ꂽ�u�ԂƂ��ăt���O�𗧂Ă�
			m_triggerState[key] = true;
		}

		// ������Ă��Ȃ����A��������񂪂���ꍇ
		if (!isPress && m_pressState[key]) {
			// �������u�ԂƂ��ăt���O�𗧂Ă�
			m_releaseState[key] = true;
		}

		// ���݂̏�Ԃ��t���O�ɔ��f������
		m_pressState[key] = isPress;
	}

	/*
	[�֐��T�v]
	�w��̃L�[�������ꂽ�u�Ԃ��ǂ�����Ԃ��i�ǂݎ���p�j
	
	[����]
	BYTE	key		�L�[

	[�߂�l]
	bool	�����ꂽ�u�ԂȂ�true, �����łȂ����false��Ԃ�
	*/
	bool IsKeyTrigger(BYTE key) {

		// ���L���b�N��������
		std::shared_lock<std::shared_mutex> lock(m_mutex);

		// �g���K�[��Ԃ�߂�l�p�ɕێ�
		bool trigger = m_triggerState[key];

		// �g���K�[��Ԃ����Z�b�g
		m_triggerState[key] = false;

		// �g���K�[��Ԃ�Ԃ�
		return trigger;
	}

	/*
	[�֐��T�v]
	�w��̃L�[��������Ă��邩�ǂ�����Ԃ��i�ǂݎ���p�j

	[����]
	BYTE	key		�L�[

	[�߂�l]
	bool	������Ă���Ȃ�true, �����łȂ����false��Ԃ�
	*/
	bool IsKeyPress(BYTE key) {

		// ���L���b�N��������
		std::shared_lock<std::shared_mutex> lock(m_mutex);

		// �v���X��Ԃ�Ԃ�
		return m_pressState[key];
	}

	/*
	[�֐��T�v]
	�w��̃L�[�𗣂����u�Ԃ��ǂ�����Ԃ��i�ǂݎ���p�j

	[����]
	BYTE	key		�L�[

	[�߂�l]
	bool	�������u�ԂȂ�true, �����łȂ����false��Ԃ�
	*/
	bool IsKeyRelease(BYTE key) {

		// ���L���b�N��������
		std::shared_lock<std::shared_mutex> lock(m_mutex);

		// �����[�X��Ԃ�߂�l�p�ɕێ�
		bool release = m_releaseState[key];

		// �����[�X��Ԃ����Z�b�g
		m_releaseState[key] = false;

		// �����[�X��Ԃ�Ԃ�
		return release;
	}

private:
	std::shared_mutex m_mutex;	// ���L
	std::array<bool, 256> m_triggerState;		// �����n�߂��L�[�̃e�[�u��
	std::array<bool, 256> m_pressState;			// ���������Ă���L�[�̃e�[�u��
	std::array<bool, 256> m_releaseState;		// �������u�Ԃ̃L�[�̃e�[�u��
};

#endif //!___CORESYSTEM_MODULE_KEYBOARD_H___