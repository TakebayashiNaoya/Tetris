#include "stdafx.h"
#include "BackGround.h"
#include <time.h>

namespace
{
	constexpr float FULL_HD_WIDTH = 1920.0f;	// フルHDの横幅。
	constexpr float FULL_HD_HEIGHT = 1080.0f;	// フルHDの縦幅。

	const std::string TITLE_LOGO_FILE_PATH = "Assets/Sprite/BackGround/";	// テトリミノのファイルパス。
	const std::string EXTENSION_DDS = ".dds";									// スプライトの拡張子。
	const std::string fileName[] =												// ファイルの名前。
	{
		std::string("CaveSky"),		// 空が見える洞窟。
		std::string("FantasticSky"),	// 幻想的な空。
		std::string("Hologram"),		// ホログラムチックな部屋。
		std::string("Lake"),			// 霧のかかった湖。
		std::string("CrystalCave"),	// 水晶洞窟。
	};
	const int BACKGROUND_TYPES_NUM = sizeof(fileName) / sizeof(fileName[0]); // 背景の種類数。

	/// <summary>
	/// 指定されたファイル名からフルパス文字列を生成します。
	/// </summary>
	/// <param name="fileName"> ファイル名を表す文字列。</param>
	/// <returns> ファイル名にパスと拡張子を付加したフルパスの文字列。</returns>
	std::string GetFullPath(int fileNum)
	{
		return TITLE_LOGO_FILE_PATH + fileName[fileNum] + EXTENSION_DDS;
	}
}

BackGround::BackGround()
{
	// ランダム性の向上。
	srand(time(nullptr));

	// 背景のスプライトをランダムに設定。
	int randomIndex = rand() % (BACKGROUND_TYPES_NUM);
	m_backGroundSpriteRender.Init(GetFullPath(randomIndex).c_str(), FULL_HD_WIDTH, FULL_HD_HEIGHT);
}

void BackGround::Render(RenderContext& rc)
{
	m_backGroundSpriteRender.Draw(rc);
}
