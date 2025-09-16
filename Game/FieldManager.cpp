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
	// ステージを描画。
	stageSpriteRender.Draw(rc);

	// フィールドに配置されたブロックを描画。
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
				arrayXY.spriteRender->Draw(rc);
			}
		}
	}
}

/// <summary>
/// フィールド上にテトリミノのブロックを保存し、各ブロックのスプライトを配置・更新します。
/// </summary>
/// <param name="gridPos">テトリミノの各ブロックのフィールド上の座標（Vector2型の配列）。</param>
/// <param name="blockSpriteRender">各ブロックに対応するスプライトレンダラー（SpriteRender型の配列）。</param>
void FieldManager::SaveTetrimino(const std::array<Vector2, MINO_PARTS_COUNT>& gridPos, std::array<SpriteRender*, MINO_PARTS_COUNT> blockSpriteRender)
{
	for (int i = 0; i < MINO_PARTS_COUNT; ++i) {
		auto& checkField = checkFields[gridPos[i].x][gridPos[i].y];
		checkField.isThereBlock = true;
		// テトリミノと同じ位置に同じブロック画像を描画。
		checkField.spriteRender = blockSpriteRender[i];
		checkField.spriteRender->SetPosition({ checkField.position.x, checkField.position.y, 0.0f });
		checkField.spriteRender->Update();
	}

	DeleteGO(m_tetrimino);
	m_tetrimino = NewGO<Tetrimino>(0, "Tetrimino");
}