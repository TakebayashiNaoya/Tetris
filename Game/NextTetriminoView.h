#pragma once
#include "Types.h"
#include <queue>
#include <vector>
#include <random>
#include <algorithm>

namespace
{
	constexpr int NEXT_MINO_DISPLAY_COUNT = 6;	// 次に表示するテトリミノの数。
}

class NextTetriminoView :public IGameObject
{
public:
	/// <summary>
	/// 次に生成されるテトリミノの種類を取得し、配列を更新する。
	/// </summary>
	int GetNextMinoKind();

private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

	/// <summary>
	/// 表示用配列を初期化する。
	/// </summary>
	void SetupmDisplayMinos();

	/// <summary>
	/// 控え配列が空になったら7バッグ方式で補充する。
	/// </summary>
	void RefillReserveQueue();

	/// <summary>
	/// 表示用の配列に必要に応じて控え配列から補充する。
	/// </summary>
	void RefillDisplayQueueIfNeeded();

	std::array<EnMinoKinds, NEXT_MINO_DISPLAY_COUNT> m_displayMinos;						// 次に生成されるテトリミノを表示する配列。
	std::array<EnMinoKinds, static_cast<int>(EnMinoKinds::enMinoKinds_Num)> m_reserveMinos;	// 次に生成されるテトリミノを控える配列。

	/// <summary>
	/// 7バッグ方式でランダムにテトリミノを生成するためのベース配列。
	/// </summary>
	std::vector<EnMinoKinds> m_base =
	{
		EnMinoKinds::enMinoKinds_I,
		EnMinoKinds::enMinoKinds_J,
		EnMinoKinds::enMinoKinds_L,
		EnMinoKinds::enMinoKinds_O,
		EnMinoKinds::enMinoKinds_S,
		EnMinoKinds::enMinoKinds_T,
		EnMinoKinds::enMinoKinds_Z
	};

	SpriteRender m_nextMinoSpriteRender[NEXT_MINO_DISPLAY_COUNT];	// 次に生成されるテトリミノを表示するスプライトレンダー。
};

