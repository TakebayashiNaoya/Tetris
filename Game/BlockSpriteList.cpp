#include "stdafx.h"
#include "BlockSpriteList.h"

namespace
{
	const std::string TETRIMINO_FILE_PATH = "Assets/Tetrimino/Mino_";				// �e�g���~�m�̃t�@�C���p�X�B
	const std::string EXTENSION_DDS = ".dds";										// �X�v���C�g�̊g���q�B
	const char fileName[enMinoKinds_Num] = { 'I','J','L','O','S','T','Z' };	// �t�@�C���̖��O�B

	/// <summary>
	/// �w�肳�ꂽ�t�@�C��������t���p�X������𐶐����܂��B
	/// </summary>
	/// <param name="fileName"> �t�@�C������\��������B</param>
	/// <returns> �t�@�C�����Ƀp�X�Ɗg���q��t�������t���p�X�̕�����B</returns>
	std::string GetFullPath(char fileName)
	{
		return TETRIMINO_FILE_PATH + fileName + EXTENSION_DDS;
	}
}

bool BlockSpriteList::Start()
{
	// blockSpriteRender�Ƀu���b�N�̃X�v���C�g��Init����B
	for (int i = 0; i < enMinoKinds_Num; i++) {
		blockSpriteRender[i].Init(GetFullPath(fileName[i]).c_str(), 40.0f, 40.0f);
	}

	return true;
}



SpriteRender* BlockCreateFactory::Create(const int index)
{
	auto* render = new SpriteRender();
	render->Init(GetFullPath(fileName[index]).c_str(), BLOCK_SIZE, BLOCK_SIZE);
	return render;
}
