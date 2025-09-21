#pragma once
#include <array>
#include "Types.h"

class BlockSpriteList :public IGameObject
{
public:
	bool Start() override final;


	/// <summary>
	/// 指定された種類のブロックスプライトレンダーのアドレスを取得します。
	/// </summary>
	/// <param name="kind">取得したいブロックスプライトレンダーの種類を示す整数値。</param>
	/// <returns>指定された種類のブロックスプライトレンダーへのポインタ。</returns>
	SpriteRender* GetBlockSpriteRenderAddress(int kind)
	{
		return &blockSpriteRender[kind];
	}

private:
	// ブロックのスプライトレンダーの配列。
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



