#include "stdafx.h"
#include "ScoreManager.h"
#include "SoundManager.h"

namespace
{
	constexpr int SCORE_PER_LINE[] = { 100,300,500,800 };	// 消したライン数に応じたスコア加算値。
	constexpr int SCORE_PER_TSPIN[] = { 800,1200,1600 };	// Tスピンで消したライン数に応じたスコア加算値。
	constexpr int SCORE_PER_COMBO = 50;						// コンボ数に応じたスコア加算値。
	constexpr int LEVEL_UP_LINE_COUNT = 10;					// レベルアップに必要な消去ライン数。
	constexpr int FIRST_LEVEL = 1;							// 最初のレベル。
}

/// <summary>
/// 消した列の数に応じてスコアに加算します。
/// </summary>
/// <param name="clearLineCount">消した列の数。</param>
void ScoreManager::AddScore(bool isTSpin, int clearLineCount, int comboCount)
{
	// 不正な値が来たら無視。
	if (clearLineCount <= 0 || clearLineCount > static_cast<int>(LineClearType::LineClearType_Num)) {
		return;
	}
	// 消したラインの数に応じて「Single」～「Tetris」のカウントを増やす。
	m_lineClearTypeCounts[clearLineCount - 1]++;

	// 総消去ライン数を計算。
	AddTotalScore(isTSpin, clearLineCount, comboCount);

	// 総消去ライン数を加算。
	m_lineClearTotalCounts += clearLineCount;

	// レベルを更新。
	RecalculateLevel();
}

bool ScoreManager::Start()
{
	m_beforeLevel = FIRST_LEVEL;
	m_currentLevel = FIRST_LEVEL;
	return true;
}

/// <summary>
/// ラインクリア数に基づいて現在のレベルを更新します。
/// </summary>
void ScoreManager::RecalculateLevel()
{
	m_currentLevel = (m_lineClearTotalCounts / LEVEL_UP_LINE_COUNT) + FIRST_LEVEL;

	PlaySoundForLevelUp();
}

/// <summary>
/// レベルアップ時に効果音を再生します。
/// </summary>
void ScoreManager::PlaySoundForLevelUp()
{
	if (m_currentLevel > m_beforeLevel) {
		SoundManager* sound = FindGO<SoundManager>("SoundManager");
		sound->SoundNewGO(enSoundList_LevelUpSE);
		m_beforeLevel = m_currentLevel;
	}
}

/// <summary>
/// Tスピンの有無、ラインの同時消去数、コンボ数に応じて合計スコアを加算します。
/// </summary>
/// <param name="isTSpin">Tスピンの有無。</param>
/// <param name="clearLineCount">一度に消去したラインの数。</param>
/// <param name="lineClearComboCount">連続してラインを消去した回数（コンボ数）。</param>
void ScoreManager::AddTotalScore(bool isTSpin, int clearLineCount, int lineClearComboCount)
{
	// ラインの消去数と参照する配列の要素番号が一致するように調整。
	int elementNum = clearLineCount - 1;

	if (isTSpin) {
		m_totalScore += SCORE_PER_TSPIN[elementNum];
	}
	else {
		m_totalScore += SCORE_PER_LINE[elementNum];
	}

	m_totalScore += CalcComboScore(lineClearComboCount);
}

/// <summary>
/// 連続ライン消去数に基づいてコンボボーナススコアを計算します。
/// </summary>
/// <param name="lineClearComboCount">連続でラインを消した回数。</param>
/// <returns>計算されたコンボボーナススコア（int型）。</returns>
int ScoreManager::CalcComboScore(int lineClearComboCount)
{
	// 最大コンボ数の記録。
	m_maxCombos = max(m_maxCombos, lineClearComboCount);

	// 1回目のライン消去はコンボに含まないため。
	int comboCount = lineClearComboCount - 1;

	// 2回目以降のライン消去に対してコンボボーナスを加算。
	int comboBonus = comboCount * SCORE_PER_COMBO;
	return comboBonus;
}
