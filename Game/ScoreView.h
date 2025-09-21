#pragma once
class ScoreManager;

class ScoreView :public IGameObject
{
private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

	/// <summary>
	/// 現在の合計スコアを表示します。
	/// </summary>
	void ViewCurrentTotalScore();

	/// <summary>
	/// 現在のレベルを表示します。
	/// </summary>
	void ViewCurrentLevel();

	/// <summary>
	/// ラインクリアの合計回数を表示します。
	/// </summary>
	void ViewLineClearTotalCount();

	SpriteRender m_backSpriteRender;	// 黒幕背景用のスプライトレンダー。
	FontRender m_viewScore;				// スコア表示用のフォントレンダー。
	FontRender m_viewLevel;				// レベル表示用のフォントレンダー。
	FontRender m_viewLine;				// ライン表示用のフォントレンダー。

	ScoreManager* m_scoreManager = nullptr;
};

