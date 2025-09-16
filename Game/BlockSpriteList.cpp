#include "stdafx.h"
#include "BlockSpriteList.h"

namespace
{
	const std::string TETRIMINO_FILE_PATH = "Assets/Tetrimino/Mino_";		// テトリミノのファイルパス。
	const std::string EXTENSION_DDS = ".dds";								// スプライトの拡張子。
	const char fileName[enMinoKinds_Num] = { 'I','J','L','O','S','T','Z' };	// ファイルの名前。

	/// <summary>
	/// 指定されたファイル名からフルパス文字列を生成します。
	/// </summary>
	/// <param name="fileName"> ファイル名を表す文字列。</param>
	/// <returns> ファイル名にパスと拡張子を付加したフルパスの文字列。</returns>
	std::string GetFullPath(char fileName)
	{
		return TETRIMINO_FILE_PATH + fileName + EXTENSION_DDS;
	}
}

bool BlockSpriteList::Start()
{
	// blockSpriteRenderにブロックのスプライトをInitする。
	for (int i = 0; i < enMinoKinds_Num; i++) {
		blockSpriteRender[i].Init(GetFullPath(fileName[i]).c_str(), BLOCK_SIZE, BLOCK_SIZE);
	}
	return true;
}

/// <summary>
/// 指定されたインデックスに基づいて新しい SpriteRender オブジェクトを作成します。
/// </summary>
/// <param name="index">ファイル名リストから使用するインデックス。</param>
/// <returns>初期化された SpriteRender オブジェクトへのポインタ。</returns>
SpriteRender* BlockCreateFactory::Create(const int index)
{
	auto* render = new SpriteRender();
	render->Init(GetFullPath(fileName[index]).c_str(), BLOCK_SIZE, BLOCK_SIZE);
	return render;
}
