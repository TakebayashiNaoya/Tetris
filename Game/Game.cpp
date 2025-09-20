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

Game::~Game()
{
	DeleteGO(m_backGround);
	DeleteGO(m_scoreManager);
}

bool Game::Start()
{
	m_scoreManager = NewGO<ScoreManager>(0, "ScoreManager");
	m_backGround = NewGO<BackGround>(0, "BackGround");
	m_scoreView = NewGO<ScoreView>(0, "ScoreView");
	m_fieldManager = NewGO<FieldManager>(0, "FieldManager");
	m_nextTetriminoView = NewGO<NextTetriminoView>(0, "NextTetriminoView");
	m_pauseView = NewGO<PauseView>(0, "PauseManager");
	return true;
}

void Game::Update()
{

}

void Game::OnGameOver()
{
	DeleteGO(m_fieldManager);
	DeleteGO(m_scoreView);
	DeleteGO(m_nextTetriminoView);
	DeleteGO(m_pauseView);
	m_resultView = NewGO<ResultView>(0, "ResultView");
}
