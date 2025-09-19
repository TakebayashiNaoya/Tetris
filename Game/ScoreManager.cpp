#include "stdafx.h"
#include "ScoreManager.h"

namespace
{
	constexpr int SCORE_PER_LINE[] = { 100,300,500,800 };	// ���������C�����ɉ������X�R�A���Z�l�B
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
	lineClearCounts[clearLineCount - 1]++;
	// �X�R�A�����Z�B
	m_totalScore += SCORE_PER_LINE[clearLineCount - 1];
}