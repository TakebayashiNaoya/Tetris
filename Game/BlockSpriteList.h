#pragma once
#include <array>

namespace
{
	constexpr float BLOCK_SIZE = 40.0f;	// 1ブロックのサイズ。
}

/// <summary>
/// ブロックスプライトの種類を表す列挙型。
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



