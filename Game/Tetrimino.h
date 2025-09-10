#pragma once

/// <summary>
/// �e�g���~�m�̎�ނ̗񋓌^�B
/// </summary>
enum EnMinoKinds
{
	enMinoKinds_I,
	enMinoKinds_J,
	enMinoKinds_L,
	enMinoKinds_O,
	enMinoKinds_S,
	enMinoKinds_T,
	enMinoKinds_Z,
	enMinoKinds_Num
};

/// <summary>
/// �e�g���~�m���\������u���b�N�����ʂ��邽�߂̗񋓌^�B
/// </summary>
enum EnParts
{
	enParts_1,
	enParts_2,
	enParts_3,
	enParts_4,
	enParts_Num
};

class Tetrimino :public IGameObject
{
private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

	/// <summary>
	/// �e�g���~�m�̉摜��ݒ肷�鏈���B
	/// </summary>
	void SetupSprite();

	/// <summary>
	/// �e�g���~�m���\�����Ă���e�u���b�N�̑��΍��W���Z�o���鏈���B
	/// </summary>
	void CalcLocalPosition();

	/// <summary>
	/// �e�g���~�m�̉�]��_���W���Z�o���鏈���B
	/// </summary>
	void SetupPivotPosition();

	/// <summary>
	/// �e�g���~�m�̉�]��Ԃ�؂�ւ��鏈���B
	/// </summary>
	void SetupRotationState();

	/// <summary>
	/// �e�g���~�m�̉�]��Ԃɉ����Ċe�u���b�N�̍��W���Z�o���鏈���B
	/// </summary>
	void CalcPositionForRotationState();

	/// <summary>
	/// �e�g���~�m�����E���ɓ����������B
	/// </summary>
	void Move();

	/// <summary>
	/// �e�g���~�m�𗎉������鏈���B
	/// </summary>
	void AddGravity();

	/// <summary>
	/// �X�v���C�g�̈ʒu��ݒ肷�鏈���B
	/// </summary>
	void SetupSpritePosition();

	/// <summary>
	/// �������Ɉړ��ł��邩�ǂ����𔻒肵�܂��B
	/// </summary>
	/// <returns>�������Ɉړ��\�ȏꍇ�� true�A�����łȂ��ꍇ�� false ��Ԃ��܂��B</returns>
	bool CanMoveDown();

	/// <summary>
	/// �e�g���~�m�̉�]��Ԃ��w���񋓌^�B
	/// ���e�g���~�m�͉E��]�B
	/// </summary>
	enum EnRotation
	{
		enRotation_0,
		enRotation_90,
		enRotation_180,
		enRotation_270
	};

	SpriteRender blockSpriteRender[enParts_Num];	// �e�g���~�m���\������e�u���b�N�̉摜���i�[����z��B
	Vector2 minoPartsLocalPosition[enParts_Num];	// �e�g���~�m���\�����Ă���e�u���b�N�̑��΍��W���i�[����z��B
	Vector2 minoPartsCarrentPosition[enParts_Num];	// �e�g���~�m���\�����Ă���e�u���b�N�̃��[���h���W���i�[����z��B
	Vector2 minoPivotPosition = Vector2::Zero;		// �e�g���~�m�̉�]��_���[���h���W���i�[����ϐ��B
	int rotationState = 0;							// �e�g���~�m�̉�]��Ԃ��i�[����ϐ��B
	int createMinoKind = 0;							// ��������e�g���~�m�̎�ނ��i�[����ϐ��B
	float spawnTimer = 0.0f;						// �^�C�}�[�B
};