#pragma once
class BackGround :public IGameObject
{
public:
	BackGround();
private:
	void Render(RenderContext& rc) override final;

	SpriteRender m_backGroundSpriteRender;
};

