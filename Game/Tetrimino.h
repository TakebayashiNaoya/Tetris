#pragma once

class FieldManager;

const int MINO_PARTS_COUNT = 4;	// テトリミノを構成するブロック数。
const int MINO_KINDS_COUNT = 7;	// テトリミノの種類数。

class Tetrimino :public IGameObject
{
private:
	/// <summary> テトリミノの回転状態。</summary>
	enum EnRotationDeg
	{
		enRotationDeg_0,
		enRotationDeg_90,
		enRotationDeg_180,
		enRotationDeg_270,
		enRotationDeg_Num
	};

	//=== ライフサイクル ===//
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;


	//=== 画像セットアップ／描画 ===//
	/// <summary> テトリミノの各ブロック画像（スプライト）を初期化します。</summary>
	void SetupSpriteImage();
	/// <summary> 各ブロックのワールド座標をスプライトに反映します。</summary>
	void SetupSpritePosition();


	//=== 座標計算パイプライン ===//
	/// <summary> 回転の基準になるピボット（基点）ワールド座標を設定します。</summary>
	void SetupPivotPosition();
	/// <summary> 基点からのデフォルト（生成時）ローカル座標を計算します。</summary>
	void CalcInitialLocalPositions();
	/// <summary> 回転状態の切り替え（Aで右回転、Bで左回転）。</summary>
	void SwitchRotationState();
	/// <summary> 回転状態に応じてローカル座標を移動させます。</summary>
	/// <param name="pos"> 回転させる2次元ベクトル。</param>
	/// <param name="rotationDeg"> 回転状態。</param>
	/// <returns> 回転後の2次元ベクトル。</returns>
	Vector2 Rotate(Vector2 pos, int rotationDeg);
	/// <summary> Rotate関数を使い、回転状態に応じたローカル座標を計算します。</summary>
	void CalcLocalGridPositionsForRotationState();

	void CalcBlocksCurrentGlobalGridPositions();

	/// <summary> ローカル座標＋基点から、各ブロックのワールド座標を計算します。</summary>
	void CalcBlocksCurrentGlobalPositions();


	//=== 入力／移動・落下 ===//
	/// <summary> 1ブロック分、即座に下へ移動します。</summary>
	void MoveDown();
	/// <summary> 1ブロック分、即座に左へ移動します。</summary>
	void MoveLeft();
	/// <summary> 1ブロック分、即座に右へ移動します。</summary>
	void MoveRight();
	/// <summary> 左右・下入力によるテトリミノの平行移動。</summary>
	void HandleInputMovement();
	/// <summary> 時間経過による落下（重力）を適用します。</summary>
	void AddGravity();


	//=== 補正 ===//
	/// <summary> 壁キックを試みます。</summary>
	void TryWallKick();
	/// <summary> 指定された状態に基づいて、ミノキックが必要かどうかを判定します。</summary>
	/// <param name="beforeState"> 判定の基準となる直前の状態を表す整数値。</param>
	void CheckNeedNearMinoKick(int beforeState);
	/// <summary> 左方向へのミノキックを実行します。</summary>
	void MinoKickLeft();
	/// <summary> 右方向へのミノキックを実行します。</summary>
	void MinoKickRight();


	//=== 着地判定 ===//
	/// <summary> ステージの最下部に到達したかどうかを判定します。</summary>
	/// <returns> 最下部に到達した場合は true、そうでない場合は false を返します。</returns>
	bool IsReachedStageBottom();
	/// <summary> 下にテトリミノがあるかどうかを判定します。</summary>
	/// <returns> 下にテトリミノがある場合は true、そうでない場合は false を返します。</returns>
	bool IsTetriminoBelow();


	//=== その他 ===//
	//template <typename TetriminoCheckFunc>
	//void ForEachCheckFields(const TetriminoCheckFunc& func);
	//void SetupBlockPresenceFlags();


	FieldManager* m_fieldManager;

	std::array<SpriteRender, MINO_PARTS_COUNT> blockSpriteRender;			// 各ブロックの画像
	std::array<Vector2, MINO_PARTS_COUNT> blocksInitialLocalPositions;		// 初期ローカル
	std::array<Vector2, MINO_PARTS_COUNT> blocksCurrentLocalGridPositions;	// 現在ローカル(グリッド)
	std::array<Vector2, MINO_PARTS_COUNT> blocksCurrentGlobalPositions;		// 現在ワールド
	std::array<Vector2, MINO_PARTS_COUNT> blocksCurrentGlobalGridPositions;	// 現在ワールド(グリッド)

	std::array<Vector2, MINO_PARTS_COUNT> minoPartsNextFallPosition;			// 次に落下するワールド座標

	Vector2 minoPivotGridPosition = Vector2::Zero;						// テトリミノの回転基点グリッド座標を格納する変数。
	int rotationState = 0;												// テトリミノの回転状態を格納する変数。
	int selectedMinoKind = 0;											// 生成するテトリミノの種類を格納する変数。
	float spawnTimer = 0.0f;											// タイマー。
};