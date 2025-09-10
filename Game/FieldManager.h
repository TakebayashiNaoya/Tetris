#pragma once
class FieldManager : public IGameObject
{
public:
	FieldManager();
private:
	bool Start()override final;
	void Render(RenderContext& rc);

	void SetupCheckFields();

	/// <summary>
	/// �X�e�[�W1�u���b�N���̏��B
	/// </summary>
	struct OneBlockOfFieldInfo
	{
		Vector2 position = Vector2::Zero;
		bool isThereBlock = false;
	};
	OneBlockOfFieldInfo checkFields[10][10];
	Vector2 stagePivotPosition = Vector2::Zero;

	SpriteRender spriteRender;
};

