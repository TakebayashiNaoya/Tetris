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
	int m_lineSimultaneouslyClearCounts[static_cast<int>(LineClearType::LineClearType_Num)];	// ���������̃J�E���g�B
	int m_lineClearTotalCounts = 0;	// ���������C�����B
	int m_totalScore = 0;			// ���v�X�R�A�B
	int m_currentLevel = 0;			// ���݂̃��x���B

	bool Start()override final;

	/// <summary>
	/// ���x�����X�V���܂��B
	/// </summary>
	void UpdateLevel();

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

	/// <summary>
	/// �w�肵�����������̃J�E���g���擾���܂��B
	/// </summary>
	int GetLineClearCount(int type) const
	{
		return m_lineSimultaneouslyClearCounts[static_cast<int>(type)];
	}

	/// <summary>
	/// ���C���N���A�̍��v�񐔂��擾���܂��B
	/// </summary>
	/// <returns>���C���N���A�̍��v�񐔁iint�^�j�B</returns>
	int GetLineClearTotalCount() const
	{
		return m_lineClearTotalCounts;
	}

	/// <summary>
	/// ���݂̃��x�����擾���܂��B
	/// </summary>
	/// <returns>���݂̃��x����\�������l�B</returns>
	int GetLevel() const
	{
		return m_currentLevel;
	}
};

