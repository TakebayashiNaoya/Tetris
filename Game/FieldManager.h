#pragma once
#include <array>
#include <vector>
#include "Tetrimino.h"
#include "BlockSpriteList.h"

class Tetrimino;

namespace
{
	const int PLAYABLE_WIDTH_IN_BLOCKS = 10;										// テトリミノを配置できる範囲（横のブロック数）
	const int PLAYABLE_HEIGHT_IN_BLOCKS = 20;										// テトリミノを配置できる範囲（縦のブロック数）

	const int STAGE_TOTAL_WIDTH_IN_BLOCKS = 12;										// 枠を含めた横のブロック数。
	const int STAGE_TOTAL_HEIGHT_IN_BLOCKS = 21;									// 枠を含めた縦のブロック数。

	const float STAGE_RIGHT_OFFSET_X_FROM_CENTER = 4.5f;							// 中央からステージ左端までのブロック数。
	const float STAGE_RIGHT_X = STAGE_RIGHT_OFFSET_X_FROM_CENTER * BLOCK_SIZE;		// ステージ右端のX座標。
	const float STAGE_LEFT_OFFSET_X_FROM_CENTER = -4.5f;							// 中央からステージ左端までのブロック数。
	const float STAGE_LEFT_X = STAGE_LEFT_OFFSET_X_FROM_CENTER * BLOCK_SIZE;		// ステージ左端のX座標。
	const float STAGE_BOTTOM_OFFSET_Y_FROM_CENTER = -9.0f;							// 中央からステージ下端までのブロック数。
	const float STAGE_BOTTOM_Y = STAGE_BOTTOM_OFFSET_Y_FROM_CENTER * BLOCK_SIZE;	// ステージ下端のY座標。
	const Vector2 STAGE_ORIGIN_POSITION = { STAGE_LEFT_X,STAGE_BOTTOM_Y };			// ステージの一番左下の座標。
}

class FieldManager : public IGameObject
{
public:
	/// <summary>
	/// ステージ1ブロック分の情報。
	/// </summary>
	struct OneBlockOfFieldInfo
	{
		SpriteRender* spriteRender;
		Vector2 position = Vector2::Zero;
		bool isThereBlock = false;

		// セッター関数
		void SetIsThereBlock(bool value) {
			isThereBlock = value;
		};
	};

private:
	bool Start()override final;
	void Render(RenderContext& rc);

	/// <summary>
	/// フィールドを管理する空オブジェクトを配置する処理。
	/// </summary>
	void SetupCheckFields();

	void DrawBlocks(RenderContext& rc);

	std::array<std::array<OneBlockOfFieldInfo, PLAYABLE_HEIGHT_IN_BLOCKS>, PLAYABLE_WIDTH_IN_BLOCKS> checkFields;
	Vector2 stagePivotPosition = Vector2::Zero;

	SpriteRender stageSpriteRender;


	Tetrimino* m_tetrimino;

public:
	FieldManager();

	/// <summary>
	/// プレイ可能なフィールドの幅と高さに対応するチェックフィールド情報を取得する処理。
	/// </summary>
	auto& GetCheckFields()
	{
		return checkFields;
	};

	Vector2 GetCheckFieldPosition(int grid_x, int grid_y)
	{
		return checkFields[grid_x][grid_y].position;
	}

	bool GetCheckFieldFlag(int grid_x, int grid_y)
	{
		return checkFields[grid_x][grid_y].isThereBlock;
	}

	/// <summary>
	/// チェックフィールドの特定の位置にブロックが存在するかどうかを設定する処理。
	/// </summary>
	/// <param name="x">x方向のブロック数</param>
	/// <param name="y">y方向のブロック数</param>
	/// <param name="isThereBlock">ブロックがある場合はtrue、無い場合はfalse</param>
	void SetCheckField(int grid_x, int grid_y, bool isThereBlock)
	{
		checkFields[grid_x][grid_y].isThereBlock = isThereBlock;
	}

	void SaveTetrimino(const std::array<Vector2, MINO_PARTS_COUNT>& gridPos, std::array<SpriteRender*, MINO_PARTS_COUNT> blockSpriteRender);
};

