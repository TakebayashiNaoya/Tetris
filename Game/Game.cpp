#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"
#include "Tetrimino.h"
#include "FieldManager.h"
#include "ScoreManager.h"
#include "ScoreView.h"
#include "NextTetriminoView.h"
#include "ResultView.h"
#include "PauseView.h"
#include "SoundManager.h"
#include "HoldView.h"

Game::~Game()
{
	DeleteGO(m_backGround);
	DeleteGO(m_scoreManager);
	SoundManager* soundManager = FindGO<SoundManager>("SoundManager");
	soundManager->SoundDeleteGO(enSoundList_InGameBGM);
}

bool Game::Start()
{
	m_scoreManager = NewGO<ScoreManager>(0, "ScoreManager");
	m_backGround = NewGO<BackGround>(0, "BackGround");
	m_scoreView = NewGO<ScoreView>(0, "ScoreView");
	m_fieldManager = NewGO<FieldManager>(0, "FieldManager");
	m_nextTetriminoView = NewGO<NextTetriminoView>(0, "NextTetriminoView");
	m_pauseView = NewGO<PauseView>(0, "PauseManager");
	m_holdView = NewGO<HoldView>(0, "HoldView");

	SoundManager* soundManager = FindGO<SoundManager>("SoundManager");
	soundManager->SoundNewGO(enSoundList_InGameBGM);

	return true;
}

void Game::Update()
{

}

/// <summary>
/// ゲームオーバー時に関連するゲームオブジェクトを削除し、結果表示ビューを生成します。
/// </summary>
void Game::OnGameOver()
{
	DeleteGO(m_fieldManager);
	DeleteGO(m_scoreView);
	DeleteGO(m_nextTetriminoView);
	DeleteGO(m_pauseView);
	DeleteGO(m_holdView);
	m_resultView = NewGO<ResultView>(0, "ResultView");
}
