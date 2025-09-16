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

	const float STAGE_RIGHT_OFFSET_X_FROM_CENTER = 4.5f;							// ��������X�e�[�W���[�܂ł̃u���b�N���B
	const float STAGE_RIGHT_X = STAGE_RIGHT_OFFSET_X_FROM_CENTER * BLOCK_SIZE;		// �X�e�[�W�E�[��X���W�B
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
	/// �X�e�[�W1�u���b�N���̏��B
	/// </summary>
	struct OneBlockOfFieldInfo
	{
		SpriteRender* spriteRender;
		Vector2 position = Vector2::Zero;
		bool isThereBlock = false;

		// �Z�b�^�[�֐�
		void SetIsThereBlock(bool value) {
			isThereBlock = value;
		};
	};

private:
	bool Start()override final;
	void Render(RenderContext& rc);

	/// <summary>
	/// �t�B�[���h���Ǘ������I�u�W�F�N�g��z�u���鏈���B
	/// </summary>
	void SetupCheckFields();

	void DrawBlocks(RenderContext& rc);

	std::array<std::array<OneBlockOfFieldInfo, PLAYABLE_HEIGHT_IN_BLOCKS>, PLAYABLE_WIDTH_IN_BLOCKS> checkFields;
	Vector2 stagePivotPosition = Vector2::Zero;

	SpriteRender stageSpriteRender;


	Tetrimino* m_tetrimino;

public:
	FieldManager();

	/// <summary>
	/// �v���C�\�ȃt�B�[���h�̕��ƍ����ɑΉ�����`�F�b�N�t�B�[���h�����擾���鏈���B
	/// </summary>
	auto& GetCheckFields()
	{
		return checkFields;
	};

	Vector2 GetCheckFieldPosition(int grid_x, int grid_y)
	{
		return checkFields[grid_x][grid_y].position;
	}

	bool GetCheckFieldFlag(int grid_x, int grid_y)
	{
		return checkFields[grid_x][grid_y].isThereBlock;
	}

	/// <summary>
	/// �`�F�b�N�t�B�[���h�̓���̈ʒu�Ƀu���b�N�����݂��邩�ǂ�����ݒ肷�鏈���B
	/// </summary>
	/// <param name="x">x�����̃u���b�N��</param>
	/// <param name="y">y�����̃u���b�N��</param>
	/// <param name="isThereBlock">�u���b�N������ꍇ��true�A�����ꍇ��false</param>
	void SetCheckField(int grid_x, int grid_y, bool isThereBlock)
	{
		checkFields[grid_x][grid_y].isThereBlock = isThereBlock;
	}

	void SaveTetrimino(const std::array<Vector2, MINO_PARTS_COUNT>& gridPos, std::array<SpriteRender*, MINO_PARTS_COUNT> blockSpriteRender);
};

