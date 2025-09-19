#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "TextTemplate.h"

namespace
{
	constexpr float FULL_HD_WIDTH = 1920.0f;	// �t��HD�̉����B
	constexpr float FULL_HD_HEIGHT = 1080.0f;	// �t��HD�̏c���B

	constexpr float TITLE_LOGO_NORMAL_WIDTH = 240.0f;	// �ʏ�̃e�g���~�m�̉����B
	constexpr float TITLE_LOGO_HEIGHT = 400.0f;			// �e�g���~�m�̏c���B

	const std::string TITLE_LOGO_FILE_PATH = "Assets/Sprite/Title/Title_";		// �e�g���~�m�̃t�@�C���p�X�B
	const std::string EXTENSION_DDS = ".dds";									// �X�v���C�g�̊g���q�B

	ResultInfo PRESS_A_START =
	{
		"Press A Start",	Vector3(-350.0f, -200.0f, 0.0f),	float(2.0f),	Vector4(g_vec4White)
	};

	/// <summary>
	/// �^�C�g�����S�̏����i�[����\���́B
	/// </summary>
	struct TitleLogoSpriteInfo
	{
		const std::string filePath;	// �t�@�C������\�������B
		const Vector2 position;		// ���S�̈ʒu�B

		/// <summary>
		/// �t�@�C�����Ƀp�X�Ɗg���q��t�������t���p�X�̕�������擾���܂��B
		/// </summary>
		/// <returns> �t���p�X�B</returns>
		std::string GetFullPath()
		{
			return TITLE_LOGO_FILE_PATH + filePath + EXTENSION_DDS;
		}
	};

	/// <summary>
	/// "TETRIS"�̊e�����ɑΉ����郍�S���̔z��B
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
	// �^�C�g���w�i�̃X�v���C�g��ݒ�B
	m_titleSpriteRender.Init("Assets/Sprite/Title/TitleBack.dds", FULL_HD_WIDTH, FULL_HD_HEIGHT);

	// "Press Any Button"�̃e�L�X�g��ݒ�B
	CreatePressAtoStartText();

	// �^�C�g�����S�̃X�v���C�g��ݒ�B
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
/// �^�C�g�����S�̃X�v���C�g�𐶐����܂��B
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
/// �uPRESS A START�v�̃e�L�X�g���쐬���܂��B
/// </summary>
void Title::CreatePressAtoStartText()
{
	wchar_t wtext[128];
	MultiByteToWideChar(CP_UTF8, 0, PRESS_A_START.text.c_str(), -1, wtext, 128);
	SetTextOption(&m_pressAtoStartFontRender, PRESS_A_START, L"%s", wtext);
}
