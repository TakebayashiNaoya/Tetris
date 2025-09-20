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
private:
	int m_lineSimultaneouslyClearCounts[static_cast<int>(LineClearType::LineClearType_Num)];	// 同時消しのカウント。
	int m_lineClearTotalCounts = 0;	// 総消去ライン数。
	int m_totalScore = 0;			// 合計スコア。
	int m_currentLevel = 0;			// 現在のレベル。

	bool Start()override final;

	/// <summary>
	/// レベルを更新します。
	/// </summary>
	void UpdateLevel();

public:
	/// <summary>
	/// 消した列の数に応じてスコアに加算します。
	/// </summary>
	/// <param name="clearLineCount">消した列の数。</param>
	void AddScore(int clearLineCount);

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
		return m_lineSimultaneouslyClearCounts[static_cast<int>(type)];
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
};

