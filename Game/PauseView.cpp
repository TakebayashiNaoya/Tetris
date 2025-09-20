#include "stdafx.h"
#include "PauseView.h"
#include "TextTemplate.h"

namespace
{
	constexpr float FULL_HD_WIDTH = 1920.0f;	// フルHDの横幅。
	constexpr float FULL_HD_HEIGHT = 1080.0f;	// フルHDの縦幅。

	ResultInfo LINE_CLEAR_TOTAL_COUNT_INFO =
	{
		"PAUSE",	Vector3(-200.0f, 100.0f, 0.0f),	float(3.0f),	Vector4(g_vec4White)
	};
}

bool PauseView::Start()
{
	// ポーズ画面の背景スプライトを設定。
	m_backSpriteRender.Init("Assets/Sprite/Result/backBlack.dds", FULL_HD_WIDTH, FULL_HD_HEIGHT);

	// "PAUSE"のテキストを設定。
	wchar_t wtext[128];
	MultiByteToWideChar(CP_UTF8, 0, LINE_CLEAR_TOTAL_COUNT_INFO.text.c_str(), -1, wtext, 128);
	SetTextOption(&m_pauseFontRender, LINE_CLEAR_TOTAL_COUNT_INFO, L"%s", wtext);

	return true;
}

void PauseView::Update()
{
	if (g_pad[0]->IsTrigger(enButtonStart)) {
		// スタートボタンが押されたらポーズのON/OFFを切り替え。
		m_isPause = !m_isPause;
	}
}

void PauseView::Render(RenderContext& rc)
{
	if (m_isPause) {
		// ポーズ中ならポーズ画面を表示。
		m_backSpriteRender.Draw(rc);
		m_pauseFontRender.Draw(rc);
	}
}
