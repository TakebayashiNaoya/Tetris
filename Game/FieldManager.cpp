#include "stdafx.h"
#include "Game.h"
#include "FieldManager.h"
#include "Tetrimino.h"

FieldManager::FieldManager()
{
	// �X�e�[�W�̉摜��ݒ�B
	stageSpriteRender.Init("Assets/Sprite/Stage/Stage.dds",
		BLOCK_SIZE * STAGE_TOTAL_WIDTH_IN_BLOCKS, BLOCK_SIZE * STAGE_TOTAL_HEIGHT_IN_BLOCKS);

	// �X�e�[�W�Ǘ��p�̋�I�u�W�F�N�g��z�u�B
	SetupCheckFields();
}

bool FieldManager::Start()
{
	m_game = FindGO<Game>("Game");
	m_scoreManager = FindGO<ScoreManager>("ScoreManager");
	m_tetrimino = NewGO<Tetrimino>(0, "Tetrimino");
	return true;
}

void FieldManager::Update()
{
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
/// �t�B�[���h���̑S�Ẵ��C���𖄂܂��Ă��邩�`�F�b�N���A���܂��Ă��郉�C�����������܂��B
/// </summary>
void FieldManager::CheckFullLines()
{
	int clearLineCount = 0; // 1���C���������Ƃ��̃X�R�A�B
	for (int y = 0; y < PLAYABLE_HEIGHT_IN_BLOCKS; y++) {
		int countBlocksInLine = 0;// 1�s�ɂ���u���b�N�̐��̃J�E���g�B
		for (int x = 0; x < PLAYABLE_WIDTH_IN_BLOCKS; x++) {
			if (checkFields[x][y].isThereBlock) {
				countBlocksInLine++;
			}
			else { break; }
		}
		// 1�s�Ƀu���b�N��10��������A�����s�̃J�E���g�𑝂₷�B
		if (countBlocksInLine == PLAYABLE_WIDTH_IN_BLOCKS) {
			ClearFullLine(y);
			clearLineCount++;
			y--; // ���C�������������Ay��1�߂��B
		}
	}
	m_scoreManager->AddScore(clearLineCount);
}

/// <summary>
/// �w�肳�ꂽ���C���������܂��B
/// </summary>
/// <param name="lineY">�������C����Y���W�B</param>
void FieldManager::ClearFullLine(int lineY)
{
	// �܂��A������s�̃X�v���C�g�� delete
	for (int x = 0; x < PLAYABLE_WIDTH_IN_BLOCKS; x++) {
		if (checkFields[x][lineY].spriteRender != nullptr) {
			delete checkFields[x][lineY].spriteRender;
			checkFields[x][lineY].spriteRender = nullptr;
		}
	}

	// ��̍s��1�����ɂ��炷�B
	for (int y = lineY; y < PLAYABLE_HEIGHT_IN_BLOCKS - 1; y++) {
		for (int x = 0; x < PLAYABLE_WIDTH_IN_BLOCKS; x++) {
			checkFields[x][y].isThereBlock = checkFields[x][y + 1].isThereBlock;
			checkFields[x][y].spriteRender = checkFields[x][y + 1].spriteRender;

			// �X�v���C�g��nullptr�̏ꍇ�̓X�L�b�v�B
			if (checkFields[x][y].spriteRender == nullptr) {
				continue;
			}

			// �X�v���C�g�̈ʒu���X�V�B
			checkFields[x][y].spriteRender->SetPosition({ checkFields[x][y].position.x, checkFields[x][y].position.y, 0.0f });
			checkFields[x][y].spriteRender->Update();
		}
	}
}

/// <summary>
/// �t�B�[���h��Ƀe�g���~�m�̃u���b�N��ۑ����A�e�u���b�N�̃X�v���C�g��z�u�E�X�V���܂��B
/// </summary>
/// <param name="gridPos">�e�g���~�m�̊e�u���b�N�̃t�B�[���h��̍��W�iVector2�^�̔z��j�B</param>
/// <param name="blockSpriteRender">�e�u���b�N�ɑΉ�����X�v���C�g�����_���[�iSpriteRender�^�̔z��j�B</param>
void FieldManager::SaveTetrimino(const std::array<Vector2, MINO_PARTS_COUNT>& gridPos,
	std::array<SpriteRender*, MINO_PARTS_COUNT> blockSpriteRender)
{
	for (int i = 0; i < MINO_PARTS_COUNT; ++i) {
		auto& checkField = checkFields[gridPos[i].x][gridPos[i].y];

		// ���łɃu���b�N������ꍇ�̓Q�[���I�[�o�[�B
		if (checkField.isThereBlock) {
			DeleteGO(m_tetrimino);
			m_game->OnGameOver();
			return;
		}
		// �u���b�N���Ȃ���΁A�u���b�N��z�u�B
		else {
			checkField.isThereBlock = true;
		}

		// �e�g���~�m�Ɠ����ʒu�ɓ����u���b�N�摜��`��B
		checkField.spriteRender = blockSpriteRender[i];
		checkField.spriteRender->SetPosition({ checkField.position.x, checkField.position.y, 0.0f });
		checkField.spriteRender->Update();
	}
	CheckFullLines();
	DeleteGO(m_tetrimino);
	m_tetrimino = NewGO<Tetrimino>(0, "Tetrimino");
}