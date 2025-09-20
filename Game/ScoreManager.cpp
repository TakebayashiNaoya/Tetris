#include "stdafx.h"
#include "ScoreManager.h"

namespace
{
	constexpr int SCORE_PER_LINE[] = { 100,300,500,800 };	// ���������C�����ɉ������X�R�A���Z�l�B
	constexpr int LEVEL_UP_LINE_COUNT = 10;		// ���x���A�b�v�ɕK�v�ȏ������C�����B
	constexpr int FIRST_LEVEL = 1;			// �ŏ��̃��x���B
}

bool ScoreManager::Start()
{
	m_currentLevel = FIRST_LEVEL;
	return true;
}

/// <summary>
/// ���C���N���A���Ɋ�Â��Č��݂̃��x�����X�V���܂��B
/// </summary>
void ScoreManager::UpdateLevel()
{
	m_currentLevel = (m_lineClearTotalCounts / LEVEL_UP_LINE_COUNT) + FIRST_LEVEL;
}

/// <summary>
/// ��������̐��ɉ����ăX�R�A�ɉ��Z���܂��B
/// </summary>
/// <param name="clearLineCount">��������̐��B</param>
void ScoreManager::AddScore(int clearLineCount)
{
	// �s���Ȓl�������疳���B
	if (clearLineCount <= 0 || clearLineCount > static_cast<int>(LineClearType::LineClearType_Num))
	{
		return;
	}
	// ��������̐��ɉ����āA�Ή�����C���f�b�N�X�̃J�E���g�𑝂₷�B
	m_lineSimultaneouslyClearCounts[clearLineCount - 1]++;
	// �X�R�A�����Z�B
	m_totalScore += SCORE_PER_LINE[clearLineCount - 1];
	// ���������C���������Z�B
	m_lineClearTotalCounts += clearLineCount;

	UpdateLevel();
}