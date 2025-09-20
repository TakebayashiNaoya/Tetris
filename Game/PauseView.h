#pragma once
class PauseView :public IGameObject
{
private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;
	SpriteRender m_backSpriteRender;	// ポーズ画面用のスプライトレンダー。
	FontRender m_pauseFontRender;		// ポーズ表示用のフォントレンダー。
	bool m_isPause = false;			// ポーズ中

public:
	bool GetIsPause() const
	{
		return m_isPause;
	}
};

