#pragma once
class ScoreManager;

class ScoreView :public IGameObject
{
private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

	void ViewCurrentTotalScore();
	void ViewCurrentLevel();
	void ViewLineClearTotalCount();

	SpriteRender m_backSpriteRender;	// 黒幕背景用のスプライトレンダー。
	FontRender m_viewScore;				// スコア表示用のフォントレンダー。
	FontRender m_viewLevel;				// レベル表示用のフォントレンダー。
	FontRender m_viewLine;				// ライン表示用のフォントレンダー。

	ScoreManager* m_scoreManager = nullptr;
};

