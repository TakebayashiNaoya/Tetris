#pragma once

#include "Level3DRender/LevelRender.h"

class FieldManager;
class Tetrimino;

class Game : public IGameObject
{
public:
	Game() {}
	~Game() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	FieldManager* m_fieldManager;
	Tetrimino* m_tetrimino;
	ModelRender m_modelRender;
	Vector3 m_pos;
};

