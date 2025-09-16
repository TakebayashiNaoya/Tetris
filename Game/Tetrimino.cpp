#include "stdafx.h"
#include "Tetrimino.h"
#include <time.h>
#include "FieldManager.h"

namespace
{
	//std::string TETRIMINO_FILE_PATH = "Assets/Tetrimino/Mino_";	// �e�g���~�m�̃t�@�C���p�X�B
	//std::string EXTENSION_DDS = ".dds";							// �X�v���C�g�̊g���q�B
	const Vector2 SPAWN_GRID_POSITION = { 4,18 };				// �e�g���~�m�̃X�|�[���ʒu�B

	/// <summary>
	/// ��������e�g���~�m�̏��B
	/// </summary>
	//struct MinoInfo
	//{
	//	char fileName;										// �t�@�C���̖��O�B
	//Vector2 BlocksLocalPositionRatio[MINO_PARTS_COUNT];	// �����̃e�g���~�m����̑��΍��W�䗦�B

	//	/// <summary>
	//	/// �e�g���~�m�̃X�v���C�g�̃t�@�C���p�X���擾���鏈���B
	//	/// </summary>
	//	std::string GetFullPath() const
	//	{
	//		return TETRIMINO_FILE_PATH + fileName + EXTENSION_DDS;
	//	}
	//};

	/// <summary>
	/// �e�g���~�m�̃X�v���C�g�t�@�C�����Ƒ��΍��W�̈ꗗ�B
	/// </summary>
	Vector2 BlocksLocalPositionRatio[MINO_KINDS_COUNT][MINO_PARTS_COUNT] =
	{
		//	1�u���b�N��,	2�u���b�N��,	3�u���b�N��,	4�u���b�N�� 
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
	// �t�B�[���h�}�l�[�W���[���擾�B
	m_fieldManager = FindGO<FieldManager>("FieldManager");

	m_blockSpriteList = FindGO<BlockSpriteList>("BlockSpriteList");

	// �e�g���~�m�����S�����_���ɂ��鏈���B
	srand(time(nullptr));

	// ��������e�g���~�m�̎�ނ𒊑I�B
	selectedMinoKind = rand() % enMinoKinds_Num;

	// �e�g���~�m�̉摜��ݒ肷��B
	SetupSpriteImage();

	// �e�g���~�m�̉�]��_�ʒu��ݒ�B
	SetupPivotPosition();

	// �����u���b�N����̑��΍��W���v�Z�B
	CalcInitialLocalPositions();

	// ��]�̃X�e�[�g�ɉ����ău���b�N�̈ړ���ʒu���v�Z�B
	CalcLocalGridPositionsForRotationState();

	// �e�u���b�N�̃O���[�o���O���b�h���W���Z�o�B
	CalcBlocksCurrentGlobalGridPositions();

	// �e�u���b�N�̃��[���h���W���Z�o�B
	CalcBlocksCurrentGlobalPositions();

	return true;
}

void Tetrimino::Update()
{
	// ��]�̃X�e�[�g�𑀍�B
	SwitchRotationState();

	// �e�g���~�m�̍��E���ړ�����B
	HandleInputMovement();

	// ���������B
	AddGravity();

	// �u���b�N�̃O���[�o�����W���Z�o�B
	CalcBlocksCurrentGlobalPositions();

	// �ړ���ʒu�������_�[�̃|�W�V�����ɐݒ�B
	SetupSpritePosition();

	// �ŉ����ɓ��B�������A���̃e�g���~�m�̏�ɏ�������𔻒�B
	if (IsReachedStageBottom() || IsTetriminoBelow()) {
		// �t�B�[���h�Ƀu���b�N��ݒu�B
		m_fieldManager->SaveTetrimino(blocksCurrentGlobalGridPositions, blockSpriteRender);
	}
}

void Tetrimino::Render(RenderContext& rc)
{
	// �e�g���~�m�̊e�u���b�N��`��B
	for (auto sprite : blockSpriteRender) {
		sprite->Draw(rc);
	}
}



//=== �摜�Z�b�g�A�b�v�^�`�� ===//

/// <summary>
/// �e�g���~�m�̊e�u���b�N�摜�i�X�v���C�g�j�����������܂��B
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
/// �e�u���b�N�̃��[���h���W���X�v���C�g�ɔ��f���܂��B
/// </summary>
void Tetrimino::SetupSpritePosition()
{
	for (int i = 0; i < MINO_PARTS_COUNT; i++) {
		blockSpriteRender[i]->SetPosition({ blocksCurrentGlobalPositions[i].x, blocksCurrentGlobalPositions[i].y,0.0f });
		blockSpriteRender[i]->Update();
	}
}



//=== ���W�v�Z�p�C�v���C�� ===//

/// <summary> 
/// ��]�̊�ɂȂ�s�{�b�g�i��_�j���[���h���W��ݒ肵�܂��B
/// </summary>
void Tetrimino::SetupPivotPosition()
{
	// �e�g���~�m�̉�]��_�ʒu�ɃX�|�[���ʒu�����B
	minoPivotGridPosition = SPAWN_GRID_POSITION;

	// I�AO�̃~�m�͊�_����ɔ��u���b�N���炷�B
	if (selectedMinoKind == enMinoKinds_I || selectedMinoKind == enMinoKinds_O)
	{
		minoPivotGridPosition.x += 0.5;
		minoPivotGridPosition.y += 0.5;
	}
}

/// <summary>
/// ��_����̃f�t�H���g�i�������j���[�J�����W���v�Z���܂��B
/// </summary>
void Tetrimino::CalcInitialLocalPositions()
{
	for (int i = 0; i < MINO_PARTS_COUNT; i++) {
		blocksInitialLocalPositions[i].x = BlocksLocalPositionRatio[selectedMinoKind][i].x * BLOCK_SIZE;
		blocksInitialLocalPositions[i].y = BlocksLocalPositionRatio[selectedMinoKind][i].y * BLOCK_SIZE;
	}
}

/// <summary>
/// ��]�̃X�e�[�g�𑀍삷�鏈���B
/// </summary>
void Tetrimino::SwitchRotationState()
{
	// A�{�^���ŉE��]�B
	if (g_pad[0]->IsTrigger(enButtonA)) {
		int beforeState = rotationState;
		rotationState = (rotationState + 1) % enRotationDeg_Num;
		CalcLocalGridPositionsForRotationState();
		CalcBlocksCurrentGlobalGridPositions();

		TryWallKick();
		CheckNeedNearMinoKick(beforeState);
	}
	// B�{�^���ō���]�B
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
/// ��]��Ԃɉ����ă��[�J�����W���ړ������܂��B
/// </summary>
/// <param name="pos">��]������2�����x�N�g���B</param>
/// <param name="rotationDeg">��]��ԁB</param>
/// <returns>��]���2�����x�N�g���B</returns>
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
/// �e�g���~�m�̊e�u���b�N�̌��݂̉�]��Ԃɉ��������[�J�����W���v�Z���܂��B
/// </summary>
void Tetrimino::CalcLocalGridPositionsForRotationState()
{
	for (int i = 0; i < MINO_PARTS_COUNT; ++i) {
		blocksCurrentLocalGridPositions[i] = Rotate(BlocksLocalPositionRatio[selectedMinoKind][i], rotationState);
	}
}

/// <summary>
/// ��_����̃��[�J�����W�����ƂɁA�e�u���b�N�̃O���[�o���O���b�h���W���v�Z���܂��B
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
/// X�AY�̃O���b�h���W�����ƂɁAFieldManager������W���擾�B�B
/// </summary>
void Tetrimino::CalcBlocksCurrentGlobalPositions()
{
	for (int i = 0; i < MINO_PARTS_COUNT; i++) {
		blocksCurrentGlobalPositions[i] =
			m_fieldManager->GetCheckFieldPosition
			(blocksCurrentGlobalGridPositions[i].x, blocksCurrentGlobalGridPositions[i].y);
	}
}



//=== ���́^�ړ��E���� ===//

/// <summary> 
/// 1�u���b�N���A�����ɉ��ֈړ����܂��B
/// </summary>
void Tetrimino::MoveDown()
{
	minoPivotGridPosition.y--;
}

/// <summary> 
/// 1�u���b�N���A�����ɍ��ֈړ����܂��B
/// </summary>
void Tetrimino::MoveLeft()
{
	minoPivotGridPosition.x--;
}

/// <summary> 
/// 1�u���b�N���A�����ɉE�ֈړ����܂��B
/// </summary>
void Tetrimino::MoveRight()
{
	minoPivotGridPosition.x++;
}

/// <summary>
/// �{�^�����͂Ńe�g���~�m�����E���ɓ����������B
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
/// ���Ԍo�߂ŗ������鏈���B
/// </summary>
void Tetrimino::AddGravity()
{
	// �b�����J�E���g�B
	spawnTimer += g_gameTime->GetFrameDeltaTime();

	// 1�b�o������1�u���b�N����������B
	if (spawnTimer > 1.0f) {
		MoveDown();
		spawnTimer = 0.0f;
	}

	CalcBlocksCurrentGlobalGridPositions();
}

/// <summary>
/// �e�g���~�m���ǂɂ߂荞�񂾏ꍇ�ɁA�ʒu��␳���鏈���B
/// </summary>
void Tetrimino::TryWallKick()
{
	// ���E�ɂ͂ݏo���Ă���ʂ𒲂ׂ�B
	int overflowXLeft = 0;
	int overflowXRight = 0;

	for (auto& blockPos : blocksCurrentGlobalGridPositions)
	{
		// �G���A�O�ɏo�����̏����B
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

	// ��x�����␳
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
/// �ǃL�b�N���K�v���ǂ����𒲂ׁA�K�v�Ȃ�ΕǃL�b�N�����s���܂��B
/// </summary>
/// <param name="beforeState">��]�O�̃X�e�[�g������B�㉺���ړ��̏ꍇ��0������B</param>
void Tetrimino::CheckNeedNearMinoKick(int beforeState)
{
	// ���E�̃u���b�N�̗L���𒲂ׂ�B
	bool isThereBlockLeft = false;
	bool isThereBlockRight = false;

	for (auto& blockPos : blocksCurrentGlobalGridPositions) {
		// ���̃e�g���~�m�Əd�Ȃ��Ă���ꍇ�B
		if (m_fieldManager->GetCheckFieldFlag(blockPos.x, blockPos.y)) {
			// ��_��荶���̃u���b�N�ŁA���̃e�g���~�m�Əd�Ȃ��Ă���ꍇ�A���[�̃u���b�N�ʒu���X�V�B
			if (blockPos.x < minoPivotGridPosition.x) {
				isThereBlockLeft = true;
			}
			// ��_���E���̃u���b�N�ŁA���̃e�g���~�m�Əd�Ȃ��Ă���ꍇ�A�E�[�̃u���b�N�ʒu���X�V�B
			else if (blockPos.x > minoPivotGridPosition.x) {
				isThereBlockRight = true;
			}
		}
	}

	// ���E�����Ƀu���b�N������ꍇ�͕␳�ł��Ȃ��̂ŉ������Ȃ��B
	if (!isThereBlockLeft && !isThereBlockRight) { return; }
	// �����Ƀu���b�N������ꍇ�͉E�ɕ␳�B
	else if (isThereBlockLeft && !isThereBlockRight) { MinoKickLeft(); }
	// �E���Ƀu���b�N������ꍇ�͍��ɕ␳�B
	else if (!isThereBlockLeft && isThereBlockRight) { MinoKickRight(); }
	// ���E�����Ƀu���b�N������ꍇ�͕␳�ł��Ȃ��̂ŁA��]�O�̏�Ԃɖ߂��B
	else if (isThereBlockLeft && isThereBlockRight) {
		rotationState = beforeState;
		CalcLocalGridPositionsForRotationState();
		CalcBlocksCurrentGlobalGridPositions();
	}
}

/// <summary>
/// ������ǃL�b�N���鏈���B
/// </summary>
void Tetrimino::MinoKickLeft()
{
	// �������R��ꍇ�A�d�Ȃ�u���b�N��X���W�͊�_��菬�����Ȃ邽�߁A��_��X���W�������l�Ƃ���B
	int leftMostThereIsBlockPosition = minoPivotGridPosition.x;
	// �������R��ꍇ�A�d�Ȃ�Ȃ��u���b�N��X���W�͊�_���傫���Ȃ�\�������邽�߁A�E�[�̃O���b�h���W�������l�Ƃ���B
	int leftMostNotThereIsBlockPosition = PLAYABLE_WIDTH_IN_BLOCKS;

	for (auto& blockPos : blocksCurrentGlobalGridPositions) {
		// ���̃e�g���~�m�Əd�Ȃ��Ă���ꍇ�A�d�Ȃ��Ă��鍶�[�̃u���b�N�ʒu���X�V�B
		if (m_fieldManager->GetCheckFieldFlag(blockPos.x, blockPos.y)) {
			leftMostThereIsBlockPosition = min(leftMostThereIsBlockPosition, blockPos.x);
		}
		// ���̃e�g���~�m�Əd�Ȃ��Ă��Ȃ��ꍇ�A�d�Ȃ��Ă��Ȃ����[�̃u���b�N�ʒu���X�V�B
		else {
			leftMostNotThereIsBlockPosition = min(leftMostNotThereIsBlockPosition, blockPos.x);
		}
	}

	// �d�Ȃ��Ă��Ȃ����[�u���b�N�̈ʒu�ƁA�d�Ȃ��Ă��鍶�[�u���b�N�̈ʒu�̍�������_�ɉ��Z����B
	minoPivotGridPosition.x += leftMostNotThereIsBlockPosition - leftMostThereIsBlockPosition;

	CalcBlocksCurrentGlobalGridPositions();
}

/// <summary>
/// �E����ǃL�b�N���鏈���B
/// </summary>
void Tetrimino::MinoKickRight()
{
	// �E�����R��ꍇ�A�d�Ȃ�u���b�N��X���W�͊�_���傫���Ȃ邽�߁A��_��X���W�������l�Ƃ���B
	int rightMostThereIsBlockPosition = minoPivotGridPosition.x;
	// �E�����R��ꍇ�A�d�Ȃ�Ȃ��u���b�N��X���W�͊�_��菬�����Ȃ�\�������邽�߁A���[�̃O���b�h���W�������l�Ƃ���B
	int rightMostNotThereIsBlockPosition = 0;

	for (auto& blockPos : blocksCurrentGlobalGridPositions) {
		// ���̃e�g���~�m�Əd�Ȃ��Ă���ꍇ�A�d�Ȃ��Ă���E�[�̃u���b�N�ʒu���X�V�B
		if (m_fieldManager->GetCheckFieldFlag(blockPos.x, blockPos.y)) {
			rightMostThereIsBlockPosition = max(rightMostThereIsBlockPosition, blockPos.x);
		}
		// ���̃e�g���~�m�Əd�Ȃ��Ă��Ȃ��ꍇ�A�d�Ȃ��Ă��Ȃ��E�[�̃u���b�N�ʒu���X�V�B
		else {
			rightMostNotThereIsBlockPosition = max(rightMostNotThereIsBlockPosition, blockPos.x);
		}
	}
	// �d�Ȃ��Ă��Ȃ��E�[�u���b�N�̈ʒu�ƁA�d�Ȃ��Ă���E�[�u���b�N�̈ʒu�̍�������_�ɉ��Z����B
	minoPivotGridPosition.x += rightMostNotThereIsBlockPosition - rightMostThereIsBlockPosition;

	CalcBlocksCurrentGlobalGridPositions();
}

/// <summary>
/// �e�g���~�m���ŉ����ɓ��B�������ǂ����𔻒肷�鏈���B
/// </summary>
/// <returns>���B���Ă����true�A�����łȂ����false��Ԃ��B</returns>
bool Tetrimino::IsReachedStageBottom()
{
	// ���ɗ�������|�W�V������ۑ��B
	for (auto& blockPos : blocksCurrentGlobalGridPositions) {
		if (blockPos.y <= 0) {
			return true;
		}
	}
	return false;
}

/// <summary>
/// �e�g���~�m�̉��ɑ��̃e�g���~�m�����邩�ǂ����𔻒肷�鏈���B
/// </summary>
/// <returns>���ɑ��̃e�g���~�m�������true�A�Ȃ����false��Ԃ��B</returns>
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
///// �e�g���~�m�̑S�Ẵ`�F�b�N�t�B�[���h�ɑ΂��Ďw�肳�ꂽ�֐���K�p���܂��B
///// </summary>
///// <typeparam name="TetriminoCheckFunc">�`�F�b�N�t�B�[���h�ɑ΂��ČĂяo�����֐��I�u�W�F�N�g�̌^�B</typeparam>
///// <param name="func">�e�`�F�b�N�t�B�[���h�ɓK�p����֐��I�u�W�F�N�g�B</param>
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
///// �e�g���~�m�̌��݂̃u���b�N�ʒu�Ɋ�Â��āA�e�t�B�[���h�Ƀu���b�N�����݂��邩�ǂ����̃t���O��ݒ肵�܂��B
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
/// ��I�u�W�F�N�g�ƃe�g���~�m�̊e�u���b�N�̍��W���ƍ����鏈���B
/// </summary>
/// <param name="pos">�e�g���~�m���\������u���b�N�̔z�������B</param>
/// <returns>���W����v������true��Ԃ��A�����łȂ����false��Ԃ��B</returns>
//bool Tetrimino::IsEmptyAtPosition()
//{
//	// �X�e�[�W��̋�I�u�W�F�N�g�̍��W�����ɏ����B
//	for (auto& emptyBlock : m_fieldManager->GetCheckFields().position)
//	{
//		// �e�g���~�m�̊e�u���b�N�̍��W�����ɏ����B
//		for (int i = 0; i < enParts_Num; i++)
//		{
//			// ��I�u�W�F�N�g�ƃe�g���~�m�̊e�u���b�N�̍��W���ƍ����A��v������true��Ԃ��B
//			if (emptyBlock->position.x == pos[i].x && emptyBlock->position.y == pos[i].y)
//			{
//				return true;
//			}
//		}
//	}
//
//	return false;
//}
