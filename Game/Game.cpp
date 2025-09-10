#include "stdafx.h"
#include "Game.h"
#include "Tetrimino.h"
#include "FieldManager.h"

bool Game::Start()
{

	m_fieldManager = NewGO<FieldManager>(0, "FieldManager");
	m_tetrimino = NewGO<Tetrimino>(0, "Tetrimino");
	return true;
}

void Game::Update()
{

}

void Game::Render(RenderContext& rc)
{

}