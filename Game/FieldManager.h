#pragma once
#include <array>
#include <vector>
#include "Tetrimino.h"
#include "BlockSpriteList.h"
#include "ScoreManager.h"

class Game;
class Tetrimino;
class ScoreManager;
class HoldView;

namespace
{
	constexpr int PLAYABLE_WIDTH_IN_BLOCKS = 10;	// テトリミノを配置できる範囲（横のブロック数）
	constexpr int PLAYABLE_HEIGHT_IN_BLOCKS = 20;	// テトリミノを配置できる範囲（縦のブロック数）
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

	/// <summary>
	/// コンストラクタ。
	/// </summary>
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
	/// テトリミノが保持されているかどうかを判定します。
	/// </summary>
	/// <returns>テトリミノが保持されていれば true、そうでなければ false を返します。</returns>
	bool GetIsHolded() const
	{
		return m_isHolded;
	}

	/// <summary>
	/// 保持されているミノの種類を取得します。
	/// </summary>
	/// <returns>保持されているミノの種類を表す整数値。</returns>
	int GetHoldedMinoKind() const
	{
		return m_holdedMinoKind;
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
	/// ラインを消しし、消されたライン数を計算します。
	/// </summary>
	/// <returns>消されたライン数。</returns>
	int LinesClearAndCalcClearedLineCount();

	/// <summary>
	/// 1ラインでも消されていれば、コンボ数を更新します。
	/// </summary>
	/// <param name="clearLineCount">消されたライン数。</param>
	void UpdateComboCount(int numClearedLines);

	/// <summary>
	/// 指定されたラインを消します。
	/// </summary>
	/// <param name="lineY">消すラインのY座標。</param>
	void ClearFullLine(int lineY);

	/// <summary>
	/// 指定されたY座標を基準に上から下にラインをシフトします。
	/// </summary>
	/// <param name="posYForFullLine">消されるラインのY座標。シフトの開始位置を指定します。</param>
	void ShiftLines(int posYForFullLine);

	/// <summary>
	/// ライン消去時にサウンドを再生します。
	/// </summary>
	/// <param name="clearLineCount">消去されたラインの数。</param>
	void PlaySoundOnLineClear(int numClearedLines);

	/// <summary>
	/// 指定されたミノの種類をホールドします。
	/// </summary>
	/// <param name="minoKind">ホールドするミノの種類を表す整数値。</param>
	void HoldMino();

	/// <summary>
	/// 保持中のミノを入れ替えます。
	/// </summary>
	void SwapHoldedMino();

	/// <summary>
	/// Tスピンかどうかを判定します。
	/// </summary>
	/// <returns>Tスピンの場合は true、そうでない場合は false を返します。</returns>
	bool CheckIsTSpin();



	// フィールド上に空ブロックを配置し、各ブロックのスプライトを配置・更新します。
	std::array<std::array<OneBlockOfFieldInfo, PLAYABLE_HEIGHT_IN_BLOCKS>, PLAYABLE_WIDTH_IN_BLOCKS> checkFields;

	SpriteRender stageSpriteRender;	// ステージのスプライトレンダー。

	Game* m_game;
	Tetrimino* m_tetrimino;
	ScoreManager* m_scoreManager;
	HoldView* m_holdView;

	int m_holdedMinoKind = 0;		// ホールドされているミノの種類。
	bool m_isHolded = false;		// ミノがホールドされている場合はtrue。
	int m_comboCount = 0;			// コンボ数。
	bool m_isAllowedToHold = true;	// ホールドが許可されているか場合はtrue。
};

