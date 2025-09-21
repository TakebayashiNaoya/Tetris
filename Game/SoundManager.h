/// <summary>
/// サウンドを管理するクラス。
/// </summary>
#pragma once
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

enum EnSoundList
{
	enSoundList_TitleBGM,		// タイトルで再生するBGM。
	enSoundList_InGameBGM,		// インゲームで再生するBGM。
	enSoundList_SelectSE,		// PressAでAを押したときに再生するSE。
	enSoundList_MoveSE,			// テトリミノを動かしたときに再生するSE。
	enSoundList_SpinSE,			// テトリミノを回転させたときに再生するSE。
	enSoundList_SaveMinoSE,		// テトリミノをホールドしたときに再生するSE。
	enSoundList_SingleSE,		// 1ライン消したときに再生するSE。
	enSoundList_DoubleSE,		// 2ライン消したときに再生するSE。
	enSoundList_TripleSE,		// 3ライン消したときに再生するSE。
	enSoundList_TetrisSE,		// 4ライン消したときに再生するSE。
	enSoundList_LevelUpSE,		// レベルアップしたときに再生するSE。
	enSoundList_PauseSE,		// ポーズしたときに再生するSE。
	enSoundList_ResultSE,		// リザルト画面に遷移したときに再生するSE。
	enSoundList_Num
};

class SoundManager :public IGameObject
{
public:
	/// <summary>
	/// 新しいゲームオブジェクトのサウンドを開始します。
	/// </summary>
	/// <param name="a">サウンドを開始するゲームオブジェクトの識別子。</param>
	void SoundNewGO(int a);

	/// <summary>
	/// 指定されたIDのサウンドオブジェクトを削除します。
	/// </summary>
	/// <param name="a">削除するサウンドオブジェクトのID。</param>
	void SoundDeleteGO(int b);

private:
	SoundSource* sound[enSoundList_Num];
};

