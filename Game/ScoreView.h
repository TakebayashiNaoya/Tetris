#pragma once
class ScoreManager;

class ScoreView :public IGameObject
{
private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

	void ViewCurrentTotalScore();
	void ViewCurrentLevel();
	void ViewLineClearTotalCount();

	SpriteRender m_backSpriteRender;	// �����w�i�p�̃X�v���C�g�����_�[�B
	FontRender m_viewScore;				// �X�R�A�\���p�̃t�H���g�����_�[�B
	FontRender m_viewLevel;				// ���x���\���p�̃t�H���g�����_�[�B
	FontRender m_viewLine;				// ���C���\���p�̃t�H���g�����_�[�B

	ScoreManager* m_scoreManager = nullptr;
};

