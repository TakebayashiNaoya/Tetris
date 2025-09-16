#pragma once
#include <array>
#include <vector>
#include "Tetrimino.h"
#include "BlockSpriteList.h"

class Tetrimino;

namespace
{
	const int PLAYABLE_WIDTH_IN_BLOCKS = 10;										// �e�g���~�m��z�u�ł���͈́i���̃u���b�N���j
	const int PLAYABLE_HEIGHT_IN_BLOCKS = 20;										// �e�g���~�m��z�u�ł���͈́i�c�̃u���b�N���j

	const int STAGE_TOTAL_WIDTH_IN_BLOCKS = 12;										// �g���܂߂����̃u���b�N���B
	const int STAGE_TOTAL_HEIGHT_IN_BLOCKS = 21;									// �g���܂߂��c�̃u���b�N���B

	const float STAGE_LEFT_OFFSET_X_FROM_CENTER = -4.5f;							// ��������X�e�[�W���[�܂ł̃u���b�N���B
	const float STAGE_LEFT_X = STAGE_LEFT_OFFSET_X_FROM_CENTER * BLOCK_SIZE;		// �X�e�[�W���[��X���W�B
	const float STAGE_BOTTOM_OFFSET_Y_FROM_CENTER = -9.0f;							// ��������X�e�[�W���[�܂ł̃u���b�N���B
	const float STAGE_BOTTOM_Y = STAGE_BOTTOM_OFFSET_Y_FROM_CENTER * BLOCK_SIZE;	// �X�e�[�W���[��Y���W�B
	const Vector2 STAGE_ORIGIN_POSITION = { STAGE_LEFT_X,STAGE_BOTTOM_Y };			// �X�e�[�W�̈�ԍ����̍��W�B
}

class FieldManager : public IGameObject
{
public:
	/// <summary>
	/// ��u���b�N�̏��B
	/// </summary>
	struct OneBlockOfFieldInfo
	{
		SpriteRender* spriteRender = nullptr;	// �X�v���C�g�����_�[�̃A�h���X�B
		Vector2 position = Vector2::Zero;		// �u���b�N�̈ʒu�B
		bool isThereBlock = false;				// �u���b�N�̗L���B
	};

	FieldManager();

	/// <summary>
	/// �w�肵���O���b�h���W�̃`�F�b�N�t�B�[���h�̈ʒu���擾���܂��B
	/// </summary>
	/// <param name="grid_x">�O���b�h��X���W�B</param>
	/// <param name="grid_y">�O���b�h��Y���W�B</param>
	/// <returns>�w�肵���O���b�h���W�ɂ���`�F�b�N�t�B�[���h�̈ʒu�iVector2�^�j�B</returns>
	Vector2 GetCheckFieldPosition(int grid_x, int grid_y)
	{
		return checkFields[grid_x][grid_y].position;
	}

	/// <summary>
	/// �w�肳�ꂽ�O���b�h���W�Ƀu���b�N�����݂��邩�ǂ����𔻒肵�܂��B
	/// </summary>
	/// <param name="grid_x">�O���b�h��X���W�B</param>
	/// <param name="grid_y">�O���b�h��Y���W�B</param>
	/// <returns>�w�肵�����W�Ƀu���b�N�����݂���ꍇ�� true�A���݂��Ȃ��ꍇ�� false ��Ԃ��܂��B</returns>
	bool GetCheckFieldFlag(int grid_x, int grid_y)
	{
		return checkFields[grid_x][grid_y].isThereBlock;
	}

	/// <summary>
	/// �e�g���~�m�̊e�u���b�N�̈ʒu�ƃX�v���C�g�����_�[��ۑ����܂��B
	/// </summary>
	/// <param name="gridPos">�e�g���~�m�̊e�u���b�N�̃O���b�h��̈ʒu��\���z��B</param>
	/// <param name="blockSpriteRender">�e�u���b�N�̃X�v���C�g�����_�[�ւ̃|�C���^�̔z��B</param>
	void SaveTetrimino(const std::array<Vector2, MINO_PARTS_COUNT>& gridPos,
		std::array<SpriteRender*, MINO_PARTS_COUNT> blockSpriteRender);

private:
	bool Start()override final;
	void Render(RenderContext& rc)override final;

	/// <summary>
	/// �t�B�[���h���Ǘ������I�u�W�F�N�g��z�u���鏈���B
	/// </summary>
	void SetupCheckFields();

	/// <summary>
	/// �e�g���~�m������t�B�[���h�̃u���b�N��`�悵�܂��B
	/// </summary>
	/// <param name="rc">�`��R���e�L�X�g�B�u���b�N�̕`��Ɏg�p����܂��B</param>
	void DrawBlocks(RenderContext& rc);

	/// <summary>
	/// �t�B�[���h��ɋ�u���b�N��z�u���A�e�u���b�N�̃X�v���C�g��z�u�E�X�V���܂��B
	/// </summary>
	std::array<std::array<OneBlockOfFieldInfo, PLAYABLE_HEIGHT_IN_BLOCKS>, PLAYABLE_WIDTH_IN_BLOCKS> checkFields;
	Vector2 stagePivotPosition = Vector2::Zero;

	SpriteRender stageSpriteRender;	// �X�e�[�W�̃X�v���C�g�����_�[�B

	Tetrimino* m_tetrimino;
};

