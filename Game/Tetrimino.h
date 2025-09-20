#pragma once

class FieldManager;
class BlockSpriteList;
class NextTetriminoView;
class ScoreManager;
class PauseView;

namespace
{
	constexpr int MINO_PARTS_COUNT = 4;	// �e�g���~�m���\������u���b�N���B
	constexpr int MINO_KINDS_COUNT = 7;	// �e�g���~�m�̎�ސ��B
}

class Tetrimino :public IGameObject
{
private:
	//=== ���C�t�T�C�N�� ===//
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;



	//=== �摜�Z�b�g�A�b�v�^�`�� ===//

	/// <summary>
	/// �e�g���~�m�̊e�u���b�N�摜�i�X�v���C�g�j�����������܂��B
	/// </summary>
	void SetupSpriteImage();

	/// <summary> 
	/// �e�u���b�N�̃��[���h���W���X�v���C�g�ɔ��f���܂��B
	/// </summary>
	void SetupSpritePosition();



	//=== ���W�v�Z�p�C�v���C�� ===//

	/// <summary>
	/// ��]�̊�ɂȂ�s�{�b�g�i��_�j���[���h���W��ݒ肵�܂��B
	/// </summary>
	void SetupPivotPosition();

	/// <summary> 
	/// ��_����̃f�t�H���g�i�������j���[�J�����W���v�Z���܂��B
	/// </summary>
	/// <summary> ��]��Ԃ̐؂�ւ��iA�ŉE��]�AB�ō���]�j�B</summary>
	void SwitchRotationState();

	/// <summary> 
	/// ��]��Ԃɉ����ă��[�J�����W���ړ������܂��B
	/// </summary>
	/// <param name="pos"> ��]������2�����x�N�g���B</param>
	/// <param name="rotationDeg"> ��]��ԁB</param>
	/// <returns> ��]���2�����x�N�g���B</returns>
	Vector2 Rotate(Vector2 pos, int rotationDeg);

	/// <summary> 
	/// Rotate�֐����g���A��]��Ԃɉ��������[�J�����W���v�Z���܂��B
	/// </summary>
	void CalcLocalGridPositionsForRotationState();

	/// <summary>
	/// ��_�ƃ��[�J�����W�����ƂɁA�e�u���b�N�̃O���[�o���O���b�h���W���v�Z���܂��B
	/// </summary>
	void CalcBlocksCurrentGlobalGridPositions();

	/// <summary> 
	/// X�AY�̃O���b�h���W�����ƂɁAFieldManager������W���擾�B
	/// </summary>
	void CalcBlocksCurrentGlobalPositions();



	//=== ���́^�ړ��E���� ===//

	/// <summary> 
	/// 1�u���b�N���A�����ɉ��ֈړ����܂��B
	/// </summary>
	void MoveDown();

	/// <summary> 
	/// 1�u���b�N���A�����ɍ��ֈړ����܂��B
	/// </summary>
	void MoveLeft();

	/// <summary> 
	/// 1�u���b�N���A�����ɉE�ֈړ����܂��B
	/// </summary>
	void MoveRight();

	/// <summary> 
	/// ���E�E�����͂ɂ��e�g���~�m�̕��s�ړ��B
	/// </summary>
	void HandleInputMovement();

	/// <summary>
	/// �e�g���~�m�̕������͂��������A�K�v�ɉ����Ĉړ��A�N�V���������s���܂��B
	/// </summary>
	/// <param name="button">��������������̓{�^���B</param>
	/// <param name="isBlocked">�ړ����u���b�N����Ă��邩�ǂ����������t���O�B</param>
	/// <param name="moveFunc">�e�g���~�m���ړ������邽�߂̊֐��I�u�W�F�N�g�B</param>
	void HandleDirectionalInput(EnButton button, bool isBlocked, std::function<void()> moveFunc);

	/// <summary> 
	/// ���Ԍo�߂ɂ�闎���i�d�́j��K�p���܂��B
	/// </summary>
	void AddGravity();



	//=== �Փ˔��� ===//

	/// <summary>
	/// �����ɃX�e�[�W�̒[�A�܂��͑��̃e�g���~�m�����邩�ǂ����𔻒肷�鏈���B
	/// </summary>
	/// <returns> �����ɃX�e�[�W�̒[�A�܂��͑��̃e�g���~�m�������true�A�Ȃ����false��Ԃ��B</returns>
	bool IsBlockedLeft();

	/// <summary>
	/// �E���ɃX�e�[�W�̒[�A�܂��͑��̃e�g���~�m�����邩�ǂ����𔻒肷�鏈���B
	/// </summary>
	/// <returns> �E���ɃX�e�[�W�̒[�A�܂��͑��̃e�g���~�m�������true�A�Ȃ����false��Ԃ��B</returns>
	bool IsBlockedRight();

	/// <summary>
	/// �����ɃX�e�[�W�̒[�A�܂��͑��̃e�g���~�m�����邩�ǂ����𔻒肷�鏈���B
	/// </summary>
	/// <returns> �����ɃX�e�[�W�̒[�A�܂��͑��̃e�g���~�m�������true�A�Ȃ����false��Ԃ��B</returns>
	bool IsBlockedBelow();



	//=== SRS(�X�[�p�[���[�e�[�V�����V�X�e��) ===//

	/// <summary>
	/// ��]�O�Ɖ�]��̏�ԂɊ�Â��āASRS�̃L�b�N�e�[�u���ɏ]���Ĉʒu�������s���܂��B
	/// </summary>
	/// <param name="beforeState">��]�O�̃e�g���~�m�̏�ԁi�p�x�j�B</param>
	/// <param name="currentState">��]��̃e�g���~�m�̏�ԁi�p�x�j�B</param>
	void SuperRotationSystem(int beforeState, int currentState);

	/// <summary>
	/// �����̒l�����e�g���~�m�̈ʒu��␳���A�␳��ɑ��̃e�g���~�m��X�e�[�W�O�ɏd�Ȃ��Ă��Ȃ������`�F�b�N���܂��B
	/// ��GetCheckFieldFlag(0�`9,0�`19)�͔͈͊O���w�肷��ƃG���[���N���邽�߁A�G���A�O�̃`�F�b�N���ɍs���Ă���B
	/// </summary>
	/// <param name="offsetX"> X���W�̕␳�l�B</param>
	/// <param name="offsetY"> Y���W�̕␳�l�B</param>
	/// <returns> �␳��ɔz�u�ł���ꍇ��true�A�ł��Ȃ��ꍇ��false��Ԃ��B</returns>
	bool SRS_Check(Vector2 offset);



	//=== �I�����̏��� ===//

	/// <summary>
	/// �e�g���~�m���ŉ����ɓ��B���邩�A���̃e�g���~�m�̏�ɏ������A�t�B�[���h�}�l�[�W���[�ɕۑ��B
	/// </summary> 
	bool SaveToFieldManager();



	//=== ���̑� ===//
	void CalcFallIntervalByLevel();

	//template <typename TetriminoCheckFunc>
	//void ForEachCheckFields(const TetriminoCheckFunc& func);
	//void SetupBlockPresenceFlags();



	FieldManager* m_fieldManager;
	BlockSpriteList* m_blockSpriteList;
	NextTetriminoView* m_nextTetriminoView;
	ScoreManager* m_scoreManager;
	PauseView* m_pauseView;

	std::array<SpriteRender*, MINO_PARTS_COUNT> m_blockSpriteRender;			// �e�u���b�N�̉摜

	std::array<Vector2, MINO_PARTS_COUNT> m_blocksCurrentLocalGridPositions;	// ���݃��[�J��(�O���b�h)
	std::array<Vector2, MINO_PARTS_COUNT> m_blocksCurrentGlobalGridPositions;	// ���݃��[���h(�O���b�h)
	std::array<Vector2, MINO_PARTS_COUNT> m_blocksCurrentGlobalPositions;		// ���݃��[���h

	Vector2 m_minoPivotGridPosition = Vector2::Zero;							// �e�g���~�m�̉�]��_�O���b�h���W���i�[����ϐ��B
	int m_rotationState = 0;													// �e�g���~�m�̉�]��Ԃ��i�[����ϐ��B
	int m_selectedMinoKind = 0;													// ��������e�g���~�m�̎�ނ��i�[����ϐ��B
	float m_fallTimer = 0.0f;													// �^�C�}�[�B
	float m_pressTimer = 0.0f;													// ���������܂ł̒������^�C�}�[�B
	float m_moveIntervalTimer = 0.0f;											// �ړ��̃C���^�[�o���^�C�}�[�B
	float m_deleteTimer = 0.0f;													// �e�g���~�m�폜�^�C�}�[�B
	float m_fallInterval = 0.0f;													// �������x�B
};