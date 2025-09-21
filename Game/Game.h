#pragma once

#include "Level3DRender/LevelRender.h"

class BackGround;
class FieldManager;
class ScoreManager;
class ScoreView;
class NextTetriminoView;
class ResultView;
class PauseView;
class HoldView;

class Game : public IGameObject
{
public:
	~Game();
	bool Start();
	void Update();

	/// <summary>
	/// ゲームオーバー時に関連するゲームオブジェクトを削除し、結果表示ビューを生成します。
	/// </summary>
	void OnGameOver();

private:
	BackGround* m_backGround;
	FieldManager* m_fieldManager;
	ScoreManager* m_scoreManager;
	ScoreView* m_scoreView;
	NextTetriminoView* m_nextTetriminoView;
	ResultView* m_resultView;
	PauseView* m_pauseView;
	HoldView* m_holdView;
};

