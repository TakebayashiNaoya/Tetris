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
	// 同時消しのカウント。
	int lineClearCounts[static_cast<int>(LineClearType::LineClearType_Num)];
	// 合計スコア。
	int m_totalScore = 0;

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

	int GetLineClearCount(int type) const
	{
		return lineClearCounts[static_cast<int>(type)];
	}
};

