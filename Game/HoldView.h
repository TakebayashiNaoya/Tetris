#pragma once

class FieldManager;

class HoldView :public IGameObject
{
public:
	void UpdateHoldView();

private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& renderContext)override final;

	FieldManager* m_fieldManager = nullptr;

	SpriteRender m_holdViewSprite;
	FontRender m_holdTextFontRender;
};

