#pragma once

/// <summary>
/// ���������̎�ށB
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
	// ���������̃J�E���g�B
	int lineClearCounts[static_cast<int>(LineClearType::LineClearType_Num)];
	// ���v�X�R�A�B
	int m_totalScore = 0;

public:
	/// <summary>
	/// ��������̐��ɉ����ăX�R�A�ɉ��Z���܂��B
	/// </summary>
	/// <param name="clearLineCount">��������̐��B</param>
	void AddScore(int clearLineCount);

	/// <summary>
	/// ���݂̃X�R�A���擾���܂��B
	/// </summary>
	/// <returns>���݂̃X�R�A�B</returns>
	int GetScore() const
	{
		return m_totalScore;
	}

	int GetLineClearCount(int type) const
	{
		return lineClearCounts[static_cast<int>(type)];
	}
};

