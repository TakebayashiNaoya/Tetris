#pragma once

class FieldManager;

const int MINO_PARTS_COUNT = 4;	// �e�g���~�m���\������u���b�N���B
const int MINO_KINDS_COUNT = 7;	// �e�g���~�m�̎�ސ��B

class Tetrimino :public IGameObject
{
private:
	/// <summary> �e�g���~�m�̉�]��ԁB</summary>
	enum EnRotationDeg
	{
		enRotationDeg_0,
		enRotationDeg_90,
		enRotationDeg_180,
		enRotationDeg_270,
		enRotationDeg_Num
	};

	//=== ���C�t�T�C�N�� ===//
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;


	//=== �摜�Z�b�g�A�b�v�^�`�� ===//
	/// <summary> �e�g���~�m�̊e�u���b�N�摜�i�X�v���C�g�j�����������܂��B</summary>
	void SetupSpriteImage();
	/// <summary> �e�u���b�N�̃��[���h���W���X�v���C�g�ɔ��f���܂��B</summary>
	void SetupSpritePosition();


	//=== ���W�v�Z�p�C�v���C�� ===//
	/// <summary> ��]�̊�ɂȂ�s�{�b�g�i��_�j���[���h���W��ݒ肵�܂��B</summary>
	void SetupPivotPosition();
	/// <summary> ��_����̃f�t�H���g�i�������j���[�J�����W���v�Z���܂��B</summary>
	void CalcInitialLocalPositions();
	/// <summary> ��]��Ԃ̐؂�ւ��iA�ŉE��]�AB�ō���]�j�B</summary>
	void SwitchRotationState();
	/// <summary> ��]��Ԃɉ����ă��[�J�����W���ړ������܂��B</summary>
	/// <param name="pos"> ��]������2�����x�N�g���B</param>
	/// <param name="rotationDeg"> ��]��ԁB</param>
	/// <returns> ��]���2�����x�N�g���B</returns>
	Vector2 Rotate(Vector2 pos, int rotationDeg);
	/// <summary> Rotate�֐����g���A��]��Ԃɉ��������[�J�����W���v�Z���܂��B</summary>
	void CalcLocalGridPositionsForRotationState();

	void CalcBlocksCurrentGlobalGridPositions();

	/// <summary> ���[�J�����W�{��_����A�e�u���b�N�̃��[���h���W���v�Z���܂��B</summary>
	void CalcBlocksCurrentGlobalPositions();


	//=== ���́^�ړ��E���� ===//
	/// <summary> 1�u���b�N���A�����ɉ��ֈړ����܂��B</summary>
	void MoveDown();
	/// <summary> 1�u���b�N���A�����ɍ��ֈړ����܂��B</summary>
	void MoveLeft();
	/// <summary> 1�u���b�N���A�����ɉE�ֈړ����܂��B</summary>
	void MoveRight();
	/// <summary> ���E�E�����͂ɂ��e�g���~�m�̕��s�ړ��B</summary>
	void HandleInputMovement();
	/// <summary> ���Ԍo�߂ɂ�闎���i�d�́j��K�p���܂��B</summary>
	void AddGravity();


	//=== �␳ ===//
	/// <summary> �ǃL�b�N�����݂܂��B</summary>
	void TryWallKick();
	/// <summary> �w�肳�ꂽ��ԂɊ�Â��āA�~�m�L�b�N���K�v���ǂ����𔻒肵�܂��B</summary>
	/// <param name="beforeState"> ����̊�ƂȂ钼�O�̏�Ԃ�\�������l�B</param>
	void CheckNeedNearMinoKick(int beforeState);
	/// <summary> �������ւ̃~�m�L�b�N�����s���܂��B</summary>
	void MinoKickLeft();
	/// <summary> �E�����ւ̃~�m�L�b�N�����s���܂��B</summary>
	void MinoKickRight();


	//=== ���n���� ===//
	/// <summary> �X�e�[�W�̍ŉ����ɓ��B�������ǂ����𔻒肵�܂��B</summary>
	/// <returns> �ŉ����ɓ��B�����ꍇ�� true�A�����łȂ��ꍇ�� false ��Ԃ��܂��B</returns>
	bool IsReachedStageBottom();
	/// <summary> ���Ƀe�g���~�m�����邩�ǂ����𔻒肵�܂��B</summary>
	/// <returns> ���Ƀe�g���~�m������ꍇ�� true�A�����łȂ��ꍇ�� false ��Ԃ��܂��B</returns>
	bool IsTetriminoBelow();


	//=== ���̑� ===//
	//template <typename TetriminoCheckFunc>
	//void ForEachCheckFields(const TetriminoCheckFunc& func);
	//void SetupBlockPresenceFlags();


	FieldManager* m_fieldManager;

	std::array<SpriteRender, MINO_PARTS_COUNT> blockSpriteRender;			// �e�u���b�N�̉摜
	std::array<Vector2, MINO_PARTS_COUNT> blocksInitialLocalPositions;		// �������[�J��
	std::array<Vector2, MINO_PARTS_COUNT> blocksCurrentLocalGridPositions;	// ���݃��[�J��(�O���b�h)
	std::array<Vector2, MINO_PARTS_COUNT> blocksCurrentGlobalPositions;		// ���݃��[���h
	std::array<Vector2, MINO_PARTS_COUNT> blocksCurrentGlobalGridPositions;	// ���݃��[���h(�O���b�h)

	std::array<Vector2, MINO_PARTS_COUNT> minoPartsNextFallPosition;			// ���ɗ������郏�[���h���W

	Vector2 minoPivotGridPosition = Vector2::Zero;						// �e�g���~�m�̉�]��_�O���b�h���W���i�[����ϐ��B
	int rotationState = 0;												// �e�g���~�m�̉�]��Ԃ��i�[����ϐ��B
	int selectedMinoKind = 0;											// ��������e�g���~�m�̎�ނ��i�[����ϐ��B
	float spawnTimer = 0.0f;											// �^�C�}�[�B
};