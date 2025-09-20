#include "stdafx.h"
#include "ScoreManager.h"

namespace
{
	constexpr int SCORE_PER_LINE[] = { 100,300,500,800 };	// 消したライン数に応じたスコア加算値。
	constexpr int LEVEL_UP_LINE_COUNT = 10;		// レベルアップに必要な消去ライン数。
	constexpr int FIRST_LEVEL = 1;			// 最初のレベル。
}

bool ScoreManager::Start()
{
	m_currentLevel = FIRST_LEVEL;
	return true;
}

/// <summary>
/// ラインクリア数に基づいて現在のレベルを更新します。
/// </summary>
void ScoreManager::UpdateLevel()
{
	m_currentLevel = (m_lineClearTotalCounts / LEVEL_UP_LINE_COUNT) + FIRST_LEVEL;
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
	m_lineSimultaneouslyClearCounts[clearLineCount - 1]++;
	// スコアを加算。
	m_totalScore += SCORE_PER_LINE[clearLineCount - 1];
	// 総消去ライン数を加算。
	m_lineClearTotalCounts += clearLineCount;

	UpdateLevel();
}