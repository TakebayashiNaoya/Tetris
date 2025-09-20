#pragma once
#include "Types.h"
#include <queue>
#include <vector>
#include <random>
#include <algorithm>

namespace
{
	constexpr int NEXT_MINO_DISPLAY_COUNT = 6;	// ���ɕ\������e�g���~�m�̐��B
}

class NextTetriminoView :public IGameObject
{
public:
	/// <summary>
	/// ���ɐ��������e�g���~�m�̎�ނ��擾���A�z����X�V����B
	/// </summary>
	int GetNextMinoKind();

private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

	/// <summary>
	/// �\���p�z�������������B
	/// </summary>
	void SetupmDisplayMinos();

	/// <summary>
	/// �T���z�񂪋�ɂȂ�����7�o�b�O�����ŕ�[����B
	/// </summary>
	void RefillReserveQueue();

	/// <summary>
	/// �\���p�̔z��ɕK�v�ɉ����čT���z�񂩂��[����B
	/// </summary>
	void RefillDisplayQueueIfNeeded();

	std::array<EnMinoKinds, NEXT_MINO_DISPLAY_COUNT> m_displayMinos;						// ���ɐ��������e�g���~�m��\������z��B
	std::array<EnMinoKinds, static_cast<int>(EnMinoKinds::enMinoKinds_Num)> m_reserveMinos;	// ���ɐ��������e�g���~�m���T����z��B

	/// <summary>
	/// 7�o�b�O�����Ń����_���Ƀe�g���~�m�𐶐����邽�߂̃x�[�X�z��B
	/// </summary>
	std::vector<EnMinoKinds> m_base =
	{
		EnMinoKinds::enMinoKinds_I,
		EnMinoKinds::enMinoKinds_J,
		EnMinoKinds::enMinoKinds_L,
		EnMinoKinds::enMinoKinds_O,
		EnMinoKinds::enMinoKinds_S,
		EnMinoKinds::enMinoKinds_T,
		EnMinoKinds::enMinoKinds_Z
	};

	SpriteRender m_nextMinoSpriteRender[NEXT_MINO_DISPLAY_COUNT];	// ���ɐ��������e�g���~�m��\������X�v���C�g�����_�[�B
};

