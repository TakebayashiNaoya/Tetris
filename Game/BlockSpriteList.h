#pragma once
#include <array>

namespace
{
	const float BLOCK_SIZE = 40.0f;	// 1ブロックのサイズ。
}

/// <summary>
/// ブロックスプライトの種類を表す列挙型。
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
	/// 指定した種類のブロックのスプライトレンダーのアドレスを取得する。
	/// </summary>
	SpriteRender* GetBlockSpriteRenderAddress(int kind)
	{
		return &blockSpriteRender[kind];
	}

public:
	bool Start() override final;


private:
	/// <summary>
	/// ブロックのスプライトレンダーの配列。
	/// </summary>
	std::array<SpriteRender, enMinoKinds_Num> blockSpriteRender;

	//std::array<std::array<SpriteRender, MINO_PARTS_COUNT>, enMinoKinds_Num> blockSpriteRender;

};

/// <summary>
/// 指定されたインデックスに基づいて、SpriteRender オブジェクトを生成します。
/// </summary>
class BlockCreateFactory
{
public:
	static SpriteRender* Create(const int index);
};



