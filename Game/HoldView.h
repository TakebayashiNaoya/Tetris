#pragma once

class FieldManager;

class HoldView :public IGameObject
{
public:
	/// <summary>
	/// ホールドの画像を更新します。
	/// </summary>
	void UpdateHoldView();

private:
	bool Start()override final;
	void Render(RenderContext& renderContext)override final;

	FieldManager* m_fieldManager = nullptr;

	SpriteRender m_holdViewSprite;		// ホールド表示用のスプライトレンダー。
	FontRender m_holdTextFontRender;	// 「HOLD」の文字表示用のフォントレンダー。
};

