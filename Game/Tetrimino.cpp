#include "stdafx.h"
#include "Tetrimino.h"
#include <time.h>

namespace
{
	std::string TETRIMINO_FILE_PATH = "Assets/Tetrimino/Mino_";	// テトリミノのファイルパス。
	std::string EXTENSION_DDS = ".dds";							// スプライトの拡張子。
	const float BLOCK_SIZE = 40.0f;								// ブロックのサイズ。
	const Vector2 SPAWN_POSITION = { 0.0f,0.0f };				// テトリミノのスポーン位置。

	/// <summary>
	/// 生成するテトリミノの情報。
	/// </summary>
	struct MinoInfo
	{
		char fileName;										// ファイルの名前。
		Vector2 minoPartsLocalPositionRatio[enParts_Num];	// 中央のテトリミノからの相対座標比率。

		/// <summary>
		/// テトリミノのスプライトのファイルパスを取得する処理。
		/// </summary>
		std::string GetFullPath() const
		{
			return TETRIMINO_FILE_PATH + fileName + EXTENSION_DDS;
		}
	};

	/// <summary>
	/// テトリミノのスプライトファイル名と相対座標の一覧。
	/// </summary>
	MinoInfo Minos[enMinoKinds_Num] =
	{
		{'I',{{-1.5f,-0.5f},{-0.5f,-0.5f},{0.5f,-0.5f},{1.5f,-0.5f}}},
		{'J',{{0,0},{-1,1},{-1,0},{1,0}}},
		{'L',{{0,0},{-1,0},{1,0},{1,1}}},
		{'O',{{-0.5f,0.5f},{-0.5f,-0.5f},{0.5f,0.5f},{0.5f,-0.5f}}},
		{'S',{{0,0},{-1,0},{0,1},{1,1}}},
		{'T',{{0,0},{-1,0},{1,1},{1,0}}},
		{'Z',{{0,0},{-1,1},{0,1},{1,0}}}
	};

}

bool Tetrimino::Start()
{
	// テトリミノを完全ランダムにする処理。
	srand(time(nullptr));

	// 生成するテトリミノの種類を抽選。
	createMinoKind = rand() % enMinoKinds_Num;

	// テトリミノの画像を設定する。
	SetupSprite();

	// 中央ブロックからの相対座標を計算。
	CalcLocalPosition();

	// テトリミノの回転基点位置を設定。
	SetupPivotPosition();

	return true;
}

void Tetrimino::Update()
{
	// 回転のステートを操作。
	SetupRotationState();

	// 回転のステートに応じてブロックの移動先位置を計算。
	CalcPositionForRotationState();

	// テトリミノの左右下移動操作。
	Move();

	// 落下処理。
	AddGravity();

	// 移動先位置をレンダーのポジションに設定。
	SetupSpritePosition();
}

void Tetrimino::Render(RenderContext& rc)
{
	// テトリミノの各ブロックを描画。
	for (int i = 0; i < enParts_Num; i++)
	{
		blockSpriteRender[i].Draw(rc);
	}
}

/// <summary>
/// テトリミノの画像を設定する処理。
/// </summary>
void Tetrimino::SetupSprite()
{
	// 抽選した種類に応じてスプライトを設定。
	for (int i = 0; i < enParts_Num; i++)
	{
		blockSpriteRender[i].Init(Minos[createMinoKind].GetFullPath().c_str(), BLOCK_SIZE, BLOCK_SIZE);
	}
}

/// <summary>
/// 中央ブロックからの各ブロックの座標を算出。
/// </summary>
void Tetrimino::CalcLocalPosition()
{
	for (int i = 0; i < enParts_Num; i++)
	{
		minoPartsLocalPosition[i].x = Minos[createMinoKind].minoPartsLocalPositionRatio[i].x * BLOCK_SIZE;
		minoPartsLocalPosition[i].y = Minos[createMinoKind].minoPartsLocalPositionRatio[i].y * BLOCK_SIZE;
	}
}

/// <summary>
/// テトリミノの回転基点位置を設定。
/// </summary>
void Tetrimino::SetupPivotPosition()
{
	// テトリミノの回転基点位置にスポーン位置を代入。
	minoPivotPosition = SPAWN_POSITION;

	// I、Oのミノは基点を上に半ブロックずらす。
	if (Minos[createMinoKind].fileName == 'I' || Minos[createMinoKind].fileName == 'O')
	{
		minoPivotPosition.y += BLOCK_SIZE / 2;
	}
	// J、L、S、T、Zのミノは基点を右に半ブロックずらす。
	else
	{
		minoPivotPosition.x += BLOCK_SIZE / 2;
	}
}

/// <summary>
/// 回転のステートを操作する処理。
/// </summary>
void Tetrimino::SetupRotationState()
{
	// Aボタンを押したら右に回転。
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		if (rotationState == enRotation_270)
		{
			rotationState = enRotation_0;
		}
		else
		{
			rotationState++;
		}
	}

	// Bボタンを押したら左に回転。
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		if (rotationState == enRotation_0)
		{
			rotationState = enRotation_270;
		}
		else
		{
			rotationState--;
		}
	}
}

/// <summary>
/// 回転のステートに応じて、各ブロックの座標を計算する処理。
/// </summary>
void Tetrimino::CalcPositionForRotationState()
{
	switch (rotationState)
	{
	case enRotation_0:
		for (int i = 0; i < enParts_Num; i++)
		{
			minoPartsCarrentPosition[i].x = minoPartsLocalPosition[i].x + minoPivotPosition.x;
			minoPartsCarrentPosition[i].y = minoPartsLocalPosition[i].y + minoPivotPosition.y;
		}
		break;
	case enRotation_90:
		for (int i = 0; i < enParts_Num; i++)
		{
			minoPartsCarrentPosition[i].x = minoPartsLocalPosition[i].y + minoPivotPosition.x;
			minoPartsCarrentPosition[i].y = -minoPartsLocalPosition[i].x + minoPivotPosition.y;
		}
		break;
	case enRotation_180:
		for (int i = 0; i < enParts_Num; i++)
		{
			minoPartsCarrentPosition[i].x = -minoPartsLocalPosition[i].x + minoPivotPosition.x;
			minoPartsCarrentPosition[i].y = -minoPartsLocalPosition[i].y + minoPivotPosition.y;
		}
		break;
	case enRotation_270:
		for (int i = 0; i < enParts_Num; i++)
		{
			minoPartsCarrentPosition[i].x = -minoPartsLocalPosition[i].y + minoPivotPosition.x;
			minoPartsCarrentPosition[i].y = minoPartsLocalPosition[i].x + minoPivotPosition.y;
		}
		break;
	}
}

/// <summary>
/// テトリミノを左右下に動かす処理。
/// </summary>
void Tetrimino::Move()
{
	// 左ボタンを押したら左に移動。
	if (g_pad[0]->IsTrigger(enButtonLeft))
	{
		minoPivotPosition.x -= BLOCK_SIZE;
	}
	// 右ボタンを押したら右に移動。
	if (g_pad[0]->IsTrigger(enButtonRight))
	{
		minoPivotPosition.x += BLOCK_SIZE;
	}
	// 下ボタンを押したら下に移動。
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		minoPivotPosition.y -= BLOCK_SIZE;
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
	minoPivotPosition.y -= BLOCK_SIZE * int(spawnTimer);

	// 1秒経ったらリセット。
	if (spawnTimer > 1.0f)
	{
		spawnTimer = 0.0f;
	}
}

/// <summary>
/// スプライトの位置を設定する処理。
/// </summary>
void Tetrimino::SetupSpritePosition()
{
	for (int i = 0; i < enParts_Num; i++)
	{
		blockSpriteRender[i].SetPosition({ minoPartsCarrentPosition[i].x, minoPartsCarrentPosition[i].y,0.0f });
		blockSpriteRender[i].Update();
	}
}

bool Tetrimino::CanMoveDown()
{
	return false;
}
