#pragma once

namespace
{
	constexpr int CHARACTER_COUNT_OF_TETRIS = 6;	// "TETRIS"の文字数。
}

class Title :public IGameObject
{
public:
	~Title();

private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;

	/// <summary>
	/// タイトルロゴのスプライトを生成します。
	/// </summary>
	void CreateTitleLogo();

	/// <summary>
	/// 「PRESS A START」のテキストを作成します。
	/// </summary>
	void CreatePressAtoStartText();

	SpriteRender m_titleSpriteRender;									// タイトル背景用のスプライトレンダー。
	SpriteRender m_titleLogoSpriteRender[CHARACTER_COUNT_OF_TETRIS];	// タイトルロゴ用のスプライトレンダー。
	FontRender m_pressAtoStartFontRender;								// 「PRESS A START」用のフォントレンダー。
};

