#pragma once
#include <array>
#include <vector>
#include "Tetrimino.h"
#include "BlockSpriteList.h"
#include "ScoreManager.h"

class Game;
class Tetrimino;
class ScoreManager;

namespace
{
	constexpr int PLAYABLE_WIDTH_IN_BLOCKS = 10;										// テトリミノを配置できる範囲（横のブロック数）
	constexpr int PLAYABLE_HEIGHT_IN_BLOCKS = 20;										// テトリミノを配置できる範囲（縦のブロック数）

	constexpr int STAGE_TOTAL_WIDTH_IN_BLOCKS = 12;										// 枠を含めた横のブロック数。
	constexpr int STAGE_TOTAL_HEIGHT_IN_BLOCKS = 21;									// 枠を含めた縦のブロック数。

	constexpr float STAGE_LEFT_OFFSET_X_FROM_CENTER = -4.5f;							// 中央からステージ左端までのブロック数。
	const float STAGE_LEFT_X = STAGE_LEFT_OFFSET_X_FROM_CENTER * BLOCK_SIZE;		// ステージ左端のX座標。
	constexpr float STAGE_BOTTOM_OFFSET_Y_FROM_CENTER = -9.0f;							// 中央からステージ下端までのブロック数。
	const float STAGE_BOTTOM_Y = STAGE_BOTTOM_OFFSET_Y_FROM_CENTER * BLOCK_SIZE;	// ステージ下端のY座標。
	const Vector2 STAGE_ORIGIN_POSITION = Vector2(STAGE_LEFT_X, STAGE_BOTTOM_Y);			// ステージの一番左下の座標。
}

class FieldManager : public IGameObject
{
public:
	/// <summary>
	/// 空ブロックの情報。
	/// </summary>
	struct OneBlockOfFieldInfo
	{
		SpriteRender* spriteRender = nullptr;	// スプライトレンダーのアドレス。
		Vector2 position = Vector2::Zero;		// ブロックの位置。
		bool isThereBlock = false;				// ブロックの有無。
	};

	FieldManager();

	/// <summary>
	/// 指定したグリッド座標のチェックフィールドの位置を取得します。
	/// </summary>
	/// <param name="grid_x">グリッドのX座標。</param>
	/// <param name="grid_y">グリッドのY座標。</param>
	/// <returns>指定したグリッド座標にあるチェックフィールドの位置（Vector2型）。</returns>
	Vector2 GetCheckFieldPosition(int grid_x, int grid_y)
	{
		return checkFields[grid_x][grid_y].position;
	}

	/// <summary>
	/// 指定されたグリッド座標にブロックが存在するかどうかを判定します。
	/// </summary>
	/// <param name="grid_x">グリッドのX座標。</param>
	/// <param name="grid_y">グリッドのY座標。</param>
	/// <returns>指定した座標にブロックが存在する場合は true、存在しない場合は false を返します。</returns>
	bool GetCheckFieldFlag(int grid_x, int grid_y)
	{
		return checkFields[grid_x][grid_y].isThereBlock;
	}

	/// <summary>
	/// テトリミノの各ブロックの位置とスプライトレンダーを保存します。
	/// </summary>
	/// <param name="gridPos">テトリミノの各ブロックのグリッド上の位置を表す配列。</param>
	/// <param name="blockSpriteRender">各ブロックのスプライトレンダーへのポインタの配列。</param>
	void SaveTetrimino(const std::array<Vector2, MINO_PARTS_COUNT>& gridPos,
		std::array<SpriteRender*, MINO_PARTS_COUNT> blockSpriteRender);

private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

	/// <summary>
	/// フィールドを管理する空オブジェクトを配置する処理。
	/// </summary>
	void SetupCheckFields();

	/// <summary>
	/// テトリミノがあるフィールドのブロックを描画します。
	/// </summary>
	/// <param name="rc">描画コンテキスト。ブロックの描画に使用されます。</param>
	void DrawBlocks(RenderContext& rc);

	/// <summary>
	/// フィールド内の全てのラインを埋まっているかチェックし、埋まっているラインを消去します。
	/// </summary>
	void CheckFullLines();

	/// <summary>
	/// 指定されたラインを消します。
	/// </summary>
	/// <param name="lineY">消すラインのY座標。</param>
	void ClearFullLine(int lineY);

	/// <summary>
	/// フィールド上に空ブロックを配置し、各ブロックのスプライトを配置・更新します。
	/// </summary>
	std::array<std::array<OneBlockOfFieldInfo, PLAYABLE_HEIGHT_IN_BLOCKS>, PLAYABLE_WIDTH_IN_BLOCKS> checkFields;
	Vector2 stagePivotPosition = Vector2::Zero;

	SpriteRender stageSpriteRender;	// ステージのスプライトレンダー。

	Game* m_game;
	Tetrimino* m_tetrimino;
	ScoreManager* m_scoreManager;
};

