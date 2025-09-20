#include "stdafx.h"
#include "ScoreView.h"
#include "ScoreManager.h"
#include "TextTemplate.h"

namespace
{
	const Vector3 POSITION_FOR_PRESS_A_TITLE = { -430.0f, -200.0f, 0.0f };	// "Press Any Button"の位置。
	constexpr float SCALE_FOR_PRESS_A_TITLE = 2.0f;							// "Press Any Button"の大きさ。

	constexpr float BACK_BLACK_WIDTH = 1000.0f;		// 黒幕背景の横幅。
	constexpr float BACK_BLACK_HEIGHT = 1080.0f;	// 黒幕背景の縦幅。

	/// <summary>
	/// 合計スコア表示設定情報。
	/// </summary>
	ResultInfo CURRENT_SCORE_INFO =
	{
		"SCORE:",	Vector3(-200.0f, 500.0f, 0.0f),	float(1.5f),	Vector4(g_vec4White)
	};

	ResultInfo CURRENT_LEVEL_INFO =
	{
		"LEVEL:",	Vector3(-450.0f, 200.0f, 0.0f),	float(1.5f),	Vector4(g_vec4White)
	};

	ResultInfo LINE_CLEAR_TOTAL_COUNT_INFO =
	{
		"LINE:",	Vector3(-450.0f, 0.0f, 0.0f),	float(1.5f),	Vector4(g_vec4White)
	};
}

bool ScoreView::Start()
{
	m_scoreManager = FindGO<ScoreManager>("ScoreManager");
	m_backSpriteRender.Init("Assets/Sprite/Result/backBlack.dds", BACK_BLACK_WIDTH, BACK_BLACK_HEIGHT);
	return true;
}

void ScoreView::Update()
{
	ViewCurrentTotalScore();
	ViewCurrentLevel();
	ViewLineClearTotalCount();
}

void ScoreView::Render(RenderContext& rc)
{
	m_backSpriteRender.Draw(rc);
	m_viewScore.Draw(rc);
	m_viewLevel.Draw(rc);
	m_viewLine.Draw(rc);
}

void ScoreView::ViewCurrentTotalScore()
{
	wchar_t wtext[128];
	MultiByteToWideChar(CP_UTF8, 0, CURRENT_SCORE_INFO.text.c_str(), -1, wtext, 128);
	SetTextOption(&m_viewScore, CURRENT_SCORE_INFO, L"%s %d", wtext, m_scoreManager->GetScore());
}

void ScoreView::ViewCurrentLevel()
{
	wchar_t wtext[128];
	MultiByteToWideChar(CP_UTF8, 0, CURRENT_LEVEL_INFO.text.c_str(), -1, wtext, 128);
	SetTextOption(&m_viewLevel, CURRENT_LEVEL_INFO, L"%s\n%d", wtext, m_scoreManager->GetLevel());
}

void ScoreView::ViewLineClearTotalCount()
{
	wchar_t wtext[128];
	MultiByteToWideChar(CP_UTF8, 0, LINE_CLEAR_TOTAL_COUNT_INFO.text.c_str(), -1, wtext, 128);
	SetTextOption(&m_viewLine, LINE_CLEAR_TOTAL_COUNT_INFO, L"%s\n%d", wtext, m_scoreManager->GetLineClearTotalCount());
}
