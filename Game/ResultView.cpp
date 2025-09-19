#include "stdafx.h"
#include "ResultView.h"
#include "Game.h"
#include "Title.h"
#include "TextTemplate.h"

namespace
{
	const Vector3 POSITION_FOR_PRESS_A_TITLE = { -430.0f, -200.0f, 0.0f };	// "Press Any Button"の位置。
	constexpr float SCALE_FOR_PRESS_A_TITLE = 2.0f;							// "Press Any Button"の大きさ。

	constexpr float BACK_BLACK_WIDTH = 1200.0f;		// 黒幕背景の横幅。
	constexpr float BACK_BLACK_HEIGHT = 1000.0f;	// 黒幕背景の縦幅。

	/// <summary>
	/// 「RESULT」の表示設定情報。
	/// </summary>
	ResultInfo PRESS_A_START =
	{
		"Result:",	Vector3(-180.0f, 350.0f, 0.0f),		float(2.5f),	Vector4(g_vec4White)
	};

	/// <summary>
	/// 各同時消しのスコア表示設定情報。
	/// </summary>
	ResultInfo SCORE_INFO[] =
	{
		"Single:",	Vector3(-230.0f,  200.0f, 0.0f),	float(2.0f),	Vector4(g_vec4White),
		"Double:",	Vector3(-230.0f,  100.0f, 0.0f),	float(2.0f),	Vector4(g_vec4White),
		"Triple:",	Vector3(-230.0f,  0.0f,	  0.0f),	float(2.0f),	Vector4(g_vec4White),
		"Tetris:",	Vector3(-230.0f, -100.0f, 0.0f),	float(2.0f),	Vector4(g_vec4White)
	};

	/// <summary>
	/// 合計スコア表示設定情報。
	/// </summary>
	ResultInfo TOTAL_SCORE_INFO =
	{
		"Score:",	Vector3(-300.0f, -250.0f, 0.0f),	float(2.0f),	Vector4(g_vec4White)
	};

	/// <summary>
	/// 「Press A Title」の表示設定情報。
	/// </summary>
	ResultInfo PRESS_A_TITLE_TEXT =
	{
		"Press A Title",	Vector3(100.0f, -400.0f, 0.0f),	float(1.5f),	Vector4(g_vec4White)
	};
}

ResultView::~ResultView()
{
	NewGO<Title>(0, "Title");
}

bool ResultView::Start()
{
	m_game = FindGO<Game>("Game");
	m_scoreManager = FindGO<ScoreManager>("ScoreManager");

	m_backSpriteRender.Init("Assets/Sprite/Result/backBlack.dds", BACK_BLACK_WIDTH, BACK_BLACK_HEIGHT);

	ViewResultText();
	ViewLineClearScore();
	ViewTotalScore();
	CreatePressAtoTitleText();

	return true;
}

void ResultView::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		DeleteGO(this);
		DeleteGO(m_game);
	}
}

void ResultView::Render(RenderContext& rc)
{
	m_backSpriteRender.Draw(rc);

	m_resultFontRender.Draw(rc);

	for (int i = 0; i < static_cast<int>(LineClearType::LineClearType_Num); i++)
	{
		m_viewLineClearScore[i].Draw(rc);
	}

	m_viewTotalScore.Draw(rc);

	m_pressAtoTitleFontRender.Draw(rc);
}


/// <summary>
/// 「RESULT」の表示。
/// </summary>
void ResultView::ViewResultText()
{
	wchar_t wtext[128];
	MultiByteToWideChar(CP_UTF8, 0, PRESS_A_START.text.c_str(), -1, wtext, 128);
	SetTextOption(&m_resultFontRender, PRESS_A_START, L"%s", wtext);
}

/// <summary>
/// 各同時消しのスコア表示。
/// </summary>
void ResultView::ViewLineClearScore()
{
	//「Single」「Double」「Triple」「Tetris」のスコア表示。
	for (int i = 0; i < static_cast<int>(LineClearType::LineClearType_Num); i++)
	{
		wchar_t wtext[128];
		MultiByteToWideChar(CP_UTF8, 0, SCORE_INFO[i].text.c_str(), -1, wtext, 128);
		SetTextOption(&m_viewLineClearScore[i], SCORE_INFO[i], L"%s %d", wtext, m_scoreManager->GetLineClearCount(i));
	}
}

/// <summary>
/// 合計スコアの表示。
/// </summary>
void ResultView::ViewTotalScore()
{
	wchar_t wtext[128];
	MultiByteToWideChar(CP_UTF8, 0, TOTAL_SCORE_INFO.text.c_str(), -1, wtext, 128);
	SetTextOption(&m_viewTotalScore, TOTAL_SCORE_INFO, L"%s %d", wtext, m_scoreManager->GetScore());
}

/// <summary>
/// 「Press A to Title」の表示。
/// </summary>
void ResultView::CreatePressAtoTitleText()
{
	wchar_t wtext[128];
	MultiByteToWideChar(CP_UTF8, 0, PRESS_A_TITLE_TEXT.text.c_str(), -1, wtext, 128);
	SetTextOption(&m_pressAtoTitleFontRender, PRESS_A_TITLE_TEXT, L"%s", wtext);
}
