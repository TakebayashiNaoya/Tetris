#pragma once
class PauseView :public IGameObject
{
private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;
	SpriteRender m_backSpriteRender;	// �|�[�Y��ʗp�̃X�v���C�g�����_�[�B
	FontRender m_pauseFontRender;		// �|�[�Y�\���p�̃t�H���g�����_�[�B
	bool m_isPause = false;			// �|�[�Y��

public:
	bool GetIsPause() const
	{
		return m_isPause;
	}
};

