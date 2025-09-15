#include "stdafx.h"
#include "FieldManager.h"
#include "Tetrimino.h"

FieldManager::FieldManager()
{
	// ステージの画像を設定。
	stageSpriteRender.Init("Assets/Stage/Stage.dds",
		BLOCK_SIZE * STAGE_TOTAL_WIDTH_IN_BLOCKS, BLOCK_SIZE * STAGE_TOTAL_HEIGHT_IN_BLOCKS);

	// ステージ管理用の空オブジェクトを配置。
	SetupCheckFields();
}

bool FieldManager::Start()
{
	m_tetrimino = NewGO<Tetrimino>(0, "Tetrimino");
	return true;
}

void FieldManager::Render(RenderContext& rc)
{

	stageSpriteRender.Draw(rc);
	DrawBlocks(rc);

}

/// <summary>
/// フィールドを管理する空オブジェクトを配置する処理。
/// </summary>
void FieldManager::SetupCheckFields()
{
	for (int x = 0; x < PLAYABLE_WIDTH_IN_BLOCKS; x++)
	{
		for (int y = 0; y < PLAYABLE_HEIGHT_IN_BLOCKS; y++)
		{
			checkFields[x][y].position =
			{ x * BLOCK_SIZE + STAGE_ORIGIN_POSITION.x, y * BLOCK_SIZE + STAGE_ORIGIN_POSITION.y };
		}
	}
}

/// <summary>
/// テトリミノがあるフィールドのブロックを描画します。
/// </summary>
/// <param name="rc">描画コンテキスト。ブロックの描画に使用されます。</param>
void FieldManager::DrawBlocks(RenderContext& rc)
{
	for (auto& arrayX : checkFields)
	{
		for (auto& arrayXY : arrayX)
		{
			if (arrayXY.isThereBlock)
			{
				arrayXY.spriteRender.Draw(rc);
			}
		}
	}
}

/// <summary>
/// テトリミノのブロック位置情報を保存し、対応するスプライトを初期化して描画位置を設定します。
/// 既存のテトリミノオブジェクトを削除し、新しいテトリミノを生成します。
/// </summary>
/// <param name="gridPos">テトリミノの各ブロックのグリッド座標を格納した配列。</param>
/// <param name="path">ブロックのスプライト画像ファイルへのパス。</param>
void FieldManager::SaveTetrimino(const std::array<Vector2, MINO_PARTS_COUNT>& gridPos, const char* path)
{
	// テトリミノが停止したときの各ブロックのグリッド位置を取得。
	for (const auto& blockPos : gridPos) {
		auto& tmp = checkFields[blockPos.x][blockPos.y];
		// チェックフィールドの対応する位置にブロックが存在することを設定。
		tmp.isThereBlock = true;
		// テトリミノと同じ位置に同じブロック画像を描画。
		tmp.spriteRender.Init(path, BLOCK_SIZE, BLOCK_SIZE);
		tmp.spriteRender.SetPosition({ tmp.position.x, tmp.position.y, 0.0f });
		tmp.spriteRender.Update();
	}
	DeleteGO(m_tetrimino);
	m_tetrimino = NewGO<Tetrimino>(0, "Tetrimino");
}