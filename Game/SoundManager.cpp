#include "stdafx.h"
#include "SoundManager.h"

namespace
{
	std::string SOUND_FILE_PATH = "Assets/Sound/";	// サウンドのファイルパス。
	std::string EXTENSION_WAV = ".wav";				// サウンドの拡張子。

	struct SoundOption
	{
		const char* fileName;	// 再生したいファイルの名前。
		bool repeat = false;	// リピートするかどうか。（true=する、false=しない）
		float volume = 0.0f;	// 音量。

		// サウンドのファイルパスと各サウンドのファイル名を結合して返す。
		std::string GetFullPath() const
		{
			return SOUND_FILE_PATH + fileName + EXTENSION_WAV;
		}
	};

	const SoundOption SOUND_OPTIONS[] =
	{
		{ "Title",			true,	1.0f },
		{ "InGame",			true,	1.0f },
		{ "Select",			false,	3.5f },
		{ "Move",			false,	4.0f },
		{ "Spin",			false,	3.5f },
		{ "SaveMino",		false,	6.0f },
		{ "Single",			false,	3.0f },
		{ "Double",			false,	6.0f },
		{ "Triple",			false,	6.0f },
		{ "Tetris",			false,	6.0f },
		{ "LevelUp",		false,	3.5f },
		{ "Pause",			false,	3.5f },
		{ "Result",			false,	3.5f },
	};
}

void SoundManager::SoundNewGO(int a)
{
	sound[a] = NewGO<SoundSource>(0);
	g_soundEngine->ResistWaveFileBank(a, SOUND_OPTIONS[a].GetFullPath().c_str());	//c_strでstringからconst char*に変換。
	sound[a]->Init(a);
	sound[a]->Play(SOUND_OPTIONS[a].repeat);
	sound[a]->SetVolume(SOUND_OPTIONS[a].volume);
}

void SoundManager::SoundDeleteGO(int b)
{
	/// <summary>
	/// main.cppでNewGOされているこのクラスが消された後に、他クラスでこの関数が呼び出されるため、
	/// このクラスが存在しない場合のみこの関数の処理を有効にする。
	/// </summary>
	if (this != nullptr)
	{
		DeleteGO(sound[b]);
	}
}