#include "stdafx.h"
#include "Tetrimino.h"
#include <time.h>
#include "FieldManager.h"

namespace
{
	const Vector2 SPAWN_GRID_POSITION = { 4,18 };	// �e�g���~�m�̃X�|�[���ʒu�B
	const float DELETE_TIME = 0.9f;					// �e�g���~�m���ŉ����ɓ��B���Ă���t�B�[���h�ɌŒ肳���܂ł̎��ԁB

	/// <summary>
	/// �e�g���~�m�̃X�v���C�g�t�@�C�����Ƒ��΍��W�̈ꗗ�B
	/// </summary>
	Vector2 BlocksLocalPositionRatio[MINO_KINDS_COUNT][MINO_PARTS_COUNT] =
	{
		//	1�u���b�N��,	2�u���b�N��,	3�u���b�N��,	4�u���b�N�� 
			{{-1.5f,0.5f},	{-0.5f,0.5f},	{0.5f,0.5f},	{1.5f,0.5f}},	// I
			{{0,0},			{-1,1},			{-1,0},			{1,0}		},	// J
			{{0,0},			{-1,0},			{1,0},			{1,1}		},	// L
			{{-0.5f,0.5f},	{-0.5f,-0.5f},	{0.5f,0.5f},	{0.5f,-0.5f}},	// O
			{{0,0},			{-1,0},			{0,1},			{1,1}		},	// S
			{{0,0},			{-1,0},			{0,1},			{1,0}		},	// T
			{{0,0},			{-1,1},			{0,1},			{1,0}		}	// Z
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
	m_selectedMinoKind = rand() % enMinoKinds_Num;

	// �e�g���~�m�̉摜��ݒ肷��B
	SetupSpriteImage();

	// �e�g���~�m�̉�]��_�ʒu��ݒ�B
	SetupPivotPosition();

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

	// �Ō�Ƀt�B�[���h�ɌŒ肷��B
	SaveToFieldManager();
}

void Tetrimino::Render(RenderContext& rc)
{
	// �e�g���~�m�̊e�u���b�N��`��B
	for (auto sprite : m_blockSpriteRender) {
		sprite->Draw(rc);
	}
}



//=== �摜�Z�b�g�A�b�v�^�`�� ===//

/// <summary>
/// �e�g���~�m�̊e�u���b�N�摜�i�X�v���C�g�j�����������܂��B
/// </summary>
void Tetrimino::SetupSpriteImage()
{
	for (auto it = m_blockSpriteRender.begin(); it != m_blockSpriteRender.end(); ++it) {
		*it = BlockCreateFactory::Create(m_selectedMinoKind);
	}
}

/// <summary>
/// �e�u���b�N�̃��[���h���W���X�v���C�g�ɔ��f���܂��B
/// </summary>
void Tetrimino::SetupSpritePosition()
{
	for (int i = 0; i < MINO_PARTS_COUNT; i++) {
		m_blockSpriteRender[i]->SetPosition({ m_blocksCurrentGlobalPositions[i].x, m_blocksCurrentGlobalPositions[i].y,0.0f });
		m_blockSpriteRender[i]->Update();
	}
}



//=== ���W�v�Z�p�C�v���C�� ===//

/// <summary> 
/// ��]�̊�ɂȂ�s�{�b�g�i��_�j���[���h���W��ݒ肵�܂��B
/// </summary>
void Tetrimino::SetupPivotPosition()
{
	// �e�g���~�m�̉�]��_�ʒu�ɃX�|�[���ʒu�����B
	m_minoPivotGridPosition = SPAWN_GRID_POSITION;

	// I�AO�̃~�m�͊�_����ɔ��u���b�N���炷�B
	if (m_selectedMinoKind == enMinoKinds_I || m_selectedMinoKind == enMinoKinds_O)
	{
		m_minoPivotGridPosition.x += 0.5;
		m_minoPivotGridPosition.y += 0.5;
	}
}

/// <summary>
/// ��]�̃X�e�[�g�𑀍삷�鏈���B
/// </summary>
void Tetrimino::SwitchRotationState()
{
	// A�{�^���ŉE��]�B
	if (g_pad[0]->IsTrigger(enButtonA)) {
		// ��]��Ԃ�ۑ��B
		int beforeState = m_rotationState;
		// ��]��Ԃ��X�V�B
		m_rotationState = (m_rotationState + 1) % enRotationDeg_Num;
		CalcLocalGridPositionsForRotationState();
		CalcBlocksCurrentGlobalGridPositions();

		// ��]�O�̏�ԂƉ�]��̏�Ԃ����ƂɁASRS�����s�B
		SuperRotationSystem(beforeState, m_rotationState);
		CalcLocalGridPositionsForRotationState();
		CalcBlocksCurrentGlobalGridPositions();
	}
	// B�{�^���ō���]�B
	else if (g_pad[0]->IsTrigger(enButtonB)) {
		// ��]��Ԃ�ۑ��B
		int beforeState = m_rotationState;
		// ��]��Ԃ��X�V�B
		m_rotationState = (m_rotationState + enRotationDeg_Num - 1) % enRotationDeg_Num;
		CalcLocalGridPositionsForRotationState();
		CalcBlocksCurrentGlobalGridPositions();

		// ��]�O�̏�ԂƉ�]��̏�Ԃ����ƂɁASRS�����s�B
		SuperRotationSystem(beforeState, m_rotationState);
		CalcLocalGridPositionsForRotationState();
		CalcBlocksCurrentGlobalGridPositions();
	}
}

/// <summary>
/// ��]��Ԃɉ����ă��[�J�����W���ړ������܂��B
/// </summary>
/// <param name="pos">��]������2�����x�N�g���B</param>
/// <param name="rotationDeg">��]��ԁB</param>
/// <returns>��]���2�����x�N�g���B</returns>
Vector2 Tetrimino::Rotate(Vector2 pos, int rotationState)
{
	switch (rotationState)
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
		m_blocksCurrentLocalGridPositions[i] = Rotate(BlocksLocalPositionRatio[m_selectedMinoKind][i], m_rotationState);
	}
}

/// <summary>
/// ��_�ƃ��[�J�����W�����ƂɁA�e�u���b�N�̃O���[�o���O���b�h���W���v�Z���܂��B
/// </summary>
void Tetrimino::CalcBlocksCurrentGlobalGridPositions()
{
	for (int i = 0; i < MINO_PARTS_COUNT; i++) {
		m_blocksCurrentGlobalGridPositions[i] =
		{ m_blocksCurrentLocalGridPositions[i].x + m_minoPivotGridPosition.x,
		  m_blocksCurrentLocalGridPositions[i].y + m_minoPivotGridPosition.y };
	}
}

/// <summary>
/// X�AY�̃O���b�h���W�����ƂɁAFieldManager������W���擾�B
/// </summary>
void Tetrimino::CalcBlocksCurrentGlobalPositions()
{
	for (int i = 0; i < MINO_PARTS_COUNT; i++) {
		m_blocksCurrentGlobalPositions[i] =
			m_fieldManager->GetCheckFieldPosition
			(m_blocksCurrentGlobalGridPositions[i].x, m_blocksCurrentGlobalGridPositions[i].y);
	}
}



//=== ���́^�ړ��E���� ===//

/// <summary> 
/// 1�u���b�N���A�����ɉ��ֈړ����܂��B
/// </summary>
void Tetrimino::MoveDown()
{
	m_minoPivotGridPosition.y--;
}

/// <summary> 
/// 1�u���b�N���A�����ɍ��ֈړ����܂��B
/// </summary>
void Tetrimino::MoveLeft()
{
	m_minoPivotGridPosition.x--;
	m_deleteTimer = 0.0f;
}

/// <summary> 
/// 1�u���b�N���A�����ɉE�ֈړ����܂��B
/// </summary>
void Tetrimino::MoveRight()
{
	m_minoPivotGridPosition.x++;
	m_deleteTimer = 0.0f;
}

/// <summary>
/// �{�^�����͂Ńe�g���~�m�����E���ɓ����������B
/// </summary>
void Tetrimino::HandleInputMovement()
{
	if (g_pad[0]->IsTrigger(enButtonLeft)) {
		// ���Ɉړ��ł��Ȃ��ꍇ�͉������Ȃ��B
		if (IsBlockedLeft()) { return; }
		MoveLeft();
		CalcBlocksCurrentGlobalGridPositions();
	}
	if (g_pad[0]->IsTrigger(enButtonRight)) {
		// �E�Ɉړ��ł��Ȃ��ꍇ�͉������Ȃ��B
		if (IsBlockedRight()) { return; }
		MoveRight();
		CalcBlocksCurrentGlobalGridPositions();
	}
	if (g_pad[0]->IsTrigger(enButtonDown)) {
		// ���Ɉړ��ł��Ȃ��ꍇ�͉������Ȃ��B
		if (IsBlockedBelow()) { return; }
		MoveDown();
		CalcBlocksCurrentGlobalGridPositions();
	}
}

/// <summary>
/// ���Ԍo�߂ŗ������鏈���B
/// </summary>
void Tetrimino::AddGravity()
{
	// ���Ɉړ��ł��Ȃ��ꍇ�͉������Ȃ��B
	if (IsBlockedBelow()) { return; }

	// �b�����J�E���g�B
	m_fallTimer += g_gameTime->GetFrameDeltaTime();

	// 1�b�o������1�u���b�N����������B
	if (m_fallTimer > 1.0f) {
		MoveDown();
		m_fallTimer = 0.0f;
	}

	CalcBlocksCurrentGlobalGridPositions();
}



//=== �Փ˔��� ===//

/// <summary>
/// �����ɃX�e�[�W�̒[�A�܂��͑��̃e�g���~�m�����邩�ǂ����𔻒肷�鏈���B
/// ��GetCheckFieldFlag(0�`9,0�`19)�͔͈͊O���w�肷��ƃG���[���N���邽�߁A�G���A�O�̃`�F�b�N���ɍs���Ă���B
/// </summary>
/// <returns> �����ɃX�e�[�W�̒[�A�܂��͑��̃e�g���~�m�������true�A�Ȃ����false��Ԃ��B</returns>
bool Tetrimino::IsBlockedLeft()
{
	for (auto& blockPos : m_blocksCurrentGlobalGridPositions)
	{
		// �����ꂩ�̃u���b�N���X�e�[�W�̍��[�ɂ���ꍇ�B
		if (blockPos.x == 0) {
			return true;
		}
		// �����ꂩ�̃u���b�N�̍����ɑ��̃e�g���~�m������ꍇ�B
		else if (m_fieldManager->GetCheckFieldFlag(blockPos.x - 1, blockPos.y)) {
			return true;
		}
	}
	return false;
}

/// <summary>
/// �E���ɃX�e�[�W�̒[�A�܂��͑��̃e�g���~�m�����邩�ǂ����𔻒肷�鏈���B
/// ��GetCheckFieldFlag(0�`9,0�`19)�͔͈͊O���w�肷��ƃG���[���N���邽�߁A�G���A�O�̃`�F�b�N���ɍs���Ă���B
/// </summary>
/// <returns> �E���ɃX�e�[�W�̒[�A�܂��͑��̃e�g���~�m�������true�A�Ȃ����false��Ԃ��B</returns>
bool Tetrimino::IsBlockedRight()
{
	for (auto& blockPos : m_blocksCurrentGlobalGridPositions)
	{
		// �����ꂩ�̃u���b�N���X�e�[�W�̉E�[�ɂ���ꍇ�B
		if (blockPos.x == (PLAYABLE_WIDTH_IN_BLOCKS - 1)) {
			return true;
		}
		// �����ꂩ�̃u���b�N�̉E���ɑ��̃e�g���~�m������ꍇ�B
		else if (m_fieldManager->GetCheckFieldFlag(blockPos.x + 1, blockPos.y)) {
			return true;
		}
	}
	return false;
}

/// <summary>
/// �����ɃX�e�[�W�̒[�A�܂��͑��̃e�g���~�m�����邩�ǂ����𔻒肷�鏈���B
/// ��GetCheckFieldFlag(0�`9,0�`19)�͔͈͊O���w�肷��ƃG���[���N���邽�߁A�G���A�O�̃`�F�b�N���ɍs���Ă���B
/// </summary>
/// <returns> �����ɃX�e�[�W�̒[�A�܂��͑��̃e�g���~�m�������true�A�Ȃ����false��Ԃ��B</returns>
bool Tetrimino::IsBlockedBelow()
{
	for (auto& blockPos : m_blocksCurrentGlobalGridPositions)
	{
		// �����ꂩ�̃u���b�N���X�e�[�W�̉��[�ɂ���ꍇ�B
		if (blockPos.y == 0) {
			return true;
		}
		// �����ꂩ�̃u���b�N�̉����ɑ��̃e�g���~�m������ꍇ�B
		else if (m_fieldManager->GetCheckFieldFlag(blockPos.x, blockPos.y - 1)) {
			return true;
		}
	}
	return false;
}



//=== SRS(�X�[�p�[���[�e�[�V�����V�X�e��) ===//

/// <summary>
/// ��]�O�Ɖ�]��̏�ԂɊ�Â��āASRS�̃L�b�N�e�[�u���ɏ]���Ĉʒu�������s���܂��B
/// </summary>
/// <param name="beforeState">��]�O�̃e�g���~�m�̏�ԁi�p�x�j�B</param>
/// <param name="currentState">��]��̃e�g���~�m�̏�ԁi�p�x�j�B</param>
void Tetrimino::SuperRotationSystem(int beforeState, int currentState)
{
	if (m_selectedMinoKind == enMinoKinds_I) {
		SuperRotationSystemVersionI(beforeState, currentState);
	}
	else {
		SuperRotationSystemVersionNormal(beforeState, currentState);
	}
}

/// <summary>
/// I�̃e�g���~�m�̉�]���ɁASRS�̃L�b�N�e�[�u���ɏ]���Ĉʒu�������s���܂��B
/// ��]��Ɉʒu�������ł��Ȃ��ꍇ�́A��]�O�̏�Ԃɖ߂��܂��B
/// </summary>
/// <param name="beforeState">��]�O�̃e�g���~�m�̏�ԁi�p�x�j�B</param>
/// <param name="currentState">��]��̃e�g���~�m�̏�ԁi�p�x�j�B</param>
void Tetrimino::SuperRotationSystemVersionI(int beforeState, int currentState)
{
	if (beforeState == enRotationDeg_0 && currentState == enRotationDeg_90) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(-2, 0)) { return; }
		else if (SuperRotationSystemCheck(1, 0)) { return; }
		else if (SuperRotationSystemCheck(-2, -1)) { return; }
		else if (SuperRotationSystemCheck(1, 2)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_90 && currentState == enRotationDeg_0) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(2, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 0)) { return; }
		else if (SuperRotationSystemCheck(2, 1)) { return; }
		else if (SuperRotationSystemCheck(-1, -2)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_90 && currentState == enRotationDeg_180) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 0)) { return; }
		else if (SuperRotationSystemCheck(2, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 2)) { return; }
		else if (SuperRotationSystemCheck(2, -1)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_180 && currentState == enRotationDeg_90) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(1, 0)) { return; }
		else if (SuperRotationSystemCheck(-2, 0)) { return; }
		else if (SuperRotationSystemCheck(1, -2)) { return; }
		else if (SuperRotationSystemCheck(-2, 1)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_180 && currentState == enRotationDeg_270) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(2, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 0)) { return; }
		else if (SuperRotationSystemCheck(2, 1)) { return; }
		else if (SuperRotationSystemCheck(-1, -2)) { return; }
	}
	else if (beforeState == enRotationDeg_270 && currentState == enRotationDeg_180) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(-2, 0)) { return; }
		else if (SuperRotationSystemCheck(1, 0)) { return; }
		else if (SuperRotationSystemCheck(-2, -1)) { return; }
		else if (SuperRotationSystemCheck(1, 2)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_270 && currentState == enRotationDeg_0) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(1, 0)) { return; }
		else if (SuperRotationSystemCheck(-2, 0)) { return; }
		else if (SuperRotationSystemCheck(1, -2)) { return; }
		else if (SuperRotationSystemCheck(-2, 1)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_0 && currentState == enRotationDeg_270) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 0)) { return; }
		else if (SuperRotationSystemCheck(2, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 2)) { return; }
		else if (SuperRotationSystemCheck(2, -1)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
}

/// <summary>
/// I�ȊO�̃e�g���~�m�̉�]���ɁASRS�̃L�b�N�e�[�u���ɏ]���Ĉʒu�������s���܂��B
/// ��]��Ɉʒu�������ł��Ȃ��ꍇ�́A��]�O�̏�Ԃɖ߂��܂��B
/// </summary>
/// <param name="beforeState">��]�O�̃e�g���~�m�̏�ԁi�p�x�j�B</param>
/// <param name="currentState">��]��̃e�g���~�m�̏�ԁi�p�x�j�B</param>
void Tetrimino::SuperRotationSystemVersionNormal(int beforeState, int currentState)
{
	if (beforeState == enRotationDeg_0 && currentState == enRotationDeg_90) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 1)) { return; }
		else if (SuperRotationSystemCheck(0, -2)) { return; }
		else if (SuperRotationSystemCheck(-1, -2)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_90 && currentState == enRotationDeg_180) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(1, 0)) { return; }
		else if (SuperRotationSystemCheck(1, -1)) { return; }
		else if (SuperRotationSystemCheck(0, 2)) { return; }
		else if (SuperRotationSystemCheck(1, 2)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_180 && currentState == enRotationDeg_270) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(1, 0)) { return; }
		else if (SuperRotationSystemCheck(1, 1)) { return; }
		else if (SuperRotationSystemCheck(0, -2)) { return; }
		else if (SuperRotationSystemCheck(1, -2)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_270 && currentState == enRotationDeg_0) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, -1)) { return; }
		else if (SuperRotationSystemCheck(0, 2)) { return; }
		else if (SuperRotationSystemCheck(-1, +2)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_0 && currentState == enRotationDeg_270) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(1, 0)) { return; }
		else if (SuperRotationSystemCheck(1, 1)) { return; }
		else if (SuperRotationSystemCheck(0, -2)) { return; }
		else if (SuperRotationSystemCheck(1, -2)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_270 && currentState == enRotationDeg_180) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, -1)) { return; }
		else if (SuperRotationSystemCheck(0, 2)) { return; }
		else if (SuperRotationSystemCheck(-1, 2)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_180 && currentState == enRotationDeg_90) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 0)) { return; }
		else if (SuperRotationSystemCheck(-1, 1)) { return; }
		else if (SuperRotationSystemCheck(0, -2)) { return; }
		else if (SuperRotationSystemCheck(-1, -2)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
	else if (beforeState == enRotationDeg_90 && currentState == enRotationDeg_0) {
		if (SuperRotationSystemCheck(0, 0)) { return; }
		else if (SuperRotationSystemCheck(1, 0)) { return; }
		else if (SuperRotationSystemCheck(1, -1)) { return; }
		else if (SuperRotationSystemCheck(0, 2)) { return; }
		else if (SuperRotationSystemCheck(1, 2)) { return; }
		else {
			m_rotationState = beforeState;
			return;
		}
	}
}

/// <summary>
/// �����̒l�����e�g���~�m�̈ʒu��␳���A�␳��ɑ��̃e�g���~�m��X�e�[�W�O�ɏd�Ȃ��Ă��Ȃ������`�F�b�N���܂��B
/// ��GetCheckFieldFlag(0�`9,0�`19)�͔͈͊O���w�肷��ƃG���[���N���邽�߁A�G���A�O�̃`�F�b�N���ɍs���Ă���B
/// </summary>
/// <param name="offsetX"> X���W�̕␳�l�B</param>
/// <param name="offsetY"> Y���W�̕␳�l�B</param>
/// <returns> �␳��ɔz�u�ł���ꍇ��true�A�ł��Ȃ��ꍇ��false��Ԃ��B</returns>
bool Tetrimino::SuperRotationSystemCheck(int offsetX, int offsetY)
{
	int OverlapCount = 0;
	for (auto& blockPos : m_blocksCurrentGlobalGridPositions) {
		// X���W���G���A�O�ɏo�Ă���ꍇ�B
		int x = blockPos.x + offsetX;
		if (x < 0 || x >= PLAYABLE_WIDTH_IN_BLOCKS) {
			OverlapCount++;
			continue;
		}
		// Y���W���G���A�O�ɏo�Ă���ꍇ�B
		int y = blockPos.y + offsetY;
		if (y < 0 || y >= PLAYABLE_HEIGHT_IN_BLOCKS) {
			OverlapCount++;
			continue;
		}
		// ���̃e�g���~�m�Əd�Ȃ��Ă���ꍇ�B
		if (m_fieldManager->GetCheckFieldFlag(x, y)) {
			OverlapCount++;
		}
	}
	// �d�Ȃ��Ă��Ȃ���Ε␳��K�p�B
	if (OverlapCount == 0) {
		m_minoPivotGridPosition.x += offsetX;
		m_minoPivotGridPosition.y += offsetY;
		CalcBlocksCurrentGlobalGridPositions();
		return true;
	}
	// �d�Ȃ��Ă���Ε␳��K�p���Ȃ��B
	else {
		return false;
	}
}



//=== �I�����̏��� ===//

/// <summary>
/// �e�g���~�m���ŉ����ɓ��B���邩�A���̃e�g���~�m�̏�ɏ������A�t�B�[���h�}�l�[�W���[�ɕۑ��B
/// </summary> 
void Tetrimino::SaveToFieldManager()
{
	// �ŉ����ɓ��B�������A���̃e�g���~�m�̏�ɏ�������𔻒�B
	if (IsBlockedBelow()) {
		m_deleteTimer += g_gameTime->GetFrameDeltaTime();
		if (g_pad[0]->IsTrigger(enButtonDown)) {
			m_fieldManager->SaveTetrimino(m_blocksCurrentGlobalGridPositions, m_blockSpriteRender);
		}
		else if (m_deleteTimer > DELETE_TIME) {
			m_fieldManager->SaveTetrimino(m_blocksCurrentGlobalGridPositions, m_blockSpriteRender);
		}
	}
	// ���������Ȃ��Ȃ�����^�C�}�[���Z�b�g�B
	else {
		m_deleteTimer = 0.0f;
	}
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
