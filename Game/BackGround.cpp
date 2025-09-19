#include "stdafx.h"
#include "BackGround.h"
#include <time.h>

namespace
{
	constexpr float FULL_HD_WIDTH = 1920.0f;	// �t��HD�̉����B
	constexpr float FULL_HD_HEIGHT = 1080.0f;	// �t��HD�̏c���B

	const std::string TITLE_LOGO_FILE_PATH = "Assets/Sprite/BackGround/";	// �e�g���~�m�̃t�@�C���p�X�B
	const std::string EXTENSION_DDS = ".dds";									// �X�v���C�g�̊g���q�B
	const std::string fileName[] =												// �t�@�C���̖��O�B
	{
		std::string("CaveSky"),		// �󂪌����铴�A�B
		std::string("FantasticSky"),	// ���z�I�ȋ�B
		std::string("Hologram"),		// �z���O�����`�b�N�ȕ����B
		std::string("Lake"),			// ���̂��������΁B
		std::string("CrystalCave"),	// �������A�B
	};
	const int BACKGROUND_TYPES_NUM = sizeof(fileName) / sizeof(fileName[0]); // �w�i�̎�ސ��B

	/// <summary>
	/// �w�肳�ꂽ�t�@�C��������t���p�X������𐶐����܂��B
	/// </summary>
	/// <param name="fileName"> �t�@�C������\��������B</param>
	/// <returns> �t�@�C�����Ƀp�X�Ɗg���q��t�������t���p�X�̕�����B</returns>
	std::string GetFullPath(int fileNum)
	{
		return TITLE_LOGO_FILE_PATH + fileName[fileNum] + EXTENSION_DDS;
	}
}

BackGround::BackGround()
{
	// �����_�����̌���B
	srand(time(nullptr));

	// �w�i�̃X�v���C�g�������_���ɐݒ�B
	int randomIndex = rand() % (BACKGROUND_TYPES_NUM);
	m_backGroundSpriteRender.Init(GetFullPath(randomIndex).c_str(), FULL_HD_WIDTH, FULL_HD_HEIGHT);
}

void BackGround::Render(RenderContext& rc)
{
	m_backGroundSpriteRender.Draw(rc);
}
