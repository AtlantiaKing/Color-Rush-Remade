#pragma once
#include "Colors.h"
#include "structs.h"
#include "Vector2f.h"
#include "InputType.h"

class Grid;
class ColorGroup;
class Texture;
class ParticleManager;

class Player
{
public:
	Player(Color color, const Point2i& position, float tileSize, InputType inputType);
	virtual ~Player();

	virtual void Update(float elapsedSec, float tileSize, Grid* pGrid, ParticleManager* pParticleManager);
	void SubmitColor(bool pressedSpace);

	void Draw(float tileSize) const;
	bool CanPlace(Grid* pGrid) const;
protected:
	enum class Direction
	{
		Left,
		Up,
		Right,
		Down
	};

	const float m_MoveSpeed;
	Color m_ClaimColor;
	Color m_CurColor;
	const float m_Size;
	Point2f m_Position;
	Vector2f m_Velocity{};
	ColorGroup* m_pPreviousGroup{ nullptr };
	bool m_Jumping{};
	float m_JumpVelocity{};

	void Jump(float elapsedSec, float tileSize, Grid* pGrid, ParticleManager* pParticleManager);
	void UpdateSprite(float elapsedSec);
private:
	const InputType m_InputType;

	Direction m_Direction{ Direction::Up };
	bool m_SubmitColor{};
	float m_JumpHeight{};

	Texture* m_pSpinTexture;
	Texture* m_pGlowTexture;
	const int m_NrFrames{ 10 };
	float m_FrameTime;
	int m_CurFrame{ 0 };
	float m_AccuSec{ 0.0f };
	bool m_IsGlowing{};

	void HandleInputKeys(float tileSize);
	void HandleInputLetters(float tileSize);
	void HandleInputBoth(float tileSize);
};

