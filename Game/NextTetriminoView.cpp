#include "stdafx.h"
#include "NextTetriminoView.h"
#include <time.h>

namespace
{
	constexpr float NEXT_MINO_VIEW_SCALE = 150.0f;		// 次に表示するテトリミノの大きさ。
	constexpr float NEXT_MINO_VIEW_POS_X = 400.0f;		// 次に表示するテトリミノのX座標。
	constexpr float NEXT_MINO_VIEW_POS_Y = 350.0f;		// 次に表示するテトリミノのY座標。
	constexpr float NEXT_MINO_VIEW_INTERVAL = 100.0f;	// 次に表示するテトリミノの間隔。
	constexpr int INIT_RESERVE_QUEUE_NUM = 12;			// 控え配列の初期化回数。

	const std::string TITLE_LOGO_FILE_PATH = "Assets/Sprite/Tetrimino/FullSizeMino_";	// テトリミノのファイルパス。
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
		return TITLE_LOGO_FILE_PATH + fileName[num] + EXTENSION_DDS;
	}
}

int NextTetriminoView::GetNextMinoKind()
{
	int nextMino = static_cast<int>(m_displayMinos.front());


	RefillDisplayQueueIfNeeded();

	for (int i = 0; i < NEXT_MINO_DISPLAY_COUNT; i++) {
		// スプライトの初期化。
		m_nextMinoSpriteRender[i].Init(GetFullPath(static_cast<int>(m_displayMinos[i])).c_str(), NEXT_MINO_VIEW_SCALE, NEXT_MINO_VIEW_SCALE);
		m_nextMinoSpriteRender[i].Update();
	}

	return nextMino;
}

bool NextTetriminoView::Start()
{
	// テトリミノを完全ランダムにする処理。
	srand(time(nullptr));

	// 表示用配列と控え配列を初期化。
	SetupmDisplayMinos();

	return true;
}

void NextTetriminoView::Update()
{
}

void NextTetriminoView::Render(RenderContext& rc)
{
	for (auto& sprite : m_nextMinoSpriteRender) {
		sprite.Draw(rc);
	}
}

/// <summary>
/// 表示用配列を初期化する。
/// </summary>
void NextTetriminoView::SetupmDisplayMinos()
{
	// スプライトレンダーの位置を設定。
	for (int i = 0; i < NEXT_MINO_DISPLAY_COUNT; i++) {
		m_nextMinoSpriteRender[i].SetPosition(Vector3(NEXT_MINO_VIEW_POS_X, NEXT_MINO_VIEW_POS_Y - i * NEXT_MINO_VIEW_INTERVAL, 0.0f));
		m_nextMinoSpriteRender[i].Update();
	}

	// 配列の中身が全て0になっているため、12回処理し、中身を吐き出す。
	for (int i = 0; i < INIT_RESERVE_QUEUE_NUM; i++) {
		RefillDisplayQueueIfNeeded();
	}

	// スプライトの初期化。
	for (int i = 0; i < NEXT_MINO_DISPLAY_COUNT; i++) {
		// スプライトの初期化。
		m_nextMinoSpriteRender[i].Init(GetFullPath(i).c_str(), NEXT_MINO_VIEW_SCALE, NEXT_MINO_VIEW_SCALE);
		m_nextMinoSpriteRender[i].Update();
	}
}

/// <summary>
/// 控えが空になったら7バッグ方式で補充する。
/// </summary>
void NextTetriminoView::RefillReserveQueue()
{
	// 7バッグ方式でランダムにテトリミノを生成する。
	random_shuffle(m_base.begin(), m_base.end());

	// 控え配列に補充。
	for (int i = 0; i < static_cast<int>(EnMinoKinds::enMinoKinds_Num); i++) {
		m_reserveMinos[i] = m_base[i];
	}
}

/// <summary>
/// 表示用の配列に必要に応じて控え配列から補充する。
/// </summary>
void NextTetriminoView::RefillDisplayQueueIfNeeded()
{
	// 控えが空なら補充。
	if (m_reserveMinos.front() == EnMinoKinds::enMinoKinds_Num) {
		RefillReserveQueue(); // 7バッグ補充
	}

	// 表示用配列を1つずつ前にずらす。
	for (int i = 0; i < NEXT_MINO_DISPLAY_COUNT - 1; i++) {
		m_displayMinos[i] = m_displayMinos[i + 1];
		m_displayMinos[i + 1] = EnMinoKinds::enMinoKinds_Num;
	}

	// 控えから1つだけ補充。
	m_displayMinos[m_displayMinos.size() - 1] = m_reserveMinos.front();

	// 控え用配列を1つずつ前にずらす。
	for (int i = 0; i < static_cast<int>(EnMinoKinds::enMinoKinds_Num) - 1; i++) {
		m_reserveMinos[i] = m_reserveMinos[i + 1];
		m_reserveMinos[i + 1] = EnMinoKinds::enMinoKinds_Num;
	}
}