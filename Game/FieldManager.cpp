#include "stdafx.h"
#include "Game.h"
#include "FieldManager.h"
#include "Tetrimino.h"
#include "SoundManager.h"
#include "Types.h"
#include "HoldView.h"

namespace
{
	constexpr int STAGE_TOTAL_WIDTH_IN_BLOCKS = 12;										// 枠を含めた横のブロック数。
	constexpr int STAGE_TOTAL_HEIGHT_IN_BLOCKS = 21;									// 枠を含めた縦のブロック数。

	constexpr int TSPIN_FILLED_CORNERS_THRESHOLD = 3;								// Tスピンと判定するために必要な埋まった隅の数の閾値。
	constexpr float STAGE_LEFT_OFFSET_X_FROM_CENTER = -4.5f;						// 中央からステージ左端までのブロック数。
	const float STAGE_LEFT_X = STAGE_LEFT_OFFSET_X_FROM_CENTER * BLOCK_SIZE;		// ステージ左端のX座標。
	constexpr float STAGE_BOTTOM_OFFSET_Y_FROM_CENTER = -9.0f;						// 中央からステージ下端までのブロック数。
	const float STAGE_BOTTOM_Y = STAGE_BOTTOM_OFFSET_Y_FROM_CENTER * BLOCK_SIZE;	// ステージ下端のY座標。
	const Vector2 STAGE_ORIGIN_POSITION = Vector2(STAGE_LEFT_X, STAGE_BOTTOM_Y);	// ステージの一番左下の座標。
}

FieldManager::FieldManager()
{
	// ステージの画像を設定。
	stageSpriteRender.Init("Assets/Sprite/Stage/Stage.dds",
		BLOCK_SIZE * STAGE_TOTAL_WIDTH_IN_BLOCKS, BLOCK_SIZE * STAGE_TOTAL_HEIGHT_IN_BLOCKS);

	// ステージ管理用の空オブジェクトを配置。
	SetupCheckFields();
}

/// <summary>
/// フィールド上にテトリミノのブロックを保存し、各ブロックのスプライトを配置・更新します。
/// </summary>
/// <param name="gridPos">テトリミノの各ブロックのフィールド上の座標（Vector2型の配列）。</param>
/// <param name="blockSpriteRender">各ブロックに対応するスプライトレンダラー（SpriteRender型の配列）。</param>
void FieldManager::SaveTetrimino(const std::array<Vector2, MINO_PARTS_COUNT>& gridPos,
	std::array<SpriteRender*, MINO_PARTS_COUNT> blockSpriteRender)
{
	for (int i = 0; i < MINO_PARTS_COUNT; ++i) {
		auto& checkField = checkFields[gridPos[i].x][gridPos[i].y];

		// すでにブロックがある場合はゲームオーバー。
		if (checkField.isThereBlock) {
			DeleteGO(m_tetrimino);
			m_game->OnGameOver();
			return;
		}
		// ブロックがなければ、ブロックを配置。
		else {
			checkField.isThereBlock = true;
		}

		// テトリミノと同じ位置に同じブロック画像を描画。
		checkField.spriteRender = blockSpriteRender[i];
		checkField.spriteRender->SetPosition({ checkField.position.x, checkField.position.y, 0.0f });
		checkField.spriteRender->Update();
	}

	// テトリミノを保存したときの効果音を鳴らす。
	SoundManager* sound = FindGO<SoundManager>("SoundManager");
	sound->SoundNewGO(enSoundList_SaveMinoSE);

	// ラインが揃っているかチェックし、揃っていたら消す。
	CheckFullLines();

	// 新しいテトリミノを生成。
	DeleteGO(m_tetrimino);
	m_tetrimino = NewGO<Tetrimino>(0, "Tetrimino");

	m_isAllowedToHold = true;
}

bool FieldManager::Start()
{
	m_game = FindGO<Game>("Game");
	m_scoreManager = FindGO<ScoreManager>("ScoreManager");
	m_tetrimino = NewGO<Tetrimino>(0, "Tetrimino");
	m_holdView = FindGO<HoldView>("HoldView");
	return true;
}

void FieldManager::Update()
{
	if (!m_isAllowedToHold) {
		return;
	}

	if (g_pad[0]->IsTrigger(enButtonX) && !m_isHolded) {
		HoldMino();
		m_holdView->UpdateHoldView();
		m_isAllowedToHold = false;
	}
	else if (g_pad[0]->IsTrigger(enButtonX) && m_isHolded) {
		SwapHoldedMino();
		m_holdView->UpdateHoldView();
		m_isAllowedToHold = false;
	}
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
/// フィールド内の全てのラインを埋まっているかチェックし、埋まっているラインを消去します。
/// </summary>
void FieldManager::CheckFullLines()
{
	//　ラインを消す前に、Tスピンかどうかをチェック。
	int isTSpin = CheckIsTSpin();

	// 同時に消したライン数を計算。
	int numClearedLines = LinesClearAndCalcClearedLineCount();

	// コンボ数を計算。
	UpdateComboCount(numClearedLines);

	// ラインを消した数に応じて、効果音を鳴らす。
	if (numClearedLines > 0) {
		PlaySoundOnLineClear(numClearedLines);
	}

	// Tスピンの有無、消したライン数、コンボに応じてスコアを加算。
	m_scoreManager->AddScore(isTSpin, numClearedLines, m_comboCount);
}

/// <summary>
/// フィールド内の全てのラインを埋まっているかチェックし、埋まっているラインを消去します。
/// </summary>
/// <returns>同時に消したライン数。</returns>
int FieldManager::LinesClearAndCalcClearedLineCount()
{
	int clearLineCount = 0; // 1ライン消したときのスコア。
	for (int y = 0; y < PLAYABLE_HEIGHT_IN_BLOCKS; y++) {
		int countBlocksInLine = 0;// 1行にあるブロックの数のカウント。
		for (int x = 0; x < PLAYABLE_WIDTH_IN_BLOCKS; x++) {
			if (checkFields[x][y].isThereBlock) {
				countBlocksInLine++;
			}
			else { break; }
		}

		if (countBlocksInLine == PLAYABLE_WIDTH_IN_BLOCKS) {
			// ラインを消す。
			ClearFullLine(y);
			// 同時消ししたライン数を増やす。
			clearLineCount++;
			// ラインを消した分、yを1つ戻す。
			y--;
		}
	}
	return clearLineCount;
}

/// <summary>
/// コンボ数を更新します。
/// </summary>
void FieldManager::UpdateComboCount(int numClearedLines)
{
	// ラインを消した数に応じて、コンボ数を増やす。
	if (numClearedLines > 0) {
		m_comboCount++;
	}
	else {
		m_comboCount = 0;
	}
}

/// <summary>
/// 指定されたラインを消します。
/// </summary>
/// <param name="lineY">消すラインのY座標。</param>
void FieldManager::ClearFullLine(int lineYToClear)
{
	// まず、消える行のスプライトを delete
	for (int x = 0; x < PLAYABLE_WIDTH_IN_BLOCKS; x++) {
		if (checkFields[x][lineYToClear].spriteRender != nullptr) {
			delete checkFields[x][lineYToClear].spriteRender;
			checkFields[x][lineYToClear].spriteRender = nullptr;
		}
	}

	ShiftLines(lineYToClear);
}

/// <summary>
/// 指定されたラインより上のラインを1つずつ下にずらします。
/// </summary>
/// <param name="posYForFullLine">消されたラインのY座標。</param>
void FieldManager::ShiftLines(int posYForFullLine)
{
	// 上の行を1つずつ下にずらす。
	for (int y = posYForFullLine; y < PLAYABLE_HEIGHT_IN_BLOCKS - 1; y++) {
		for (int x = 0; x < PLAYABLE_WIDTH_IN_BLOCKS; x++) {
			checkFields[x][y].isThereBlock = checkFields[x][y + 1].isThereBlock;
			checkFields[x][y].spriteRender = checkFields[x][y + 1].spriteRender;

			// スプライトがnullptrの場合はスキップ。
			if (checkFields[x][y].spriteRender == nullptr) {
				continue;
			}

			// スプライトの位置を更新。
			checkFields[x][y].spriteRender->SetPosition({ checkFields[x][y].position.x, checkFields[x][y].position.y, 0.0f });
			checkFields[x][y].spriteRender->Update();
		}
	}
}

/// <summary>
/// 同時に消したラインの数に応じてSEを再生します。
/// </summary>
void FieldManager::PlaySoundOnLineClear(int numClearedLines)
{
	SoundManager* sound = FindGO<SoundManager>("SoundManager");
	switch (numClearedLines) {
	case 1:
		sound->SoundNewGO(enSoundList_SingleSE);
		break;
	case 2:
		sound->SoundNewGO(enSoundList_DoubleSE);
		break;
	case 3:
		sound->SoundNewGO(enSoundList_TripleSE);
		break;
	case 4:
		sound->SoundNewGO(enSoundList_TetrisSE);
		break;
	default:
		break;
	}
}

/// <summary>
/// テトリミノの各ブロックの位置とスプライトレンダーを保存します。
/// </summary>
void FieldManager::HoldMino()
{
	m_holdedMinoKind = m_tetrimino->GetMinoKind();
	m_isHolded = true;
	DeleteGO(m_tetrimino);
	m_tetrimino = NewGO<Tetrimino>(0, "Tetrimino");
}

/// <summary>
/// 保持中のミノと操作中のミノを入れ替えます。
/// </summary>
void FieldManager::SwapHoldedMino()
{
	int activeMinoKind = m_tetrimino->GetMinoKind();
	m_tetrimino->SetMinoKind(m_holdedMinoKind);
	m_tetrimino->Reset();
	m_holdedMinoKind = activeMinoKind;
}

/// <summary>
/// Tスピンかどうかを判定します。
/// </summary>
bool FieldManager::CheckIsTSpin()
{
	if (m_tetrimino->GetMinoKind() != static_cast<int>(EnMinoKinds::enMinoKinds_T)) {
		return false;
	}

	int filledCorners = 0;
	Vector2 pivot = m_tetrimino->GetPivotGridPosition();
	// ピボットの周囲4隅の座標を計算
	std::array<Vector2, 4> corners = {
		Vector2(pivot.x - 1, pivot.y + 1), // 左上
		Vector2(pivot.x + 1, pivot.y + 1), // 右上
		Vector2(pivot.x - 1, pivot.y - 1), // 左下
		Vector2(pivot.x + 1, pivot.y - 1)  // 右下
	};
	// 各隅がフィールド外またはブロックで埋まっているかチェック
	for (const auto& corner : corners) {
		int x = static_cast<int>(corner.x);
		int y = static_cast<int>(corner.y);
		if (x < 0 || x >= PLAYABLE_WIDTH_IN_BLOCKS || y < 0 || y >= PLAYABLE_HEIGHT_IN_BLOCKS) {
			filledCorners++;
			continue;
		}
		else if (checkFields[x][y].isThereBlock) {
			filledCorners++;
		}
	}
	// 埋まっている隅が3つ以上ならTスピンと判定
	if (filledCorners >= TSPIN_FILLED_CORNERS_THRESHOLD) {
		return true;
	}

	return false;
}