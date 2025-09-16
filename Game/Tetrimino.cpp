#include "stdafx.h"
#include "Tetrimino.h"
#include <time.h>
#include "FieldManager.h"

namespace
{
	//std::string TETRIMINO_FILE_PATH = "Assets/Tetrimino/Mino_";	// テトリミノのファイルパス。
	//std::string EXTENSION_DDS = ".dds";							// スプライトの拡張子。
	const Vector2 SPAWN_GRID_POSITION = { 4,18 };				// テトリミノのスポーン位置。

	/// <summary>
	/// 生成するテトリミノの情報。
	/// </summary>
	//struct MinoInfo
	//{
	//	char fileName;										// ファイルの名前。
	//Vector2 BlocksLocalPositionRatio[MINO_PARTS_COUNT];	// 中央のテトリミノからの相対座標比率。

	//	/// <summary>
	//	/// テトリミノのスプライトのファイルパスを取得する処理。
	//	/// </summary>
	//	std::string GetFullPath() const
	//	{
	//		return TETRIMINO_FILE_PATH + fileName + EXTENSION_DDS;
	//	}
	//};

	/// <summary>
	/// テトリミノのスプライトファイル名と相対座標の一覧。
	/// </summary>
	Vector2 BlocksLocalPositionRatio[MINO_KINDS_COUNT][MINO_PARTS_COUNT] =
	{
		//	1ブロック目,	2ブロック目,	3ブロック目,	4ブロック目 
			{{-1.5f,-0.5f},	{-0.5f,-0.5f},	{0.5f,-0.5f},	{1.5f,-0.5f}},
			{{0,0},			{-1,1},			{-1,0},			{1,0}		},
			{{0,0},			{-1,0},			{1,0},			{1,1}		},
			{{-0.5f,0.5f},	{-0.5f,-0.5f},	{0.5f,0.5f},	{0.5f,-0.5f}},
			{{0,0},			{-1,0},			{0,1},			{1,1}		},
			{{0,0},			{-1,0},			{1,1},			{1,0}		},
			{{0,0},			{-1,1},			{0,1},			{1,0}		}
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
	selectedMinoKind = rand() % enMinoKinds_Num;

	// テトリミノの画像を設定する。
	SetupSpriteImage();

	// テトリミノの回転基点位置を設定。
	SetupPivotPosition();

	// 中央ブロックからの相対座標を計算。
	CalcInitialLocalPositions();

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

	// 最下部に到達したか、他のテトリミノの上に乗ったかを判定。
	if (IsReachedStageBottom() || IsTetriminoBelow()) {
		// フィールドにブロックを設置。
		m_fieldManager->SaveTetrimino(blocksCurrentGlobalGridPositions, blockSpriteRender);
	}
}

void Tetrimino::Render(RenderContext& rc)
{
	// テトリミノの各ブロックを描画。
	for (auto sprite : blockSpriteRender) {
		sprite->Draw(rc);
	}
}



//=== 画像セットアップ／描画 ===//

/// <summary>
/// テトリミノの各ブロック画像（スプライト）を初期化します。
/// </summary>
void Tetrimino::SetupSpriteImage()
{
	for (auto it = blockSpriteRender.begin(); it != blockSpriteRender.end(); ++it) {
		*it = BlockCreateFactory::Create(selectedMinoKind);//m_blockSpriteList->GetBlockSpriteRenderAddress(selectedMinoKind);
	}

	//for (auto* sprite : blockSpriteRender) {
	//	sprite = m_blockSpriteList->GetBlockSpriteRenderAddress(selectedMinoKind);
	//}
}

/// <summary>
/// 各ブロックのワールド座標をスプライトに反映します。
/// </summary>
void Tetrimino::SetupSpritePosition()
{
	for (int i = 0; i < MINO_PARTS_COUNT; i++) {
		blockSpriteRender[i]->SetPosition({ blocksCurrentGlobalPositions[i].x, blocksCurrentGlobalPositions[i].y,0.0f });
		blockSpriteRender[i]->Update();
	}
}



//=== 座標計算パイプライン ===//

/// <summary> 
/// 回転の基準になるピボット（基点）ワールド座標を設定します。
/// </summary>
void Tetrimino::SetupPivotPosition()
{
	// テトリミノの回転基点位置にスポーン位置を代入。
	minoPivotGridPosition = SPAWN_GRID_POSITION;

	// I、Oのミノは基点を上に半ブロックずらす。
	if (selectedMinoKind == enMinoKinds_I || selectedMinoKind == enMinoKinds_O)
	{
		minoPivotGridPosition.x += 0.5;
		minoPivotGridPosition.y += 0.5;
	}
}

/// <summary>
/// 基点からのデフォルト（生成時）ローカル座標を計算します。
/// </summary>
void Tetrimino::CalcInitialLocalPositions()
{
	for (int i = 0; i < MINO_PARTS_COUNT; i++) {
		blocksInitialLocalPositions[i].x = BlocksLocalPositionRatio[selectedMinoKind][i].x * BLOCK_SIZE;
		blocksInitialLocalPositions[i].y = BlocksLocalPositionRatio[selectedMinoKind][i].y * BLOCK_SIZE;
	}
}

/// <summary>
/// 回転のステートを操作する処理。
/// </summary>
void Tetrimino::SwitchRotationState()
{
	// Aボタンで右回転。
	if (g_pad[0]->IsTrigger(enButtonA)) {
		int beforeState = rotationState;
		rotationState = (rotationState + 1) % enRotationDeg_Num;
		CalcLocalGridPositionsForRotationState();
		CalcBlocksCurrentGlobalGridPositions();

		TryWallKick();
		CheckNeedNearMinoKick(beforeState);
	}
	// Bボタンで左回転。
	else if (g_pad[0]->IsTrigger(enButtonB)) {
		int beforeState = rotationState;
		rotationState = (rotationState + enRotationDeg_Num - 1) % enRotationDeg_Num;
		CalcLocalGridPositionsForRotationState();
		CalcBlocksCurrentGlobalGridPositions();

		TryWallKick();
		CheckNeedNearMinoKick(beforeState);
	}
}

/// <summary>
/// 回転状態に応じてローカル座標を移動させます。
/// </summary>
/// <param name="pos">回転させる2次元ベクトル。</param>
/// <param name="rotationDeg">回転状態。</param>
/// <returns>回転後の2次元ベクトル。</returns>
Vector2 Tetrimino::Rotate(Vector2 pos, int rotationDeg)
{
	switch (rotationDeg)
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
		blocksCurrentLocalGridPositions[i] = Rotate(BlocksLocalPositionRatio[selectedMinoKind][i], rotationState);
	}
}

/// <summary>
/// 基点からのローカル座標をもとに、各ブロックのグローバルグリッド座標を計算します。
/// </summary>
void Tetrimino::CalcBlocksCurrentGlobalGridPositions()
{
	for (int i = 0; i < MINO_PARTS_COUNT; i++) {
		blocksCurrentGlobalGridPositions[i] =
		{ blocksCurrentLocalGridPositions[i].x + minoPivotGridPosition.x,
		  blocksCurrentLocalGridPositions[i].y + minoPivotGridPosition.y };
	}
}

/// <summary>
/// X、Yのグリッド座標をもとに、FieldManagerから座標を取得。。
/// </summary>
void Tetrimino::CalcBlocksCurrentGlobalPositions()
{
	for (int i = 0; i < MINO_PARTS_COUNT; i++) {
		blocksCurrentGlobalPositions[i] =
			m_fieldManager->GetCheckFieldPosition
			(blocksCurrentGlobalGridPositions[i].x, blocksCurrentGlobalGridPositions[i].y);
	}
}



//=== 入力／移動・落下 ===//

/// <summary> 
/// 1ブロック分、即座に下へ移動します。
/// </summary>
void Tetrimino::MoveDown()
{
	minoPivotGridPosition.y--;
}

/// <summary> 
/// 1ブロック分、即座に左へ移動します。
/// </summary>
void Tetrimino::MoveLeft()
{
	minoPivotGridPosition.x--;
}

/// <summary> 
/// 1ブロック分、即座に右へ移動します。
/// </summary>
void Tetrimino::MoveRight()
{
	minoPivotGridPosition.x++;
}

/// <summary>
/// ボタン入力でテトリミノを左右下に動かす処理。
/// </summary>
void Tetrimino::HandleInputMovement()
{
	if (g_pad[0]->IsTrigger(enButtonLeft)) {
		MoveLeft();
		CalcBlocksCurrentGlobalGridPositions();

		TryWallKick();
		CheckNeedNearMinoKick(0);
	}
	if (g_pad[0]->IsTrigger(enButtonRight)) {
		MoveRight();
		CalcBlocksCurrentGlobalGridPositions();

		TryWallKick();
		CheckNeedNearMinoKick(0);
	}
	if (g_pad[0]->IsTrigger(enButtonDown)) {
		MoveDown();
		CalcBlocksCurrentGlobalGridPositions();
	}
}

/// <summary>
/// 時間経過で落下する処理。
/// </summary>
void Tetrimino::AddGravity()
{
	// 秒数をカウント。
	spawnTimer += g_gameTime->GetFrameDeltaTime();

	// 1秒経ったら1ブロック分落下する。
	if (spawnTimer > 1.0f) {
		MoveDown();
		spawnTimer = 0.0f;
	}

	CalcBlocksCurrentGlobalGridPositions();
}

/// <summary>
/// テトリミノが壁にめり込んだ場合に、位置を補正する処理。
/// </summary>
void Tetrimino::TryWallKick()
{
	// 左右にはみ出している量を調べる。
	int overflowXLeft = 0;
	int overflowXRight = 0;

	for (auto& blockPos : blocksCurrentGlobalGridPositions)
	{
		// エリア外に出た時の処理。
		if (blockPos.x < 0)
		{
			int overflow = -blockPos.x;
			overflowXLeft = max(overflowXLeft, overflow);
		}
		else if (blockPos.x >= PLAYABLE_WIDTH_IN_BLOCKS)
		{
			int overflow = blockPos.x - (PLAYABLE_WIDTH_IN_BLOCKS - 1);
			overflowXRight = max(overflowXRight, overflow);
		}
	}

	// 一度だけ補正
	if (overflowXLeft > 0.0f)
	{
		minoPivotGridPosition.x += overflowXLeft;
	}
	else if (overflowXRight > 0.0f)
	{
		minoPivotGridPosition.x -= overflowXRight;
	}

	CalcBlocksCurrentGlobalGridPositions();
}

/// <summary>
/// 壁キックが必要かどうかを調べ、必要ならば壁キックを実行します。
/// </summary>
/// <param name="beforeState">回転前のステートを入れる。上下下移動の場合は0を入れる。</param>
void Tetrimino::CheckNeedNearMinoKick(int beforeState)
{
	// 左右のブロックの有無を調べる。
	bool isThereBlockLeft = false;
	bool isThereBlockRight = false;

	for (auto& blockPos : blocksCurrentGlobalGridPositions) {
		// 他のテトリミノと重なっている場合。
		if (m_fieldManager->GetCheckFieldFlag(blockPos.x, blockPos.y)) {
			// 基点より左側のブロックで、他のテトリミノと重なっている場合、左端のブロック位置を更新。
			if (blockPos.x < minoPivotGridPosition.x) {
				isThereBlockLeft = true;
			}
			// 基点より右側のブロックで、他のテトリミノと重なっている場合、右端のブロック位置を更新。
			else if (blockPos.x > minoPivotGridPosition.x) {
				isThereBlockRight = true;
			}
		}
	}

	// 左右両方にブロックがある場合は補正できないので何もしない。
	if (!isThereBlockLeft && !isThereBlockRight) { return; }
	// 左側にブロックがある場合は右に補正。
	else if (isThereBlockLeft && !isThereBlockRight) { MinoKickLeft(); }
	// 右側にブロックがある場合は左に補正。
	else if (!isThereBlockLeft && isThereBlockRight) { MinoKickRight(); }
	// 左右両方にブロックがある場合は補正できないので、回転前の状態に戻す。
	else if (isThereBlockLeft && isThereBlockRight) {
		rotationState = beforeState;
		CalcLocalGridPositionsForRotationState();
		CalcBlocksCurrentGlobalGridPositions();
	}
}

/// <summary>
/// 左側を壁キックする処理。
/// </summary>
void Tetrimino::MinoKickLeft()
{
	// 左側を蹴る場合、重なるブロックのX座標は基点より小さくなるため、基点のX座標を初期値とする。
	int leftMostThereIsBlockPosition = minoPivotGridPosition.x;
	// 左側を蹴る場合、重ならないブロックのX座標は基点より大きくなる可能性があるため、右端のグリッド座標を初期値とする。
	int leftMostNotThereIsBlockPosition = PLAYABLE_WIDTH_IN_BLOCKS;

	for (auto& blockPos : blocksCurrentGlobalGridPositions) {
		// 他のテトリミノと重なっている場合、重なっている左端のブロック位置を更新。
		if (m_fieldManager->GetCheckFieldFlag(blockPos.x, blockPos.y)) {
			leftMostThereIsBlockPosition = min(leftMostThereIsBlockPosition, blockPos.x);
		}
		// 他のテトリミノと重なっていない場合、重なっていない左端のブロック位置を更新。
		else {
			leftMostNotThereIsBlockPosition = min(leftMostNotThereIsBlockPosition, blockPos.x);
		}
	}

	// 重なっていない左端ブロックの位置と、重なっている左端ブロックの位置の差分を基点に加算する。
	minoPivotGridPosition.x += leftMostNotThereIsBlockPosition - leftMostThereIsBlockPosition;

	CalcBlocksCurrentGlobalGridPositions();
}

/// <summary>
/// 右側を壁キックする処理。
/// </summary>
void Tetrimino::MinoKickRight()
{
	// 右側を蹴る場合、重なるブロックのX座標は基点より大きくなるため、基点のX座標を初期値とする。
	int rightMostThereIsBlockPosition = minoPivotGridPosition.x;
	// 右側を蹴る場合、重ならないブロックのX座標は基点より小さくなる可能性があるため、左端のグリッド座標を初期値とする。
	int rightMostNotThereIsBlockPosition = 0;

	for (auto& blockPos : blocksCurrentGlobalGridPositions) {
		// 他のテトリミノと重なっている場合、重なっている右端のブロック位置を更新。
		if (m_fieldManager->GetCheckFieldFlag(blockPos.x, blockPos.y)) {
			rightMostThereIsBlockPosition = max(rightMostThereIsBlockPosition, blockPos.x);
		}
		// 他のテトリミノと重なっていない場合、重なっていない右端のブロック位置を更新。
		else {
			rightMostNotThereIsBlockPosition = max(rightMostNotThereIsBlockPosition, blockPos.x);
		}
	}
	// 重なっていない右端ブロックの位置と、重なっている右端ブロックの位置の差分を基点に加算する。
	minoPivotGridPosition.x += rightMostNotThereIsBlockPosition - rightMostThereIsBlockPosition;

	CalcBlocksCurrentGlobalGridPositions();
}

/// <summary>
/// テトリミノが最下部に到達したかどうかを判定する処理。
/// </summary>
/// <returns>到達していればtrue、そうでなければfalseを返す。</returns>
bool Tetrimino::IsReachedStageBottom()
{
	// 次に落下するポジションを保存。
	for (auto& blockPos : blocksCurrentGlobalGridPositions) {
		if (blockPos.y <= 0) {
			return true;
		}
	}
	return false;
}

/// <summary>
/// テトリミノの下に他のテトリミノがあるかどうかを判定する処理。
/// </summary>
/// <returns>下に他のテトリミノがあればtrue、なければfalseを返す。</returns>
bool Tetrimino::IsTetriminoBelow()
{
	for (auto& blockPos : blocksCurrentGlobalGridPositions) {
		int checkY = blockPos.y - 1;
		if (m_fieldManager->GetCheckFieldFlag(blockPos.x, checkY)) {
			return true;
		}
	}
	return false;
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
