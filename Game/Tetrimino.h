#pragma once

class FieldManager;
class BlockSpriteList;
class NextTetriminoView;
class ScoreManager;
class PauseView;

namespace
{
	constexpr int MINO_PARTS_COUNT = 4;	// テトリミノを構成するブロック数。
	constexpr int MINO_KINDS_COUNT = 7;	// テトリミノの種類数。
}

class Tetrimino :public IGameObject
{
private:
	//=== ライフサイクル ===//
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;



	//=== 画像セットアップ／描画 ===//

	/// <summary>
	/// テトリミノの各ブロック画像（スプライト）を初期化します。
	/// </summary>
	void SetupSpriteImage();

	/// <summary> 
	/// 各ブロックのワールド座標をスプライトに反映します。
	/// </summary>
	void SetupSpritePosition();



	//=== 座標計算パイプライン ===//

	/// <summary>
	/// 回転の基準になるピボット（基点）ワールド座標を設定します。
	/// </summary>
	void SetupPivotPosition();

	/// <summary> 
	/// 基点からのデフォルト（生成時）ローカル座標を計算します。
	/// </summary>
	/// <summary> 回転状態の切り替え（Aで右回転、Bで左回転）。</summary>
	void SwitchRotationState();

	/// <summary> 
	/// 回転状態に応じてローカル座標を移動させます。
	/// </summary>
	/// <param name="pos"> 回転させる2次元ベクトル。</param>
	/// <param name="rotationDeg"> 回転状態。</param>
	/// <returns> 回転後の2次元ベクトル。</returns>
	Vector2 Rotate(Vector2 pos, int rotationDeg);

	/// <summary> 
	/// Rotate関数を使い、回転状態に応じたローカル座標を計算します。
	/// </summary>
	void CalcLocalGridPositionsForRotationState();

	/// <summary>
	/// 基点とローカル座標をもとに、各ブロックのグローバルグリッド座標を計算します。
	/// </summary>
	void CalcBlocksCurrentGlobalGridPositions();

	/// <summary> 
	/// X、Yのグリッド座標をもとに、FieldManagerから座標を取得。
	/// </summary>
	void CalcBlocksCurrentGlobalPositions();



	//=== 入力／移動・落下 ===//

	/// <summary> 
	/// 1ブロック分、即座に下へ移動します。
	/// </summary>
	void MoveDown();

	/// <summary> 
	/// 1ブロック分、即座に左へ移動します。
	/// </summary>
	void MoveLeft();

	/// <summary> 
	/// 1ブロック分、即座に右へ移動します。
	/// </summary>
	void MoveRight();

	/// <summary> 
	/// 左右・下入力によるテトリミノの平行移動。
	/// </summary>
	void HandleInputMovement();

	/// <summary>
	/// テトリミノの方向入力を処理し、必要に応じて移動アクションを実行します。
	/// </summary>
	/// <param name="button">処理する方向入力ボタン。</param>
	/// <param name="isBlocked">移動がブロックされているかどうかを示すフラグ。</param>
	/// <param name="moveFunc">テトリミノを移動させるための関数オブジェクト。</param>
	void HandleDirectionalInput(EnButton button, bool isBlocked, std::function<void()> moveFunc);

	/// <summary> 
	/// 時間経過による落下（重力）を適用します。
	/// </summary>
	void AddGravity();



	//=== 衝突判定 ===//

	/// <summary>
	/// 左側にステージの端、または他のテトリミノがあるかどうかを判定する処理。
	/// </summary>
	/// <returns> 左側にステージの端、または他のテトリミノがあればtrue、なければfalseを返す。</returns>
	bool IsBlockedLeft();

	/// <summary>
	/// 右側にステージの端、または他のテトリミノがあるかどうかを判定する処理。
	/// </summary>
	/// <returns> 右側にステージの端、または他のテトリミノがあればtrue、なければfalseを返す。</returns>
	bool IsBlockedRight();

	/// <summary>
	/// 下側にステージの端、または他のテトリミノがあるかどうかを判定する処理。
	/// </summary>
	/// <returns> 下側にステージの端、または他のテトリミノがあればtrue、なければfalseを返す。</returns>
	bool IsBlockedBelow();



	//=== SRS(スーパーローテーションシステム) ===//

	/// <summary>
	/// 回転前と回転後の状態に基づいて、SRSのキックテーブルに従って位置調整を行います。
	/// </summary>
	/// <param name="beforeState">回転前のテトリミノの状態（角度）。</param>
	/// <param name="currentState">回転後のテトリミノの状態（角度）。</param>
	void SuperRotationSystem(int beforeState, int currentState);

	/// <summary>
	/// 引数の値だけテトリミノの位置を補正し、補正後に他のテトリミノやステージ外に重なっていないかをチェックします。
	/// ※GetCheckFieldFlag(0～9,0～19)は範囲外を指定するとエラーが起きるため、エリア外のチェックを先に行っている。
	/// </summary>
	/// <param name="offsetX"> X座標の補正値。</param>
	/// <param name="offsetY"> Y座標の補正値。</param>
	/// <returns> 補正後に配置できる場合はtrue、できない場合はfalseを返す。</returns>
	bool SRS_Check(Vector2 offset);



	//=== 終了時の処理 ===//

	/// <summary>
	/// テトリミノが最下部に到達するか、他のテトリミノの上に乗ったら、フィールドマネージャーに保存。
	/// </summary> 
	bool SaveToFieldManager();



	//=== その他 ===//
	void CalcFallIntervalByLevel();

	//template <typename TetriminoCheckFunc>
	//void ForEachCheckFields(const TetriminoCheckFunc& func);
	//void SetupBlockPresenceFlags();



	FieldManager* m_fieldManager;
	BlockSpriteList* m_blockSpriteList;
	NextTetriminoView* m_nextTetriminoView;
	ScoreManager* m_scoreManager;
	PauseView* m_pauseView;

	std::array<SpriteRender*, MINO_PARTS_COUNT> m_blockSpriteRender;			// 各ブロックの画像

	std::array<Vector2, MINO_PARTS_COUNT> m_blocksCurrentLocalGridPositions;	// 現在ローカル(グリッド)
	std::array<Vector2, MINO_PARTS_COUNT> m_blocksCurrentGlobalGridPositions;	// 現在ワールド(グリッド)
	std::array<Vector2, MINO_PARTS_COUNT> m_blocksCurrentGlobalPositions;		// 現在ワールド

	Vector2 m_minoPivotGridPosition = Vector2::Zero;							// テトリミノの回転基点グリッド座標を格納する変数。
	int m_rotationState = 0;													// テトリミノの回転状態を格納する変数。
	int m_selectedMinoKind = 0;													// 生成するテトリミノの種類を格納する変数。
	float m_fallTimer = 0.0f;													// タイマー。
	float m_pressTimer = 0.0f;													// 自動落下までの長押しタイマー。
	float m_moveIntervalTimer = 0.0f;											// 移動のインターバルタイマー。
	float m_deleteTimer = 0.0f;													// テトリミノ削除タイマー。
	float m_fallInterval = 0.0f;													// 落下速度。
};