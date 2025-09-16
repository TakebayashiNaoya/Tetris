#include "stdafx.h"
#include "Tetrimino.h"
#include <time.h>
#include "FieldManager.h"

namespace
{
	const Vector2 SPAWN_GRID_POSITION = { 4,18 };	// テトリミノのスポーン位置。
	const float DELETE_TIME = 0.9f;					// テトリミノが最下部に到達してからフィールドに固定されるまでの時間。

	/// <summary>
	/// テトリミノのスプライトファイル名と相対座標の一覧。
	/// </summary>
	Vector2 BlocksLocalPositionRatio[MINO_KINDS_COUNT][MINO_PARTS_COUNT] =
	{
		//	1ブロック目,	2ブロック目,	3ブロック目,	4ブロック目 
			{{-1.5f,0.5f},	{-0.5f,0.5f},	{0.5f,0.5f},	{1.5f,0.5f}},	// I
			{{0,0},			{-1,1},			{-1,0},			{1,0}		},	// J
			{{0,0},			{-1,0},			{1,0},			{1,1}		},	// L
			{{-0.5f,0.5f},	{-0.5f,-0.5f},	{0.5f,0.5f},	{0.5f,-0.5f}},	// O
			{{0,0},			{-1,0},			{0,1},			{1,1}		},	// S
			{{0,0},			{-1,0},			{0,1},			{1,0}		},	// T
			{{0,0},			{-1,1},			{0,1},			{1,0}		}	// Z
	};

}

bool Tetrimino::Start()
{
	// フィールドマネージャーを取得。
	m_fieldManager = FindGO<FieldManager>("FieldManager");

	m_blockSpriteList = FindGO<BlockSpriteList>("BlockSpriteList");

	// テトリミノを完全ランダムにする処理。
	srand(time(nullptr));

	// 生成するテトリミノの種類を抽選。
	m_selectedMinoKind = rand() % enMinoKinds_Num;

	// テトリミノの画像を設定する。
	SetupSpriteImage();

	// テトリミノの回転基点位置を設定。
	SetupPivotPosition();

	// 回転のステートに応じてブロックの移動先位置を計算。
	CalcLocalGridPositionsForRotationState();

	// 各ブロックのグローバルグリッド座標を算出。
	CalcBlocksCurrentGlobalGridPositions();

	// 各ブロックのワールド座標を算出。
	CalcBlocksCurrentGlobalPositions();

	return true;
}

void Tetrimino::Update()
{
	// 回転のステートを操作。
	SwitchRotationState();

	// テトリミノの左右下移動操作。
	HandleInputMovement();

	// 落下処理。
	AddGravity();

	// ブロックのグローバル座標を算出。
	CalcBlocksCurrentGlobalPositions();

	// 移動先位置をレンダーのポジションに設定。
	SetupSpritePosition();

	// 最後にフィールドに固定する。
	SaveToFieldManager();
}

void Tetrimino::Render(RenderContext& rc)
{
	// テトリミノの各ブロックを描画。
	for (auto sprite : m_blockSpriteRender) {
		sprite->Draw(rc);
	}
}



//=== 画像セットアップ／描画 ===//

/// <summary>
/// テトリミノの各ブロック画像（スプライト）を初期化します。
/// </summary>
void Tetrimino::SetupSpriteImage()
{
	for (auto it = m_blockSpriteRender.begin(); it != m_blockSpriteRender.end(); ++it) {
		*it = BlockCreateFactory::Create(m_selectedMinoKind);
	}
}

/// <summary>
/// 各ブロックのワールド座標をスプライトに反映します。
/// </summary>
void Tetrimino::SetupSpritePosition()
{
	for (int i = 0; i < MINO_PARTS_COUNT; i++) {
		m_blockSpriteRender[i]->SetPosition({ m_blocksCurrentGlobalPositions[i].x, m_blocksCurrentGlobalPositions[i].y,0.0f });
		m_blockSpriteRender[i]->Update();
	}
}



//=== 座標計算パイプライン ===//

/// <summary> 
/// 回転の基準になるピボット（基点）ワールド座標を設定します。
/// </summary>
void Tetrimino::SetupPivotPosition()
{
	// テトリミノの回転基点位置にスポーン位置を代入。
	m_minoPivotGridPosition = SPAWN_GRID_POSITION;

	// I、Oのミノは基点を上に半ブロックずらす。
	if (m_selectedMinoKind == enMinoKinds_I || m_selectedMinoKind == enMinoKinds_O)
	{
		m_minoPivotGridPosition.x += 0.5;
		m_minoPivotGridPosition.y += 0.5;
	}
}

/// <summary>
/// 回転のステートを操作する処理。
/// </summary>
void Tetrimino::SwitchRotationState()
{
	// Aボタンで右回転。
	if (g_pad[0]->IsTrigger(enButtonA)) {
		// 回転状態を保存。
		int beforeState = m_rotationState;
		// 回転状態を更新。
		m_rotationState = (m_rotationState + 1) % enRotationDeg_Num;
		CalcLocalGridPositionsForRotationState();
		CalcBlocksCurrentGlobalGridPositions();

		// 回転前の状態と回転後の状態をもとに、SRSを実行。
		SuperRotationSystem(beforeState, m_rotationState);
		CalcLocalGridPositionsForRotationState();
		CalcBlocksCurrentGlobalGridPositions();
	}
	// Bボタンで左回転。
	else if (g_pad[0]->IsTrigger(enButtonB)) {
		// 回転状態を保存。
		int beforeState = m_rotationState;
		// 回転状態を更新。
		m_rotationState = (m_rotationState + enRotationDeg_Num - 1) % enRotationDeg_Num;
		CalcLocalGridPositionsForRotationState();
		CalcBlocksCurrentGlobalGridPositions();

		// 回転前の状態と回転後の状態をもとに、SRSを実行。
		SuperRotationSystem(beforeState, m_rotationState);
		CalcLocalGridPositionsForRotationState();
		CalcBlocksCurrentGlobalGridPositions();
	}
}

/// <summary>
/// 回転状態に応じてローカル座標を移動させます。
/// </summary>
/// <param name="pos">回転させる2次元ベクトル。</param>
/// <param name="rotationDeg">回転状態。</param>
/// <returns>回転後の2次元ベクトル。</returns>
Vector2 Tetrimino::Rotate(Vector2 pos, int rotationState)
{
	switch (rotationState)
	{
	case enRotationDeg_0:   return pos;
	case enRotationDeg_90:  return { pos.y, -pos.x };
	case enRotationDeg_180: return { -pos.x, -pos.y };
	case enRotationDeg_270: return { -pos.y, pos.x };
	}
	return pos;
}

/// <summary>
/// テトリミノの各ブロックの現在の回転状態に応じたローカル座標を計算します。
/// </summary>
void Tetrimino::CalcLocalGridPositionsForRotationState()
{
	for (int i = 0; i < MINO_PARTS_COUNT; ++i) {
		m_blocksCurrentLocalGridPositions[i] = Rotate(BlocksLocalPositionRatio[m_selectedMinoKind][i], m_rotationState);
	}
}

/// <summary>
/// 基点とローカル座標をもとに、各ブロックのグローバルグリッド座標を計算します。
/// </summary>
void Tetrimino::CalcBlocksCurrentGlobalGridPositions()
{
	for (int i = 0; i < MINO_PARTS_COUNT; i++) {
		m_blocksCurrentGlobalGridPositions[i] =
		{ m_blocksCurrentLocalGridPositions[i].x + m_minoPivotGridPosition.x,
		  m_blocksCurrentLocalGridPositions[i].y + m_minoPivotGridPosition.y };
	}
}

/// <summary>
/// X、Yのグリッド座標をもとに、FieldManagerから座標を取得。
/// </summary>
void Tetrimino::CalcBlocksCurrentGlobalPositions()
{
	for (int i = 0; i < MINO_PARTS_COUNT; i++) {
		m_blocksCurrentGlobalPositions[i] =
			m_fieldManager->GetCheckFieldPosition
			(m_blocksCurrentGlobalGridPositions[i].x, m_blocksCurrentGlobalGridPositions[i].y);
	}
}



//=== 入力／移動・落下 ===//

/// <summary> 
/// 1ブロック分、即座に下へ移動します。
/// </summary>
void Tetrimino::MoveDown()
{
	m_minoPivotGridPosition.y--;
}

/// <summary> 
/// 1ブロック分、即座に左へ移動します。
/// </summary>
void Tetrimino::MoveLeft()
{
	m_minoPivotGridPosition.x--;
	m_deleteTimer = 0.0f;
}

/// <summary> 
/// 1ブロック分、即座に右へ移動します。
/// </summary>
void Tetrimino::MoveRight()
{
	m_minoPivotGridPosition.x++;
	m_deleteTimer = 0.0f;
}

/// <summary>
/// ボタン入力でテトリミノを左右下に動かす処理。
/// </summary>
void Tetrimino::HandleInputMovement()
{
	if (g_pad[0]->IsTrigger(enButtonLeft)) {
		// 左に移動できない場合は何もしない。
		if (IsBlockedLeft()) { return; }
		MoveLeft();
		CalcBlocksCurrentGlobalGridPositions();
	}
	if (g_pad[0]->IsTrigger(enButtonRight)) {
		// 右に移動できない場合は何もしない。
		if (IsBlockedRight()) { return; }
		MoveRight();
		CalcBlocksCurrentGlobalGridPositions();
	}
	if (g_pad[0]->IsTrigger(enButtonDown)) {
		// 下に移動できない場合は何もしない。
		if (IsBlockedBelow()) { return; }
		MoveDown();
		CalcBlocksCurrentGlobalGridPositions();
	}
}

/// <summary>
/// 時間経過で落下する処理。
/// </summary>
void Tetrimino::AddGravity()
{
	// 下に移動できない場合は何もしない。
	if (IsBlockedBelow()) { return; }

	// 秒数をカウント。
	m_fallTimer += g_gameTime->GetFrameDeltaTime();

	// 1秒経ったら1ブロック分落下する。
	if (m_fallTimer > 1.0f) {
		MoveDown();
		m_fallTimer = 0.0f;
	}

	CalcBlocksCurrentGlobalGridPositions();
}



//=== 衝突判定 ===//

/// <summary>
/// 左側にステージの端、または他のテトリミノがあるかどうかを判定する処理。
/// ※GetCheckFieldFlag(0～9,0～19)は範囲外を指定するとエラーが起きるため、エリア外のチェックを先に行っている。
/// </summary>
/// <returns> 左側にステージの端、または他のテトリミノがあればtrue、なければfalseを返す。</returns>
bool Tetrimino::IsBlockedLeft()
{
	for (auto& blockPos : m_blocksCurrentGlobalGridPositions)
	{
		// いずれかのブロックがステージの左端にある場合。
		if (blockPos.x == 0) {
			return true;
		}
		// いずれかのブロックの左側に他のテトリミノがある場合。
		else if (m_fieldManager->GetCheckFieldFlag(blockPos.x - 1, blockPos.y)) {
			return true;
		}
	}
	return false;
}

/// <summary>
/// 右側にステージの端、または他のテトリミノがあるかどうかを判定する処理。
/// ※GetCheckFieldFlag(0～9,0～19)は範囲外を指定するとエラーが起きるため、エリア外のチェックを先に行っている。
/// </summary>
/// <returns> 右側にステージの端、または他のテトリミノがあればtrue、なければfalseを返す。</returns>
bool Tetrimino::IsBlockedRight()
{
	for (auto& blockPos : m_blocksCurrentGlobalGridPositions)
	{
		// いずれかのブロックがステージの右端にある場合。
		if (blockPos.x == (PLAYABLE_WIDTH_IN_BLOCKS - 1)) {
			return true;
		}
		// いずれかのブロックの右側に他のテトリミノがある場合。
		else if (m_fieldManager->GetCheckFieldFlag(blockPos.x + 1, blockPos.y)) {
			return true;
		}
	}
	return false;
}

/// <summary>
/// 下側にステージの端、または他のテトリミノがあるかどうかを判定する処理。
/// ※GetCheckFieldFlag(0～9,0～19)は範囲外を指定するとエラーが起きるため、エリア外のチェックを先に行っている。
/// </summary>
/// <returns> 下側にステージの端、または他のテトリミノがあればtrue、なければfalseを返す。</returns>
bool Tetrimino::IsBlockedBelow()
{
	for (auto& blockPos : m_blocksCurrentGlobalGridPositions)
	{
		// いずれかのブロックがステージの下端にある場合。
		if (blockPos.y == 0) {
			return true;
		}
		// いずれかのブロックの下側に他のテトリミノがある場合。
		else if (m_fieldManager->GetCheckFieldFlag(blockPos.x, blockPos.y - 1)) {
			return true;
		}
	}
	return false;
}



//=== SRS(スーパーローテーションシステム) ===//

/// <summary>
/// 回転前と回転後の状態に基づいて、SRSのキックテーブルに従って位置調整を行います。
/// </summary>
/// <param name="beforeState">回転前のテトリミノの状態（角度）。</param>
/// <param name="currentState">回転後のテトリミノの状態（角度）。</param>
void Tetrimino::SuperRotationSystem(int beforeState, int currentState)
{
	if (m_selectedMinoKind == enMinoKinds_I) {
		SuperRotationSystemVersionI(beforeState, currentState);
	}
	else {
		SuperRotationSystemVersionNormal(beforeState, currentState);
	}
}

/// <summary>
/// Iのテトリミノの回転時に、SRSのキックテーブルに従って位置調整を行います。
/// 回転後に位置調整ができない場合は、回転前の状態に戻します。
/// </summary>
/// <param name="beforeState">回転前のテトリミノの状態（角度）。</param>
/// <param name="currentState">回転後のテトリミノの状態（角度）。</param>
void Tetrimino::SuperRotationSystemVersionI(int beforeState, int currentState)
{
	if (beforeState == enRotationDeg_0 && currentState == enRotationDeg_90) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(-2, 0)) { return; }
		else if (SuperRotationSystemCheck(1, 0)) { return; }
		else if (SuperRotationSystemCheck(-2, -1)) { return; }
		else if (SuperRotationSystemCheck(1, 2)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_90 && currentState == enRotationDeg_0) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(2, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 0)) { return; }
		else if (SuperRotationSystemCheck(2, 1)) { return; }
		else if (SuperRotationSystemCheck(-1, -2)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_90 && currentState == enRotationDeg_180) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 0)) { return; }
		else if (SuperRotationSystemCheck(2, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 2)) { return; }
		else if (SuperRotationSystemCheck(2, -1)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_180 && currentState == enRotationDeg_90) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(1, 0)) { return; }
		else if (SuperRotationSystemCheck(-2, 0)) { return; }
		else if (SuperRotationSystemCheck(1, -2)) { return; }
		else if (SuperRotationSystemCheck(-2, 1)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_180 && currentState == enRotationDeg_270) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(2, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 0)) { return; }
		else if (SuperRotationSystemCheck(2, 1)) { return; }
		else if (SuperRotationSystemCheck(-1, -2)) { return; }
	}
	else if (beforeState == enRotationDeg_270 && currentState == enRotationDeg_180) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(-2, 0)) { return; }
		else if (SuperRotationSystemCheck(1, 0)) { return; }
		else if (SuperRotationSystemCheck(-2, -1)) { return; }
		else if (SuperRotationSystemCheck(1, 2)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_270 && currentState == enRotationDeg_0) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(1, 0)) { return; }
		else if (SuperRotationSystemCheck(-2, 0)) { return; }
		else if (SuperRotationSystemCheck(1, -2)) { return; }
		else if (SuperRotationSystemCheck(-2, 1)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_0 && currentState == enRotationDeg_270) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 0)) { return; }
		else if (SuperRotationSystemCheck(2, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 2)) { return; }
		else if (SuperRotationSystemCheck(2, -1)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
}

/// <summary>
/// I以外のテトリミノの回転時に、SRSのキックテーブルに従って位置調整を行います。
/// 回転後に位置調整ができない場合は、回転前の状態に戻します。
/// </summary>
/// <param name="beforeState">回転前のテトリミノの状態（角度）。</param>
/// <param name="currentState">回転後のテトリミノの状態（角度）。</param>
void Tetrimino::SuperRotationSystemVersionNormal(int beforeState, int currentState)
{
	if (beforeState == enRotationDeg_0 && currentState == enRotationDeg_90) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 1)) { return; }
		else if (SuperRotationSystemCheck(0, -2)) { return; }
		else if (SuperRotationSystemCheck(-1, -2)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_90 && currentState == enRotationDeg_180) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(1, 0)) { return; }
		else if (SuperRotationSystemCheck(1, -1)) { return; }
		else if (SuperRotationSystemCheck(0, 2)) { return; }
		else if (SuperRotationSystemCheck(1, 2)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_180 && currentState == enRotationDeg_270) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(1, 0)) { return; }
		else if (SuperRotationSystemCheck(1, 1)) { return; }
		else if (SuperRotationSystemCheck(0, -2)) { return; }
		else if (SuperRotationSystemCheck(1, -2)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_270 && currentState == enRotationDeg_0) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, -1)) { return; }
		else if (SuperRotationSystemCheck(0, 2)) { return; }
		else if (SuperRotationSystemCheck(-1, +2)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_0 && currentState == enRotationDeg_270) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(1, 0)) { return; }
		else if (SuperRotationSystemCheck(1, 1)) { return; }
		else if (SuperRotationSystemCheck(0, -2)) { return; }
		else if (SuperRotationSystemCheck(1, -2)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_270 && currentState == enRotationDeg_180) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, -1)) { return; }
		else if (SuperRotationSystemCheck(0, 2)) { return; }
		else if (SuperRotationSystemCheck(-1, 2)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_180 && currentState == enRotationDeg_90) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 1)) { return; }
		else if (SuperRotationSystemCheck(0, -2)) { return; }
		else if (SuperRotationSystemCheck(-1, -2)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_90 && currentState == enRotationDeg_0) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(1, 0)) { return; }
		else if (SuperRotationSystemCheck(1, -1)) { return; }
		else if (SuperRotationSystemCheck(0, 2)) { return; }
		else if (SuperRotationSystemCheck(1, 2)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
}

/// <summary>
/// 引数の値だけテトリミノの位置を補正し、補正後に他のテトリミノやステージ外に重なっていないかをチェックします。
/// ※GetCheckFieldFlag(0～9,0～19)は範囲外を指定するとエラーが起きるため、エリア外のチェックを先に行っている。
/// </summary>
/// <param name="offsetX"> X座標の補正値。</param>
/// <param name="offsetY"> Y座標の補正値。</param>
/// <returns> 補正後に配置できる場合はtrue、できない場合はfalseを返す。</returns>
bool Tetrimino::SuperRotationSystemCheck(int offsetX, int offsetY)
{
	int OverlapCount = 0;
	for (auto& blockPos : m_blocksCurrentGlobalGridPositions) {
		// X座標がエリア外に出ている場合。
		int x = blockPos.x + offsetX;
		if (x < 0 || x >= PLAYABLE_WIDTH_IN_BLOCKS) {
			OverlapCount++;
			continue;
		}
		// Y座標がエリア外に出ている場合。
		int y = blockPos.y + offsetY;
		if (y < 0 || y >= PLAYABLE_HEIGHT_IN_BLOCKS) {
			OverlapCount++;
			continue;
		}
		// 他のテトリミノと重なっている場合。
		if (m_fieldManager->GetCheckFieldFlag(x, y)) {
			OverlapCount++;
		}
	}
	// 重なっていなければ補正を適用。
	if (OverlapCount == 0) {
		m_minoPivotGridPosition.x += offsetX;
		m_minoPivotGridPosition.y += offsetY;
		CalcBlocksCurrentGlobalGridPositions();
		return true;
	}
	// 重なっていれば補正を適用しない。
	else {
		return false;
	}
}



//=== 終了時の処理 ===//

/// <summary>
/// テトリミノが最下部に到達するか、他のテトリミノの上に乗ったら、フィールドマネージャーに保存。
/// </summary> 
void Tetrimino::SaveToFieldManager()
{
	// 最下部に到達したか、他のテトリミノの上に乗ったかを判定。
	if (IsBlockedBelow()) {
		m_deleteTimer += g_gameTime->GetFrameDeltaTime();
		if (g_pad[0]->IsTrigger(enButtonDown)) {
			m_fieldManager->SaveTetrimino(m_blocksCurrentGlobalGridPositions, m_blockSpriteRender);
		}
		else if (m_deleteTimer > DELETE_TIME) {
			m_fieldManager->SaveTetrimino(m_blocksCurrentGlobalGridPositions, m_blockSpriteRender);
		}
	}
	// 下が着かなくなったらタイマーリセット。
	else {
		m_deleteTimer = 0.0f;
	}
}



///// <summary>
///// テトリミノの全てのチェックフィールドに対して指定された関数を適用します。
///// </summary>
///// <typeparam name="TetriminoCheckFunc">チェックフィールドに対して呼び出される関数オブジェクトの型。</typeparam>
///// <param name="func">各チェックフィールドに適用する関数オブジェクト。</param>
//template <typename TetriminoCheckFunc>
//void Tetrimino::ForEachCheckFields(const TetriminoCheckFunc& func)
//{
//	for (auto& checkFieldArray : m_fieldManager->GetCheckFields()) {
//		for (auto& checkField : checkFieldArray) {
//			func(checkField);
//		}
//	}
//}
//
///// <summary>
///// テトリミノの現在のブロック位置に基づいて、各フィールドにブロックが存在するかどうかのフラグを設定します。
///// </summary>
//void Tetrimino::SetupBlockPresenceFlags()
//{
//	ForEachCheckFields([&](FieldManager::OneBlockOfFieldInfo& checkField)
//		{
//			for (auto& blockPos : BlocksCurrentGlobalPositions) {
//				if (blockPos.IsEquals(checkField.position))
//				{
//					checkField.SetIsThereBlock(true);
//				}
//			}
//		});
//}

/// <summary>
/// 空オブジェクトとテトリミノの各ブロックの座標を照合する処理。
/// </summary>
/// <param name="pos">テトリミノを構成するブロックの配列を入れる。</param>
/// <returns>座標が一致したらtrueを返し、そうでなければfalseを返す。</returns>
//bool Tetrimino::IsEmptyAtPosition()
//{
//	// ステージ上の空オブジェクトの座標を順に処理。
//	for (auto& emptyBlock : m_fieldManager->GetCheckFields().position)
//	{
//		// テトリミノの各ブロックの座標を順に処理。
//		for (int i = 0; i < enParts_Num; i++)
//		{
//			// 空オブジェクトとテトリミノの各ブロックの座標を照合し、一致したらtrueを返す。
//			if (emptyBlock->position.x == pos[i].x && emptyBlock->position.y == pos[i].y)
//			{
//				return true;
//			}
//		}
//	}
//
//	return false;
//}
