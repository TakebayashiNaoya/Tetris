#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "TextTemplate.h"
#include "SoundManager.h"

namespace
{
	constexpr float FULL_HD_WIDTH = 1920.0f;	// フルHDの横幅。
	constexpr float FULL_HD_HEIGHT = 1080.0f;	// フルHDの縦幅。

	constexpr float TITLE_LOGO_NORMAL_WIDTH = 240.0f;	// 通常のテトリミノの横幅。
	constexpr float TITLE_LOGO_HEIGHT = 400.0f;			// テトリミノの縦幅。

	const std::string TITLE_LOGO_FILE_PATH = "Assets/Sprite/Title/Title_";	// テトリミノのファイルパス。
	const std::string EXTENSION_DDS = ".dds";								// スプライトの拡張子。

	/// <summary>
	/// 「PRESS A START」の表示設定情報。
	ResultInfo PRESS_A_START =
	{
		"Press A Start",	Vector3(-350.0f, -200.0f, 0.0f),	float(2.0f),	Vector4(g_vec4White)
	};

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
		{std::string("FirstT"),		Vector2(-640.0f,100.0f)},
		{std::string("E"),			Vector2(-360.0f,100.0f)},
		{std::string("SecondT"),	Vector2(-80.0f,	100.0f)},
		{std::string("R"),			Vector2(200.0f,	100.0f)},
		{std::string("I"),			Vector2(400.0f,	100.0f)},
		{std::string("S"),			Vector2(600.0f,	100.0f)},
	};

}

Title::~Title()
{
	SoundManager* sound = FindGO<SoundManager>("SoundManager");
	sound->SoundDeleteGO(enSoundList_TitleBGM);
}

bool Title::Start()
{
	SoundManager* soundManager = FindGO<SoundManager>("SoundManager");
	soundManager->SoundNewGO(enSoundList_TitleBGM);

	// タイトル背景のスプライトを設定。
	m_titleSpriteRender.Init("Assets/Sprite/Title/TitleBack.dds", FULL_HD_WIDTH, FULL_HD_HEIGHT);

	// "Press Any Button"のテキストを設定。
	CreatePressAtoStartText();

	// タイトルロゴのスプライトを設定。
	CreateTitleLogo();

	return true;
}

void Title::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		SoundManager* sound = FindGO<SoundManager>("SoundManager");
		sound->SoundNewGO(enSoundList_SelectSE);
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

/// <summary>
/// 「PRESS A START」のテキストを作成します。
/// </summary>
void Title::CreatePressAtoStartText()
{
	wchar_t wtext[128];
	MultiByteToWideChar(CP_UTF8, 0, PRESS_A_START.text.c_str(), -1, wtext, 128);
	SetTextOption(&m_pressAtoStartFontRender, PRESS_A_START, L"%s", wtext);
}
