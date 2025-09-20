#pragma once
#include <array>
#include "Types.h"

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



