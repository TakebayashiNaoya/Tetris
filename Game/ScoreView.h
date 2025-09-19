#pragma once
class ScoreManager;

class ScoreView :public IGameObject
{
private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

	FontRender m_viewScore;	// �X�R�A�\���p�̃t�H���g�����_�[�B

	ScoreManager* m_scoreManager = nullptr;
};

