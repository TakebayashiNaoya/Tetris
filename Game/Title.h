#pragma once

namespace
{
	constexpr int CHARACTER_COUNT_OF_TETRIS = 6;
}

class Title :public IGameObject
{
public:
	Title();

private:
	void Update() override final;
	void Render(RenderContext& rc) override final;

	/// <summary>
	/// �^�C�g�����S�̃X�v���C�g�𐶐����܂��B
	/// </summary>
	void CreateTitleLogo();

	void CreatePressAtoStartText();

	SpriteRender m_titleSpriteRender;
	SpriteRender m_titleLogoSpriteRender[CHARACTER_COUNT_OF_TETRIS];

	FontRender m_pressAtoStartFontRender;
};

