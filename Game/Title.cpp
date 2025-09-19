#include "stdafx.h"
#include "Title.h"
#include "Game.h"

namespace
{
	constexpr float FULL_HD_WIDTH = 1920.0f;	// フルHDの横幅。
	constexpr float FULL_HD_HEIGHT = 1080.0f;	// フルHDの縦幅。

	constexpr float TITLE_LOGO_NORMAL_WIDTH = 240.0f;	// 通常のテトリミノの横幅。
	constexpr float TITLE_LOGO_HEIGHT = 400.0f;			// テトリミノの縦幅。

	const Vector3 POSITION_FOR_PRESS_A_START = { -430.0f, -200.0f, 0.0f };	// "Press Any Button"の位置。
	constexpr float SCALE_FOR_PRESS_A_START = 2.0f;								// "Press Any Button"の大きさ。

	const std::string TITLE_LOGO_FILE_PATH = "Assets/Sprite/Title/Title_";		// テトリミノのファイルパス。
	const std::string EXTENSION_DDS = ".dds";									// スプライトの拡張子。

	/// <summary>
	/// タイトルロゴの情報を格納する構造体。
	/// </summary>
	struct TitleLogoSpriteInfo
	{
		const std::string filePath;	// ファイル名を表す文字。
		const Vector2 position;		// ロゴの位置。

		/// <summary>
		/// ファイル名にパスと拡張子を付加したフルパスの文字列を取得します。
		/// </summary>
		/// <returns> フルパス。</returns>
		std::string GetFullPath()
		{
			return TITLE_LOGO_FILE_PATH + filePath + EXTENSION_DDS;
		}
	};

	/// <summary>
	/// "TETRIS"の各文字に対応するロゴ情報の配列。
	/// </summary>
	TitleLogoSpriteInfo titleLogoSpritesInfo[CHARACTER_COUNT_OF_TETRIS] =
	{
		{"FirstT",	{-640.0f,	100.0f}},
		{"E",		{-360.0f,	100.0f}},
		{"SecondT",	{-80.0f,	100.0f}},
		{"R",		{200.0f,	100.0f}},
		{"I",		{400.0f,	100.0f}},
		{"S",		{600.0f,	100.0f}},
	};

}

Title::Title()
{
	// タイトル背景のスプライトを設定。
	m_titleSpriteRender.Init("Assets/Sprite/Title/TitleBack.dds", FULL_HD_WIDTH, FULL_HD_HEIGHT);

	// "Press Any Button"のテキストを設定。
	CreatePressAtoStartText();

	// タイトルロゴのスプライトを設定。
	CreateTitleLogo();
}

void Title::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		DeleteGO(this);
		NewGO<Game>(0, "Game");
	}
}

void Title::Render(RenderContext& rc)
{
	m_titleSpriteRender.Draw(rc);
	m_pressAtoStartFontRender.Draw(rc);

	for (auto& sprite : m_titleLogoSpriteRender) {
		sprite.Draw(rc);
	}
}

/// <summary>
/// タイトルロゴのスプライトを生成します。
/// </summary>
void Title::CreateTitleLogo()
{
	for (int i = 0; i < CHARACTER_COUNT_OF_TETRIS; i++) {
		TitleLogoSpriteInfo tmp = titleLogoSpritesInfo[i];
		m_titleLogoSpriteRender[i].Init(tmp.GetFullPath().c_str(), TITLE_LOGO_NORMAL_WIDTH, TITLE_LOGO_HEIGHT);
		m_titleLogoSpriteRender[i].SetPosition({ tmp.position.x , tmp.position.y , 0.0f });
		m_titleLogoSpriteRender[i].Update();
	}
}

void Title::CreatePressAtoStartText()
{
	m_pressAtoStartFontRender.SetText(L"Press A Start");
	m_pressAtoStartFontRender.SetPosition(POSITION_FOR_PRESS_A_START);
	m_pressAtoStartFontRender.SetScale(SCALE_FOR_PRESS_A_START);
	m_pressAtoStartFontRender.SetColor(g_vec4White);
}
