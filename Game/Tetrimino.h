#pragma once

/// <summary>
/// テトリミノの種類の列挙型。
/// </summary>
enum EnMinoKinds
{
	enMinoKinds_I,
	enMinoKinds_J,
	enMinoKinds_L,
	enMinoKinds_O,
	enMinoKinds_S,
	enMinoKinds_T,
	enMinoKinds_Z,
	enMinoKinds_Num
};

/// <summary>
/// テトリミノを構成するブロックを識別するための列挙型。
/// </summary>
enum EnParts
{
	enParts_1,
	enParts_2,
	enParts_3,
	enParts_4,
	enParts_Num
};

class Tetrimino :public IGameObject
{
private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

	/// <summary>
	/// テトリミノの画像を設定する処理。
	/// </summary>
	void SetupSprite();

	/// <summary>
	/// テトリミノを構成している各ブロックの相対座標を算出する処理。
	/// </summary>
	void CalcLocalPosition();

	/// <summary>
	/// テトリミノの回転基点座標を算出する処理。
	/// </summary>
	void SetupPivotPosition();

	/// <summary>
	/// テトリミノの回転状態を切り替える処理。
	/// </summary>
	void SetupRotationState();

	/// <summary>
	/// テトリミノの回転状態に応じて各ブロックの座標を算出する処理。
	/// </summary>
	void CalcPositionForRotationState();

	/// <summary>
	/// テトリミノを左右下に動かす処理。
	/// </summary>
	void Move();

	/// <summary>
	/// テトリミノを落下させる処理。
	/// </summary>
	void AddGravity();

	/// <summary>
	/// スプライトの位置を設定する処理。
	/// </summary>
	void SetupSpritePosition();

	/// <summary>
	/// 下方向に移動できるかどうかを判定します。
	/// </summary>
	/// <returns>下方向に移動可能な場合は true、そうでない場合は false を返します。</returns>
	bool CanMoveDown();

	/// <summary>
	/// テトリミノの回転状態を指す列挙型。
	/// ※テトリミノは右回転。
	/// </summary>
	enum EnRotation
	{
		enRotation_0,
		enRotation_90,
		enRotation_180,
		enRotation_270
	};

	SpriteRender blockSpriteRender[enParts_Num];	// テトリミノを構成する各ブロックの画像を格納する配列。
	Vector2 minoPartsLocalPosition[enParts_Num];	// テトリミノを構成している各ブロックの相対座標を格納する配列。
	Vector2 minoPartsCarrentPosition[enParts_Num];	// テトリミノを構成している各ブロックのワールド座標を格納する配列。
	Vector2 minoPivotPosition = Vector2::Zero;		// テトリミノの回転基点ワールド座標を格納する変数。
	int rotationState = 0;							// テトリミノの回転状態を格納する変数。
	int createMinoKind = 0;							// 生成するテトリミノの種類を格納する変数。
	float spawnTimer = 0.0f;						// タイマー。
};