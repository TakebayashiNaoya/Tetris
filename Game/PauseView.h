#pragma once
class PauseView :public IGameObject
{
private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

	SpriteRender m_backSpriteRender;	// ポーズ画面用のスプライトレンダー。
	FontRender m_pauseFontRender;		// ポーズ表示用のフォントレンダー。
	bool m_isPause = false;				// ポーズ中ならtrue。

public:
	/// <summary>
	/// 一時停止状態かどうかを取得します。
	/// </summary>
	/// <returns>一時停止中の場合は true、そうでない場合は false を返します。</returns>
	bool GetIsPause() const
	{
		return m_isPause;
	}
};

