#pragma once
#include <array>

namespace
{
	constexpr float BLOCK_SIZE = 40.0f;	// 1�u���b�N�̃T�C�Y�B
}

/// <summary>
/// �u���b�N�X�v���C�g�̎�ނ�\���񋓌^�B
/// </summary>
enum class EnMinoKinds : uint8_t
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
	bool Start() override final;

	/// <summary> 
	/// �w�肵����ނ̃u���b�N�̃X�v���C�g�����_�[�̃A�h���X���擾����B
	/// </summary>
	SpriteRender* GetBlockSpriteRenderAddress(int kind)
	{
		return &blockSpriteRender[kind];
	}

private:
	/// <summary>
	/// �u���b�N�̃X�v���C�g�����_�[�̔z��B
	/// </summary>
	std::array<SpriteRender, static_cast<int>(EnMinoKinds::enMinoKinds_Num)> blockSpriteRender;
};

/// <summary>
/// �w�肳�ꂽ�C���f�b�N�X�Ɋ�Â��āASpriteRender �I�u�W�F�N�g�𐶐����܂��B
/// </summary>
class BlockCreateFactory
{
public:
	static SpriteRender* Create(const int index);
};



