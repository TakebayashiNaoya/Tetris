#include "stdafx.h"
#include "HoldView.h"
#include "Types.h"
#include "FieldManager.h"
#include "TextTemplate.h"

namespace
{
	constexpr float HOLD_VIEW_SCALE = 150.0f;	// ホールド表示の大きさ。
	constexpr float HOLD_VIEW_POS_X = -350.0f;	// ホールド表示のX座標。
	constexpr float HOLD_VIEW_POS_Y = 350.0f;	// ホールド表示のY座標。
	const std::string HOLD_VIEW_FILE_PATH = "Assets/Sprite/Tetrimino/FullSizeMino_";	// テトリミノのファイルパス。
	const std::string EXTENSION_DDS = ".dds";											// スプライトの拡張子。
	constexpr char fileName[static_cast<int>(EnMinoKinds::enMinoKinds_Num)] =
	{ 'I','J','L','O','S','T','Z' };													// ファイルの名前。
	/// <summary>
	/// 指定されたファイル名からフルパス文字列を生成します。
	/// </summary>
	/// <param name="fileName"> ファイル名を表す文字列。</param>
	/// <returns> ファイル名にパスと拡張子を付加したフルパスの文字列。</returns>
	std::string GetFullPath(int num)
	{
		return HOLD_VIEW_FILE_PATH + fileName[num] + EXTENSION_DDS;
	}

	/// <summary>
	/// 「HOLD」と表示するための情報。
	/// </summary>
	ResultInfo HOLD =
	{
		"HOLD",	Vector3(-420.0f, 480.0f, 0.0f),		float(1.5f),	Vector4(g_vec4White)
	};
}

bool HoldView::Start()
{
	m_fieldManager = FindGO<FieldManager>("FieldManager");

	wchar_t wtext[128];
	MultiByteToWideChar(CP_UTF8, 0, HOLD.text.c_str(), -1, wtext, 128);
	SetTextOption(&m_holdTextFontRender, HOLD, L"%s", wtext);

	return true;
}

void HoldView::Render(RenderContext& renderContext)
{
	m_holdTextFontRender.Draw(renderContext);
	if (m_fieldManager->GetIsHolded()) {
		m_holdViewSprite.Draw(renderContext);
	}
}

/// <summary>
/// ホールドされたミノの種類に応じて画像を更新します。
/// </summary>
void HoldView::UpdateHoldView()
{
	int holdedMinoKind = m_fieldManager->GetHoldedMinoKind();
	m_holdViewSprite.Init(GetFullPath(holdedMinoKind).c_str(), HOLD_VIEW_SCALE, HOLD_VIEW_SCALE);
	m_holdViewSprite.SetPosition({ HOLD_VIEW_POS_X, HOLD_VIEW_POS_Y, 0.0f });
	m_holdViewSprite.Update();
}
