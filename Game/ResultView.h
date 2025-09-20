#pragma once
#include "ScoreManager.h"

class Game;
class ScoreManager;

class ResultView :public IGameObject
{
public:
	~ResultView();

private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

	/// <summary>
	/// �uRESULT�v�̕\���B
	/// </summary>
	void ViewResultText();

	/// <summary>
	/// �e���������̃X�R�A�\���B
	/// </summary>
	void ViewLineClearScore();

	/// <summary>
	/// �S�s�N���A�̍��v�񐔂�\�����܂��B
	/// </summary>
	void ViewLineClearTotalCount();

	/// <summary>
	/// ���v�X�R�A�̕\���B
	/// </summary>
	void ViewTotalScore();

	/// <summary>
	/// �uPress A to Title�v�̕\���B
	/// </summary>
	void CreatePressAtoTitleText();

	SpriteRender m_backSpriteRender;	// ���ʕ\���p�̃X�v���C�g�����_�[�B

	FontRender m_viewLineClearScore[static_cast<int>(LineClearType::LineClearType_Num)];	// �X�R�A�\���p�̃t�H���g�����_�[�B
	FontRender m_resultFontRender;			// ���ʕ\���p�̃t�H���g�����_�[�B
	FontRender m_viewLineClearTotalCount;	// �����C�����\���p�̃t�H���g�����_�[�B
	FontRender m_viewTotalScore;			// ���v�X�R�A�\���p�̃t�H���g�����_�[�B
	FontRender m_pressAtoTitleFontRender;	// �^�C�g���֖߂�ē��p�̃t�H���g�����_�[�B

	Game* m_game = nullptr;					// �Q�[���N���X�B
	ScoreManager* m_scoreManager = nullptr;	// �X�R�A�}�l�[�W���[�B
};

