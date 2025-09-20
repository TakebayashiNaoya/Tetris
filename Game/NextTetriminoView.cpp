#include "stdafx.h"
#include "NextTetriminoView.h"
#include <time.h>

namespace
{
	constexpr float NEXT_MINO_VIEW_SCALE = 150.0f;		// ���ɕ\������e�g���~�m�̑傫���B
	constexpr float NEXT_MINO_VIEW_POS_X = 400.0f;		// ���ɕ\������e�g���~�m��X���W�B
	constexpr float NEXT_MINO_VIEW_POS_Y = 350.0f;		// ���ɕ\������e�g���~�m��Y���W�B
	constexpr float NEXT_MINO_VIEW_INTERVAL = 100.0f;	// ���ɕ\������e�g���~�m�̊Ԋu�B
	constexpr int INIT_RESERVE_QUEUE_NUM = 12;			// �T���z��̏������񐔁B

	const std::string TITLE_LOGO_FILE_PATH = "Assets/Sprite/Tetrimino/FullSizeMino_";	// �e�g���~�m�̃t�@�C���p�X�B
	const std::string EXTENSION_DDS = ".dds";											// �X�v���C�g�̊g���q�B
	constexpr char fileName[static_cast<int>(EnMinoKinds::enMinoKinds_Num)] =
	{ 'I','J','L','O','S','T','Z' };													// �t�@�C���̖��O�B

	/// <summary>
	/// �w�肳�ꂽ�t�@�C��������t���p�X������𐶐����܂��B
	/// </summary>
	/// <param name="fileName"> �t�@�C������\��������B</param>
	/// <returns> �t�@�C�����Ƀp�X�Ɗg���q��t�������t���p�X�̕�����B</returns>
	std::string GetFullPath(int num)
	{
		return TITLE_LOGO_FILE_PATH + fileName[num] + EXTENSION_DDS;
	}
}

int NextTetriminoView::GetNextMinoKind()
{
	int nextMino = static_cast<int>(m_displayMinos.front());


	RefillDisplayQueueIfNeeded();

	for (int i = 0; i < NEXT_MINO_DISPLAY_COUNT; i++) {
		// �X�v���C�g�̏������B
		m_nextMinoSpriteRender[i].Init(GetFullPath(static_cast<int>(m_displayMinos[i])).c_str(), NEXT_MINO_VIEW_SCALE, NEXT_MINO_VIEW_SCALE);
		m_nextMinoSpriteRender[i].Update();
	}

	return nextMino;
}

bool NextTetriminoView::Start()
{
	// �e�g���~�m�����S�����_���ɂ��鏈���B
	srand(time(nullptr));

	// �\���p�z��ƍT���z����������B
	SetupmDisplayMinos();

	return true;
}

void NextTetriminoView::Update()
{
}

void NextTetriminoView::Render(RenderContext& rc)
{
	for (auto& sprite : m_nextMinoSpriteRender) {
		sprite.Draw(rc);
	}
}

/// <summary>
/// �\���p�z�������������B
/// </summary>
void NextTetriminoView::SetupmDisplayMinos()
{
	// �X�v���C�g�����_�[�̈ʒu��ݒ�B
	for (int i = 0; i < NEXT_MINO_DISPLAY_COUNT; i++) {
		m_nextMinoSpriteRender[i].SetPosition(Vector3(NEXT_MINO_VIEW_POS_X, NEXT_MINO_VIEW_POS_Y - i * NEXT_MINO_VIEW_INTERVAL, 0.0f));
		m_nextMinoSpriteRender[i].Update();
	}

	// �z��̒��g���S��0�ɂȂ��Ă��邽�߁A12�񏈗����A���g��f���o���B
	for (int i = 0; i < INIT_RESERVE_QUEUE_NUM; i++) {
		RefillDisplayQueueIfNeeded();
	}

	// �X�v���C�g�̏������B
	for (int i = 0; i < NEXT_MINO_DISPLAY_COUNT; i++) {
		// �X�v���C�g�̏������B
		m_nextMinoSpriteRender[i].Init(GetFullPath(i).c_str(), NEXT_MINO_VIEW_SCALE, NEXT_MINO_VIEW_SCALE);
		m_nextMinoSpriteRender[i].Update();
	}
}

/// <summary>
/// �T������ɂȂ�����7�o�b�O�����ŕ�[����B
/// </summary>
void NextTetriminoView::RefillReserveQueue()
{
	// 7�o�b�O�����Ń����_���Ƀe�g���~�m�𐶐�����B
	random_shuffle(m_base.begin(), m_base.end());

	// �T���z��ɕ�[�B
	for (int i = 0; i < static_cast<int>(EnMinoKinds::enMinoKinds_Num); i++) {
		m_reserveMinos[i] = m_base[i];
	}
}

/// <summary>
/// �\���p�̔z��ɕK�v�ɉ����čT���z�񂩂��[����B
/// </summary>
void NextTetriminoView::RefillDisplayQueueIfNeeded()
{
	// �T������Ȃ��[�B
	if (m_reserveMinos.front() == EnMinoKinds::enMinoKinds_Num) {
		RefillReserveQueue(); // 7�o�b�O��[
	}

	// �\���p�z���1���O�ɂ��炷�B
	for (int i = 0; i < NEXT_MINO_DISPLAY_COUNT - 1; i++) {
		m_displayMinos[i] = m_displayMinos[i + 1];
		m_displayMinos[i + 1] = EnMinoKinds::enMinoKinds_Num;
	}

	// �T������1������[�B
	m_displayMinos[m_displayMinos.size() - 1] = m_reserveMinos.front();

	// �T���p�z���1���O�ɂ��炷�B
	for (int i = 0; i < static_cast<int>(EnMinoKinds::enMinoKinds_Num) - 1; i++) {
		m_reserveMinos[i] = m_reserveMinos[i + 1];
		m_reserveMinos[i + 1] = EnMinoKinds::enMinoKinds_Num;
	}
}