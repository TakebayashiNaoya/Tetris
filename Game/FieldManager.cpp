#include "stdafx.h"
#include "FieldManager.h"

namespace
{
	const float BLOCK_SIZE = 40.0f;
	const float STAGE_LEFT_OFFSET_X_FROM_CENTER = -4.5f;
	const float STAGE_BOTTOM_OFFSET_Y_FROM_CENTER = -9.0f;
	const Vector2 STAGE_ORIGIN_POSITION =
	{ BLOCK_SIZE * STAGE_LEFT_OFFSET_X_FROM_CENTER,BLOCK_SIZE * STAGE_BOTTOM_OFFSET_Y_FROM_CENTER };
	const int STAGE_WIDTH = 10;
	const int STAGE_HEIGHT = 20;
}

FieldManager::FieldManager()
{
	spriteRender.Init("Assets/Stage/Stage.dds", BLOCK_SIZE * 12, BLOCK_SIZE * 21);
	//SetupCheckFields();
}

bool FieldManager::Start()
{


	return true;
}

void FieldManager::Render(RenderContext& rc)
{
	spriteRender.Draw(rc);
}

/// <summary>
/// ステージのチェックポイント。
/// </summary>
void FieldManager::SetupCheckFields()
{
	for (int x = 0; x < STAGE_WIDTH; x++)
	{
		for (int y = 0; y < STAGE_HEIGHT; y++)
		{
			checkFields[x][y].position = { x * BLOCK_SIZE + STAGE_ORIGIN_POSITION.x,y * BLOCK_SIZE + STAGE_ORIGIN_POSITION.y };
		}
	}
}
