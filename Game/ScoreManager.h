#pragma once

/// <summary>
/// 同時消しの種類。
/// </summary>
enum class LineClearType
{
	Single,
	Double,
	Triple,
	Tetris,
	LineClearType_Num
};

class ScoreManager :public IGameObject
{
public:
	/// <summary>
	/// 現在のスコアを取得します。
	/// </summary>
	/// <returns>現在のスコア。</returns>
	int GetScore() const
	{
		return m_totalScore;
	}

	/// <summary>
	/// 指定した同時消しのカウントを取得します。
	/// </summary>
	int GetLineClearCount(int type) const
	{
		return m_lineClearTypeCounts[static_cast<int>(type)];
	}

	/// <summary>
	/// ラインクリアの合計回数を取得します。
	/// </summary>
	/// <returns>ラインクリアの合計回数（int型）。</returns>
	int GetLineClearTotalCount() const
	{
		return m_lineClearTotalCounts;
	}

	/// <summary>
	/// 現在のレベルを取得します。
	/// </summary>
	/// <returns>現在のレベルを表す整数値。</returns>
	int GetLevel() const
	{
		return m_currentLevel;
	}

	/// <summary>
	/// 消した列の数に応じてスコアに加算します。
	/// </summary>
	/// <param name="clearLineCount">消した列の数。</param>
	void AddScore(bool isTSpin, int clearLineCount, int comboCount);

private:
	bool Start()override final;

	/// <summary>
	/// レベルを更新します。
	/// </summary>
	void RecalculateLevel();

	/// <summary>
	/// レベルアップ時にサウンドを再生します。
	/// </summary>
	void PlaySoundForLevelUp();

	/// <summary>
	/// Tスピンの有無、同時に消したライン数、コンボ数を基に、合計スコアを加算します。
	/// </summary>
	/// <param name="isTSpin">Tスピンが行われた場合はtrue。</param>
	/// <param name="clearLineCount">同時に消したライン数。</param>
	/// <param name="comboCount">現在のコンボ数。</param>
	void AddTotalScore(bool isTSpin, int clearLineCount, int comboCount);

	/// <summary>
	/// コンボ数に基づいてスコアを計算します。
	/// </summary>
	/// <param name="comboCount">計算に使用するコンボの回数。</param>
	/// <returns>計算されたコンボスコア（整数値）。</returns>
	int CalcComboScore(int comboCount);

	int m_lineClearTypeCounts[static_cast<int>(LineClearType::LineClearType_Num)];	// 同時消しのカウント。
	int m_lineClearTotalCounts = 0;	// 総消去ライン数。
	int m_totalScore = 0;			// 合計スコア。
	int m_currentLevel = 0;			// 現在のレベル。
	int m_beforeLevel = 0;			// 前のレベル。
	int m_maxCombos = 0;			// 最大コンボ数。
};

