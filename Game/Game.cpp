#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"
#include "Tetrimino.h"
#include "FieldManager.h"
#include "ScoreManager.h"
#include "ScoreView.h"
#include "ResultView.h"

Game::~Game()
{
	DeleteGO(m_backGround);
	DeleteGO(m_scoreManager);
}

bool Game::Start()
{
	m_backGround = NewGO<BackGround>(0, "BackGround");
	m_fieldManager = NewGO<FieldManager>(0, "FieldManager");
	m_scoreManager = NewGO<ScoreManager>(0, "ScoreManager");
	m_scoreView = NewGO<ScoreView>(0, "ScoreView");
	return true;
}

void Game::Update()
{

}

void Game::OnGameOver()
{
	DeleteGO(m_fieldManager);
	DeleteGO(m_scoreView);
	m_resultView = NewGO<ResultView>(0, "ResultView");
}
