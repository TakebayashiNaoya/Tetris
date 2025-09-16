#pragma once
#include <array>

namespace
{
	const float BLOCK_SIZE = 40.0f;	// 1�u���b�N�̃T�C�Y�B
}

/// <summary>
/// �u���b�N�X�v���C�g�̎�ނ�\���񋓌^�B
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
	enMinoKinds_Num,
};

class BlockSpriteList :public IGameObject
{
public:
	/// <summary> 
	/// �w�肵����ނ̃u���b�N�̃X�v���C�g�����_�[�̃A�h���X���擾����B
	/// </summary>
	SpriteRender* GetBlockSpriteRenderAddress(int kind)
	{
		return &blockSpriteRender[kind];
	}

public:
	bool Start() override final;


private:
	/// <summary>
	/// �u���b�N�̃X�v���C�g�����_�[�̔z��B
	/// </summary>
	std::array<SpriteRender, enMinoKinds_Num> blockSpriteRender;

	//std::array<std::array<SpriteRender, MINO_PARTS_COUNT>, enMinoKinds_Num> blockSpriteRender;

};

/// <summary>
/// �w�肳�ꂽ�C���f�b�N�X�Ɋ�Â��āASpriteRender �I�u�W�F�N�g�𐶐����܂��B
/// </summary>
class BlockCreateFactory
{
public:
	static SpriteRender* Create(const int index);
};



