#include "stdafx.h"
#include "ScoreManager.h"

namespace
{
	constexpr int SCORE_PER_LINE[] = { 100,300,500,800 };	// 消したライン数に応じたスコア加算値。
}

/// <summary>
/// 消した列の数に応じてスコアに加算します。
/// </summary>
/// <param name="clearLineCount">消した列の数。</param>
void ScoreManager::AddScore(int clearLineCount)
{
	// 不正な値が来たら無視。
	if (clearLineCount <= 0 || clearLineCount > static_cast<int>(LineClearType::LineClearType_Num))
	{
		return;
	}
	// 消した列の数に応じて、対応するインデックスのカウントを増やす。
	lineClearCounts[clearLineCount - 1]++;
	// スコアを加算。
	m_totalScore += SCORE_PER_LINE[clearLineCount - 1];
}