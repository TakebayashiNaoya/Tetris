#include "stdafx.h"
#include "Tetrimino.h"
#include <time.h>
#include "FieldManager.h"
#include <algorithm>

namespace
{
	const Vector2 SPAWN_GRID_POSITION = Vector2(4.0f, 18.0f);	// �e�g���~�m�̃X�|�[���ʒu�B
	constexpr float DELETE_TIME = 0.9f;							// �e�g���~�m���ŉ����ɓ��B���Ă���t�B�[���h�ɌŒ肳���܂ł̎��ԁB
	constexpr float PIVOT_OFFSET = 0.5f;						// ���u���b�N�̃T�C�Y�B
	constexpr float REPEAT_MOVE_START_DELAY = 0.3f;				// �ړ��̃��s�[�g�J�n�܂ł̒x�����ԁB
	constexpr float REPEAT_MOVE_INTERVAL = 0.05f;				// �ړ��̃��s�[�g�Ԋu�B
	constexpr int offsetPatternForRotateState = 8;				// SRS�̃I�t�Z�b�g�p�^�[�����B
	constexpr int offsetCountPerPattern = 5;					// SRS��1�p�^�[��������̃I�t�Z�b�g���B

	/// <summary>
	/// �e�g���~�m�̃X�v���C�g�t�@�C�����Ƒ��΍��W�̈ꗗ�B
	/// </summary>
	Vector2 BlocksLocalPositionRatio[MINO_KINDS_COUNT][MINO_PARTS_COUNT] =
	{
		//	1�u���b�N��,	2�u���b�N��,	3�u���b�N��,	4�u���b�N�� 
			{{-1.5f,0.5f},	{-0.5f,0.5f},	{0.5f,0.5f},	{1.5f,0.5f} },	// I
			{{0,0},			{-1,1},			{-1,0},			{1,0}		},	// J
			{{0,0},			{-1,0},			{1,0},			{1,1}		},	// L
			{{-0.5f,0.5f},	{-0.5f,-0.5f},	{0.5f,0.5f},	{0.5f,-0.5f}},	// O
			{{0,0},			{-1,0},			{0,1},			{1,1}		},	// S
			{{0,0},			{-1,0},			{0,1},			{1,0}		},	// T
			{{0,0},			{-1,1},			{0,1},			{1,0}		}	// Z
	};

	/// <summary> 
	/// �e�g���~�m�̉�]��ԁB
	/// </summary>
	enum class EnRotationDeg : uint8_t
	{
		enRotationDeg_0,
		enRotationDeg_90,
		enRotationDeg_180,
		enRotationDeg_270,
		enRotationDeg_Num
	};

	struct SRSOffsetInfo
	{
		int beforeState;							// ��]�O�̏�ԁB
		int currentState;							// ��]��̏�ԁB
		Vector2 Offset[offsetCountPerPattern];		// 1�ڂ̃I�t�Z�b�g�B
	};

	SRSOffsetInfo SRSOffsetTableForNormal[offsetPatternForRotateState] =
	{
		//	��]�O�̏��										��]��̏��
		{	static_cast<int>(EnRotationDeg::enRotationDeg_0),	static_cast<int>(EnRotationDeg::enRotationDeg_90),
		//	1st�I�t�Z�b�g,		 2nd�I�t�Z�b�g,		  3rd�I�t�Z�b�g,		4th�I�t�Z�b�g,		 5th�I�t�Z�b�g
			{Vector2(0.0f,0.0f), Vector2(-1.0f,0.0f), Vector2(-1.0f,1.0f),  Vector2(0.0f,-2.0f), Vector2(-1.0f,-2.0f)}},

		{	static_cast<int>(EnRotationDeg::enRotationDeg_90),	static_cast<int>(EnRotationDeg::enRotationDeg_0),
			{Vector2(0.0f,0.0f), Vector2(1.0f,0.0f),  Vector2(1.0f,-1.0f),  Vector2(0.0f,2.0f),  Vector2(1.0f,2.0f)}},

		{	static_cast<int>(EnRotationDeg::enRotationDeg_90),	static_cast<int>(EnRotationDeg::enRotationDeg_180),
			{Vector2(0.0f,0.0f), Vector2(1.0f,0.0f),  Vector2(1.0f,-1.0f),  Vector2(0.0f,2.0f),  Vector2(1.0f,2.0f)}},

		{	static_cast<int>(EnRotationDeg::enRotationDeg_180),	static_cast<int>(EnRotationDeg::enRotationDeg_90),
			{Vector2(0.0f,0.0f), Vector2(-1.0f,0.0f), Vector2(-1.0f,1.0f),  Vector2(0.0f,-2.0f), Vector2(-1.0f,-2.0f)}},

		{	static_cast<int>(EnRotationDeg::enRotationDeg_180),	static_cast<int>(EnRotationDeg::enRotationDeg_270),
			{Vector2(0.0f,0.0f), Vector2(1.0f,0.0f),  Vector2(1.0f,1.0f),   Vector2(0.0f,-2.0f), Vector2(1.0f,-2.0f)}},

		{	static_cast<int>(EnRotationDeg::enRotationDeg_270),	static_cast<int>(EnRotationDeg::enRotationDeg_180),
			{Vector2(0.0f,0.0f), Vector2(-1.0f,0.0f), Vector2(-1.0f,-1.0f), Vector2(0.0f,2.0f),  Vector2(-1.0f,2.0f)}},

		{	static_cast<int>(EnRotationDeg::enRotationDeg_270),	static_cast<int>(EnRotationDeg::enRotationDeg_0),
			{Vector2(0.0f,0.0f), Vector2(-1.0f,0.0f), Vector2(-1.0f,-1.0f), Vector2(0.0f,2.0f),  Vector2(-1.0f,2.0f)}},

		{	static_cast<int>(EnRotationDeg::enRotationDeg_0),	static_cast<int>(EnRotationDeg::enRotationDeg_270),
			{Vector2(0.0f,0.0f), Vector2(1.0f,0.0f),  Vector2(1.0f,1.0f),   Vector2(0.0f,-2.0f), Vector2(1.0f,-2.0f)}}
	};

	SRSOffsetInfo SRSOffsetTableForI[offsetPatternForRotateState] =
	{
		//	��]�O�̏��										��]��̏��
		{	static_cast<int>(EnRotationDeg::enRotationDeg_0),	static_cast<int>(EnRotationDeg::enRotationDeg_90),
		//	1st�I�t�Z�b�g,		 2nd�I�t�Z�b�g,		  3rd�I�t�Z�b�g,	   4th�I�t�Z�b�g,		 5th�I�t�Z�b�g
			{Vector2(0.0f,0.0f), Vector2(-2.0f,0.0f), Vector2(1.0f,0.0f),  Vector2(-2.0f,-1.0f), Vector2(1.0f,2.0f)}},

		{	static_cast<int>(EnRotationDeg::enRotationDeg_90),	static_cast<int>(EnRotationDeg::enRotationDeg_0),
			{Vector2(0.0f,0.0f), Vector2(2.0f,0.0f),  Vector2(-1.0f,0.0f), Vector2(2.0f,1.0f),   Vector2(-1.0f,-2.0f)}},

		{	static_cast<int>(EnRotationDeg::enRotationDeg_90),	static_cast<int>(EnRotationDeg::enRotationDeg_180),
			{Vector2(0.0f,0.0f), Vector2(-1.0f,0.0f), Vector2(2.0f,0.0f),  Vector2(-1.0f,2.0f),  Vector2(2.0f,-1.0f)}},

		{	static_cast<int>(EnRotationDeg::enRotationDeg_180),	static_cast<int>(EnRotationDeg::enRotationDeg_90),
			{Vector2(0.0f,0.0f), Vector2(1.0f,0.0f),  Vector2(-2.0f,0.0f), Vector2(1.0f,-2.0f),  Vector2(-2.0f,1.0f)}},

		{	static_cast<int>(EnRotationDeg::enRotationDeg_180),	static_cast<int>(EnRotationDeg::enRotationDeg_270),
			{Vector2(0.0f,0.0f), Vector2(2.0f,0.0f),  Vector2(-1.0f,0.0f), Vector2(2.0f,1.0f),   Vector2(-1.0f,-2.0f)}},

		{	static_cast<int>(EnRotationDeg::enRotationDeg_270),	static_cast<int>(EnRotationDeg::enRotationDeg_180),
			{Vector2(0.0f,0.0f), Vector2(-2.0f,0.0f), Vector2(1.0f,0.0f),  Vector2(-2.0f,-1.0f), Vector2(1.0f,2.0f)}},

		{	static_cast<int>(EnRotationDeg::enRotationDeg_270),	static_cast<int>(EnRotationDeg::enRotationDeg_0),
			{Vector2(0.0f,0.0f), Vector2(1.0f,0.0f),  Vector2(-2.0f,0.0f), Vector2(1.0f,-2.0f),  Vector2(-2.0f,1.0f)}},

		{	static_cast<int>(EnRotationDeg::enRotationDeg_0),	static_cast<int>(EnRotationDeg::enRotationDeg_270),
			{Vector2(0.0f,0.0f), Vector2(-1.0f,0.0f), Vector2(2.0f,0.0f),  Vector2(-1.0f,2.0f),  Vector2(2.0f,-1.0f)}}
	};
};



bool Tetrimino::Start()
{
	// �t�B�[���h�}�l�[�W���[���擾�B
	m_fieldManager = FindGO<FieldManager>("FieldManager");

	m_blockSpriteList = FindGO<BlockSpriteList>("BlockSpriteList");

	// �e�g���~�m�����S�����_���ɂ��鏈���B
	srand(time(nullptr));

	// ��������e�g���~�m�̎�ނ𒊑I�B
	m_selectedMinoKind = rand() % static_cast<int>(EnMinoKinds::enMinoKinds_Num);

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
	if (m_selectedMinoKind == static_cast<int>(EnMinoKinds::enMinoKinds_I)
		|| m_selectedMinoKind == static_cast<int>(EnMinoKinds::enMinoKinds_O))
	{
		m_minoPivotGridPosition.x += PIVOT_OFFSET;
		m_minoPivotGridPosition.y += PIVOT_OFFSET;
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
		m_rotationState = (m_rotationState + 1) % static_cast<int>(EnRotationDeg::enRotationDeg_Num);
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
		m_rotationState = (m_rotationState + static_cast<int>(EnRotationDeg::enRotationDeg_Num) - 1)
			% static_cast<int>(EnRotationDeg::enRotationDeg_Num);
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
	case static_cast<int>(EnRotationDeg::enRotationDeg_0):   return pos;
	case static_cast<int>(EnRotationDeg::enRotationDeg_90):  return { pos.y, -pos.x };
	case static_cast<int>(EnRotationDeg::enRotationDeg_180): return { -pos.x, -pos.y };
	case static_cast<int>(EnRotationDeg::enRotationDeg_270): return { -pos.y, pos.x };
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
	m_fallTimer = 0.0f;
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
	HandleDirectionalInput(enButtonLeft, IsBlockedLeft(), [&] { MoveLeft(); });
	HandleDirectionalInput(enButtonRight, IsBlockedRight(), [&] { MoveRight(); });
	HandleDirectionalInput(enButtonDown, IsBlockedBelow(), [&] { MoveDown(); });
}

/// <summary>
/// �e�g���~�m�̕������͂��������A�K�v�ɉ����Ĉړ��A�N�V���������s���܂��B
/// </summary>
/// <param name="button">��������������̓{�^���B</param>
/// <param name="isBlocked">�ړ����u���b�N����Ă��邩�ǂ����������t���O�B</param>
/// <param name="moveFunc">�e�g���~�m���ړ������邽�߂̊֐��I�u�W�F�N�g�B</param>
void Tetrimino::HandleDirectionalInput(EnButton button, bool isBlocked, std::function<void()> moveFunc)
{
	// �g���K�[���͂ő�����1�u���b�N���ړ��B
	if (g_pad[0]->IsTrigger(button)) {
		if (isBlocked) return;
		moveFunc();
		CalcBlocksCurrentGlobalGridPositions();
		m_pressTimer = 0.0f;
	}

	// �������ŘA���ړ��B
	else if (g_pad[0]->IsPress(button)) {
		if (isBlocked) return;

		// ��莞�Ԓ�����������A���ړ��J�n�B
		m_pressTimer += g_gameTime->GetFrameDeltaTime();
		if (m_pressTimer < REPEAT_MOVE_START_DELAY) return;

		// ���Ԋu�ŘA���ړ��B
		m_moveIntervalTimer += g_gameTime->GetFrameDeltaTime();
		if (m_moveIntervalTimer > REPEAT_MOVE_INTERVAL) {
			moveFunc();
			CalcBlocksCurrentGlobalGridPositions();
			m_moveIntervalTimer = 0.0f;
		}
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
	SRSOffsetInfo useOffsetPattern[offsetPatternForRotateState];

	// I�~�m�Ƃ���ȊO�ŃI�t�Z�b�g�p�^�[�����قȂ邽�߁A�ꍇ�����B
	if (m_selectedMinoKind == static_cast<int>(EnMinoKinds::enMinoKinds_I)) {
		std::copy(std::begin(SRSOffsetTableForI), std::end(SRSOffsetTableForI), useOffsetPattern);
	}
	else {
		std::copy(std::begin(SRSOffsetTableForNormal), std::end(SRSOffsetTableForNormal), useOffsetPattern);
	}

	// ��]�O�Ɖ�]��̏�ԂɈ�v����I�t�Z�b�g�p�^�[����T���A���ɃI�t�Z�b�g�������B
	for (int i = 0; i < offsetPatternForRotateState; i++) {
		// ��]�O�Ɖ�]��̏�Ԃ���v����p�^�[�����������珈���ɓ���B�B
		if (beforeState == useOffsetPattern[i].beforeState && currentState == useOffsetPattern[i].currentState) {
			// �I�t�Z�b�g�p�^�[�������Ɏ����A�z�u�ł��邩�`�F�b�N�B
			for (int j = 0; j < offsetCountPerPattern; j++) {
				if (SRS_Check(useOffsetPattern[i].Offset[j])) { return; }
			}
			// �ǂ̃I�t�Z�b�g�p�^�[���ł��z�u�ł��Ȃ������ꍇ�A��]�O�̏�Ԃɖ߂��B
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
bool Tetrimino::SRS_Check(Vector2 offset)
{
	int OverlapCount = 0;
	for (auto& blockPos : m_blocksCurrentGlobalGridPositions) {
		// X���W���G���A�O�ɏo�Ă���ꍇ�B
		int x = blockPos.x + offset.x;
		if (x < 0 || x >= PLAYABLE_WIDTH_IN_BLOCKS) {
			OverlapCount++;
			continue;
		}
		// Y���W���G���A�O�ɏo�Ă���ꍇ�B
		int y = blockPos.y + offset.y;
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
		m_minoPivotGridPosition.x += offset.x;
		m_minoPivotGridPosition.y += offset.y;
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
		if (g_pad[0]->IsPress(enButtonDown)) {
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
