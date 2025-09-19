#pragma once

#include "Level3DRender/LevelRender.h"

class BackGround;
class FieldManager;
class ScoreManager;
class ScoreView;
class ResultView;

class Game : public IGameObject
{
public:
	Game() {}
	~Game();
	bool Start();
	void Update();

	/// <summary>
	/// ゲームオーバー状態を処理します。
	/// </summary>
	void OnGameOver();

private:
	BackGround* m_backGround;
	FieldManager* m_fieldManager;
	ScoreManager* m_scoreManager;
	ScoreView* m_scoreView;
	ResultView* m_resultView;

	ModelRender m_modelRender;
	Vector3 m_pos;
};

