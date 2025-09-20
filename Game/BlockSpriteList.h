#pragma once
#include <array>
#include "Types.h"

class BlockSpriteList :public IGameObject
{
public:
	bool Start() override final;

	/// <summary> 
	/// 指定した種類のブロックのスプライトレンダーのアドレスを取得する。
	/// </summary>
	SpriteRender* GetBlockSpriteRenderAddress(int kind)
	{
		return &blockSpriteRender[kind];
	}

private:
	/// <summary>
	/// ブロックのスプライトレンダーの配列。
	/// </summary>
	std::array<SpriteRender, static_cast<int>(EnMinoKinds::enMinoKinds_Num)> blockSpriteRender;
};

/// <summary>
/// 指定されたインデックスに基づいて、SpriteRender オブジェクトを生成します。
/// </summary>
class BlockCreateFactory
{
public:
	static SpriteRender* Create(const int index);
};



