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

	stageSpriteRender.Draw(rc);
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
				arrayXY.spriteRender.Draw(rc);
			}
		}
	}
}

/// <summary>
/// �e�g���~�m�̃u���b�N�ʒu����ۑ����A�Ή�����X�v���C�g�����������ĕ`��ʒu��ݒ肵�܂��B
/// �����̃e�g���~�m�I�u�W�F�N�g���폜���A�V�����e�g���~�m�𐶐����܂��B
/// </summary>
/// <param name="gridPos">�e�g���~�m�̊e�u���b�N�̃O���b�h���W���i�[�����z��B</param>
/// <param name="path">�u���b�N�̃X�v���C�g�摜�t�@�C���ւ̃p�X�B</param>
void FieldManager::SaveTetrimino(const std::array<Vector2, MINO_PARTS_COUNT>& gridPos, const char* path)
{
	// �e�g���~�m����~�����Ƃ��̊e�u���b�N�̃O���b�h�ʒu���擾�B
	for (const auto& blockPos : gridPos) {
		auto& tmp = checkFields[blockPos.x][blockPos.y];
		// �`�F�b�N�t�B�[���h�̑Ή�����ʒu�Ƀu���b�N�����݂��邱�Ƃ�ݒ�B
		tmp.isThereBlock = true;
		// �e�g���~�m�Ɠ����ʒu�ɓ����u���b�N�摜��`��B
		tmp.spriteRender.Init(path, BLOCK_SIZE, BLOCK_SIZE);
		tmp.spriteRender.SetPosition({ tmp.position.x, tmp.position.y, 0.0f });
		tmp.spriteRender.Update();
	}
	DeleteGO(m_tetrimino);
	m_tetrimino = NewGO<Tetrimino>(0, "Tetrimino");
}