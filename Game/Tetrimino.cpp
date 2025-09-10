#include "stdafx.h"
#include "Tetrimino.h"
#include <time.h>

namespace
{
	std::string TETRIMINO_FILE_PATH = "Assets/Tetrimino/Mino_";	// �e�g���~�m�̃t�@�C���p�X�B
	std::string EXTENSION_DDS = ".dds";							// �X�v���C�g�̊g���q�B
	const float BLOCK_SIZE = 40.0f;								// �u���b�N�̃T�C�Y�B
	const Vector2 SPAWN_POSITION = { 0.0f,0.0f };				// �e�g���~�m�̃X�|�[���ʒu�B

	/// <summary>
	/// ��������e�g���~�m�̏��B
	/// </summary>
	struct MinoInfo
	{
		char fileName;										// �t�@�C���̖��O�B
		Vector2 minoPartsLocalPositionRatio[enParts_Num];	// �����̃e�g���~�m����̑��΍��W�䗦�B

		/// <summary>
		/// �e�g���~�m�̃X�v���C�g�̃t�@�C���p�X���擾���鏈���B
		/// </summary>
		std::string GetFullPath() const
		{
			return TETRIMINO_FILE_PATH + fileName + EXTENSION_DDS;
		}
	};

	/// <summary>
	/// �e�g���~�m�̃X�v���C�g�t�@�C�����Ƒ��΍��W�̈ꗗ�B
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
	// �e�g���~�m�����S�����_���ɂ��鏈���B
	srand(time(nullptr));

	// ��������e�g���~�m�̎�ނ𒊑I�B
	createMinoKind = rand() % enMinoKinds_Num;

	// �e�g���~�m�̉摜��ݒ肷��B
	SetupSprite();

	// �����u���b�N����̑��΍��W���v�Z�B
	CalcLocalPosition();

	// �e�g���~�m�̉�]��_�ʒu��ݒ�B
	SetupPivotPosition();

	return true;
}

void Tetrimino::Update()
{
	// ��]�̃X�e�[�g�𑀍�B
	SetupRotationState();

	// ��]�̃X�e�[�g�ɉ����ău���b�N�̈ړ���ʒu���v�Z�B
	CalcPositionForRotationState();

	// �e�g���~�m�̍��E���ړ�����B
	Move();

	// ���������B
	AddGravity();

	// �ړ���ʒu�������_�[�̃|�W�V�����ɐݒ�B
	SetupSpritePosition();
}

void Tetrimino::Render(RenderContext& rc)
{
	// �e�g���~�m�̊e�u���b�N��`��B
	for (int i = 0; i < enParts_Num; i++)
	{
		blockSpriteRender[i].Draw(rc);
	}
}

/// <summary>
/// �e�g���~�m�̉摜��ݒ肷�鏈���B
/// </summary>
void Tetrimino::SetupSprite()
{
	// ���I������ނɉ����ăX�v���C�g��ݒ�B
	for (int i = 0; i < enParts_Num; i++)
	{
		blockSpriteRender[i].Init(Minos[createMinoKind].GetFullPath().c_str(), BLOCK_SIZE, BLOCK_SIZE);
	}
}

/// <summary>
/// �����u���b�N����̊e�u���b�N�̍��W���Z�o�B
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
/// �e�g���~�m�̉�]��_�ʒu��ݒ�B
/// </summary>
void Tetrimino::SetupPivotPosition()
{
	// �e�g���~�m�̉�]��_�ʒu�ɃX�|�[���ʒu�����B
	minoPivotPosition = SPAWN_POSITION;

	// I�AO�̃~�m�͊�_����ɔ��u���b�N���炷�B
	if (Minos[createMinoKind].fileName == 'I' || Minos[createMinoKind].fileName == 'O')
	{
		minoPivotPosition.y += BLOCK_SIZE / 2;
	}
	// J�AL�AS�AT�AZ�̃~�m�͊�_���E�ɔ��u���b�N���炷�B
	else
	{
		minoPivotPosition.x += BLOCK_SIZE / 2;
	}
}

/// <summary>
/// ��]�̃X�e�[�g�𑀍삷�鏈���B
/// </summary>
void Tetrimino::SetupRotationState()
{
	// A�{�^������������E�ɉ�]�B
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

	// B�{�^�����������獶�ɉ�]�B
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
/// ��]�̃X�e�[�g�ɉ����āA�e�u���b�N�̍��W���v�Z���鏈���B
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
/// �e�g���~�m�����E���ɓ����������B
/// </summary>
void Tetrimino::Move()
{
	// ���{�^�����������獶�Ɉړ��B
	if (g_pad[0]->IsTrigger(enButtonLeft))
	{
		minoPivotPosition.x -= BLOCK_SIZE;
	}
	// �E�{�^������������E�Ɉړ��B
	if (g_pad[0]->IsTrigger(enButtonRight))
	{
		minoPivotPosition.x += BLOCK_SIZE;
	}
	// ���{�^�����������牺�Ɉړ��B
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		minoPivotPosition.y -= BLOCK_SIZE;
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
	minoPivotPosition.y -= BLOCK_SIZE * int(spawnTimer);

	// 1�b�o�����烊�Z�b�g�B
	if (spawnTimer > 1.0f)
	{
		spawnTimer = 0.0f;
	}
}

/// <summary>
/// �X�v���C�g�̈ʒu��ݒ肷�鏈���B
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
