#pragma once
#include <cstdint>

namespace
{
	constexpr float BLOCK_SIZE = 40.0f;	// 1ブロックのサイズ。
}

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