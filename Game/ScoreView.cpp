#include "stdafx.h"
#include "ScoreView.h"
#include "ScoreManager.h"

namespace
{
	const Vector3 SCORE_POSITION = Vector3(300.0f, 400.0f, 0.0f);	// スコア表示の開始位置。
	constexpr float SCORE_SCALE = 2.0f;								// スコア表示の大きさ。
}

bool ScoreView::Start()
{
	m_scoreManager = FindGO<ScoreManager>("ScoreManager");
	return true;
}

void ScoreView::Update()
{
	wchar_t wcsbuf[256];
	swprintf(wcsbuf, 256, L"SCORE：%d", m_scoreManager->GetScore());
	m_viewScore.SetText(wcsbuf);
	m_viewScore.SetPosition(SCORE_POSITION);
	m_viewScore.SetScale(SCORE_SCALE);
	m_viewScore.SetColor(g_vec4White);
}

void ScoreView::Render(RenderContext& rc)
{
	m_viewScore.Draw(rc);
}