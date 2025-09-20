#pragma once
#include "ScoreManager.h"

class Game;
class ScoreManager;

class ResultView :public IGameObject
{
public:
	~ResultView();

private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

	/// <summary>
	/// 「RESULT」の表示。
	/// </summary>
	void ViewResultText();

	/// <summary>
	/// 各同時消しのスコア表示。
	/// </summary>
	void ViewLineClearScore();

	/// <summary>
	/// 全行クリアの合計回数を表示します。
	/// </summary>
	void ViewLineClearTotalCount();

	/// <summary>
	/// 合計スコアの表示。
	/// </summary>
	void ViewTotalScore();

	/// <summary>
	/// 「Press A to Title」の表示。
	/// </summary>
	void CreatePressAtoTitleText();

	SpriteRender m_backSpriteRender;	// 結果表示用のスプライトレンダー。

	FontRender m_viewLineClearScore[static_cast<int>(LineClearType::LineClearType_Num)];	// スコア表示用のフォントレンダー。
	FontRender m_resultFontRender;			// 結果表示用のフォントレンダー。
	FontRender m_viewLineClearTotalCount;	// 総ライン数表示用のフォントレンダー。
	FontRender m_viewTotalScore;			// 合計スコア表示用のフォントレンダー。
	FontRender m_pressAtoTitleFontRender;	// タイトルへ戻る案内用のフォントレンダー。

	Game* m_game = nullptr;					// ゲームクラス。
	ScoreManager* m_scoreManager = nullptr;	// スコアマネージャー。
};

