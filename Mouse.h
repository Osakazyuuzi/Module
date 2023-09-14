/////////////////////////////////////////////////////////////////////////
// �t�@�C�����FMouse.h
/////////////////////////////////////////////////////////////////////////
#ifndef ___CORESYSTEM_MODULE_MOUSE_H___
#define ___CORESYSTEM_MODULE_MOUSE_H___

/////////////////
// �C���N���[�h //
/////////////////
#include <shared_mutex>

//////////////////////////
// �I���W�i���C���N���[�h //
//////////////////////////
#include "Math.h"

class Mouse {
public:

	/*
	[�֐��T�v]
	�C���X�^���X�擾

	[�߂�l]
	Mouse&	�C���X�^���X�̎Q��
	*/
	static Mouse& GetInstance() {
		static Mouse instance;
		return instance;
	}

	/*
	[�֐��T�v]
	�}�E�X���W���X�V����i�������ݐ�p�j

	[����]
	int		x		X���W
	int		y		Y���W
	*/
	void UpdateMouseMove(int x, int y) {

		// ��L���b�N��������
		std::unique_lock<std::shared_mutex> lock(m_mutex);

		// �}�E�X���W���X�V����
		m_position = { x, y };
	}

	/*
	[�֐��T�v]
	�z�C�[����]�ʂ��X�V����i�������ݐ�p�j

	[����]
	float	wheelDelta		��]��
	*/
	void UpdateWheelDelta(float wheelDelta) {

		// ��L���b�N��������
		std::unique_lock<std::shared_mutex> lock(m_mutex);

		// �z�C�[����]�ʂ��X�V����
		m_wheelDelta = wheelDelta;
	}

	/*
	[�֐��T�v]
	�}�E�X���W���擾����i�ǂݎ���p�j

	[�߂�l]
	Float2		�}�E�X���W
	*/
	const Float2 GetPos() {

		// ���L���b�N��������
		std::shared_lock<std::shared_mutex> lock(m_mutex);

		// �}�E�X���W��Ԃ�
		return m_position;
	}

	/*
	[�֐��T�v]
	�}�E�X���WX���擾����i�ǂݎ���p�j

	[�߂�l]
	float		�}�E�X���WX
	*/
	const float GetPosX() {

		// ���L���b�N��������
		std::shared_lock<std::shared_mutex> lock(m_mutex);

		// �}�E�X���WX��Ԃ�
		return m_position.x;
	}

	/*
	[�֐��T�v]
	�}�E�X���WY���擾����i�ǂݎ���p�j

	[�߂�l]
	float		�}�E�X���WY
	*/
	const float GetPosY() {

		// ���L���b�N��������
		std::shared_lock<std::shared_mutex> lock(m_mutex);

		// �}�E�X���WY��Ԃ�
		return m_position.y;
	}

	/*
	[�֐��T�v]
	�z�C�[����]�ʂ��擾����i�ǂݎ���p�j

	[�߂�l]
	float			�z�C�[����]��
	*/
	const float GetWheelDelta() {

		// ���L���b�N��������
		std::shared_lock<std::shared_mutex> lock(m_mutex);


		// �z�C�[����]�ʂ�Ԃ�
		return m_wheelDelta;
	}

private:
	std::shared_mutex m_mutex;	// ���L
	Float2 m_position;			// �J�[�\�����W
	float  m_wheelDelta;		// �z�C�[����]��
};


#endif //!___CORESYSTEM_MODULE_MOUSE_H___