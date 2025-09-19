#pragma once
class ScoreManager;

class ScoreView :public IGameObject
{
private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

	FontRender m_viewScore;	// スコア表示用のフォントレンダー。

	ScoreManager* m_scoreManager = nullptr;
};

