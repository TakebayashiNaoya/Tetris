#include "stdafx.h"
#include "FieldManager.h"
#include "Tetrimino.h"

FieldManager::FieldManager()
{
	// �X�e�[�W�̉摜��ݒ�B
	stageSpriteRender.Init("Assets/Stage/Stage.dds",
		BLOCK_SIZE * STAGE_TOTAL_WIDTH_IN_BLOCKS, BLOCK_SIZE * STAGE_TOTAL_HEIGHT_IN_BLOCKS);

	// �X�e�[�W�Ǘ��p�̋�I�u�W�F�N�g��z�u�B
	SetupCheckFields();
}

bool FieldManager::Start()
{
	m_tetrimino = NewGO<Tetrimino>(0, "Tetrimino");
	return true;
}

void FieldManager::Render(RenderContext& rc)
{
	// �X�e�[�W��`��B
	stageSpriteRender.Draw(rc);

	// �t�B�[���h�ɔz�u���ꂽ�u���b�N��`��B
	DrawBlocks(rc);
}

/// <summary>
/// �t�B�[���h���Ǘ������I�u�W�F�N�g��z�u���鏈���B
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
/// �e�g���~�m������t�B�[���h�̃u���b�N��`�悵�܂��B
/// </summary>
/// <param name="rc">�`��R���e�L�X�g�B�u���b�N�̕`��Ɏg�p����܂��B</param>
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
/// �t�B�[���h��Ƀe�g���~�m�̃u���b�N��ۑ����A�e�u���b�N�̃X�v���C�g��z�u�E�X�V���܂��B
/// </summary>
/// <param name="gridPos">�e�g���~�m�̊e�u���b�N�̃t�B�[���h��̍��W�iVector2�^�̔z��j�B</param>
/// <param name="blockSpriteRender">�e�u���b�N�ɑΉ�����X�v���C�g�����_���[�iSpriteRender�^�̔z��j�B</param>
void FieldManager::SaveTetrimino(const std::array<Vector2, MINO_PARTS_COUNT>& gridPos, std::array<SpriteRender*, MINO_PARTS_COUNT> blockSpriteRender)
{
	for (int i = 0; i < MINO_PARTS_COUNT; ++i) {
		auto& checkField = checkFields[gridPos[i].x][gridPos[i].y];
		checkField.isThereBlock = true;
		// �e�g���~�m�Ɠ����ʒu�ɓ����u���b�N�摜��`��B
		checkField.spriteRender = blockSpriteRender[i];
		checkField.spriteRender->SetPosition({ checkField.position.x, checkField.position.y, 0.0f });
		checkField.spriteRender->Update();
	}

	DeleteGO(m_tetrimino);
	m_tetrimino = NewGO<Tetrimino>(0, "Tetrimino");
}