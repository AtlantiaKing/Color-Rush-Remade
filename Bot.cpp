#include "Bot.h"
#include "Grid.h"
#include "ColorGroup.h"

Bot::Bot(Color color, const Point2i& position, float tileSize)
	: Player(color, position, tileSize, InputType::None)
	, m_PrevColor{ color }
{
}

void Bot::Update(float elapsedSec, float tileSize, Grid* pGrid, ParticleManager* pParticleManager)
{
	if (!pGrid->Contains(m_pPreviousGroup))
	{
		m_pPreviousGroup = nullptr;
	}

	int radius{ 1 };
	const int gridSize{ pGrid->GetSize() };
	const int maxAngle{ gridSize * 2 };
	const Point2i curPosition{ int(floor(m_Position.x / tileSize)), int(floor(m_Position.y / tileSize)) };

	const Color curColorOnTarget{ pGrid->GetColorAt(m_Target) };
	if (curColorOnTarget == m_CurColor || curColorOnTarget == Color::red || curColorOnTarget == Color::blue || !pGrid->IsValidPosition(m_Target) || (m_pPreviousGroup != nullptr && m_pPreviousGroup->Contains(m_Target)))
	{
		m_HasTarget = false;
	}

	while (!m_HasTarget)
	{
		if (m_CurColor == m_ClaimColor)
		{
			m_HasTarget = true;
			m_Target = pGrid->GetLargestTarget(m_pPreviousGroup)->GetTileAtIndex(0);
		}
		else
		{
			ColorGroup* largestGroup{};

			for (int extraX{ -radius }; extraX <= radius; ++extraX)
			{
				for (int extraY{ -radius }; extraY <= radius; ++extraY)
				{
					const int x = curPosition.x + extraX;
					const int y = curPosition.y + extraY;

					const Point2i curCirclePos{ x, y };

					if (m_pPreviousGroup != nullptr && m_pPreviousGroup->Contains(curCirclePos))
					{
						continue;
					}

					ColorGroup* curGroup{ pGrid->GetGroupAt(curCirclePos) };
					if (curGroup == nullptr)
					{
						continue;
					}

					const Color circleColor{ curGroup->GetColor() };
					if (circleColor == m_CurColor || circleColor == Color::red || circleColor == Color::blue)
					{
						continue;
					}

					if (largestGroup == nullptr || curGroup->GetSize() > largestGroup->GetSize())
					{
						largestGroup = curGroup;
						m_Target = curCirclePos;
						m_HasTarget = true;
					}
				}
			}

			if (m_HasTarget)
			{
				return;
			}

			++radius;
			if (radius > gridSize)
			{
				break;
			}
		}
	}

	if (m_HasTarget)
	{
		float distanceToTile{  };
		if (abs(m_Velocity.x) > FLT_EPSILON)
		{
			distanceToTile = abs(m_Position.x - tileSize / 2.0f - floor(m_Position.x / tileSize) * tileSize);
		}
		else if (abs(m_Velocity.y) > FLT_EPSILON)
		{
			distanceToTile = abs(m_Position.y - tileSize / 2.0f - floor(m_Position.y / tileSize) * tileSize);
		}

		if (distanceToTile < 1.0f)
		{
			const Point2f direction{ (m_Target.x * tileSize + tileSize / 2.0f) - m_Position.x, (m_Target.y * tileSize + tileSize / 2.0f) - m_Position.y };

			if (abs(direction.x) > 5.0f || abs(direction.y) > 5.0f)
			{
				if (abs(direction.x) > abs(direction.y))
				{
					if (direction.x > 0.0f)
					{
						if (pGrid->IsValidPosition(Point2i{ curPosition.x + 1, curPosition.y }))
						{
							m_Velocity.x = m_MoveSpeed;
							m_Velocity.y = 0.0f;
						}
						else
						{
							if (direction.y > 0.0f && pGrid->IsValidPosition(Point2i{ curPosition.x, curPosition.y + 1 }))
							{
								m_Velocity.y = m_MoveSpeed;
								m_Velocity.x = 0.0f;
							}
							else
							{
								m_Velocity.y = -m_MoveSpeed;
								m_Velocity.x = 0.0f;
							}
						}
					}
					else
					{
						if (pGrid->IsValidPosition(Point2i{ curPosition.x - 1, curPosition.y }))
						{
							m_Velocity.x = -m_MoveSpeed;
							m_Velocity.y = 0.0f;
						}
						else
						{
							if (direction.y > 0.0f && pGrid->IsValidPosition(Point2i{ curPosition.x, curPosition.y + 1 }))
							{
								m_Velocity.y = m_MoveSpeed;
								m_Velocity.x = 0.0f;
							}
							else
							{
								m_Velocity.y = -m_MoveSpeed;
								m_Velocity.x = 0.0f;
							}
						}
					}
				}
				else
				{
					if (direction.y > 0.0f)
					{
						if (pGrid->IsValidPosition(Point2i{ curPosition.x, curPosition.y + 1 }))
						{
							m_Velocity.y = m_MoveSpeed;
							m_Velocity.x = 0.0f;
						}
						else
						{
							if (direction.x > 0.0f && pGrid->IsValidPosition(Point2i{ curPosition.x + 1, curPosition.y }))
							{
								m_Velocity.x = m_MoveSpeed;
								m_Velocity.y = 0.0f;
							}
							else
							{
								m_Velocity.x = -m_MoveSpeed;
								m_Velocity.y = 0.0f;
							}
						}
					}
					else
					{
						if (pGrid->IsValidPosition(Point2i{ curPosition.x, curPosition.y - 1 }))
						{
							m_Velocity.y = -m_MoveSpeed;
							m_Velocity.x = 0.0f;
						}
						else
						{
							if (direction.x > 0.0f && pGrid->IsValidPosition(Point2i{ curPosition.x + 1, curPosition.y }))
							{
								m_Velocity.x = m_MoveSpeed;
								m_Velocity.y = 0.0f;
							}
							else
							{
								m_Velocity.x = -m_MoveSpeed;
								m_Velocity.y = 0.0f;
							}
						}
					}
				}
			}
			else
			{
				m_Velocity.x = 0.0f;
				m_Velocity.y = 0.0f;
				const Color curColorOnPos{ pGrid->GetColorAt(curPosition) };

				if (curColorOnPos == m_CurColor || curColorOnPos == Color::red || curColorOnPos == Color::blue || !pGrid->IsValidPosition(curPosition) || (m_pPreviousGroup != nullptr && m_pPreviousGroup->Contains(curPosition)))
				{
					m_HasTarget = false;
				}
				else
				{
					if (!m_Jumping)
					{
						m_Jumping = true;
						m_JumpVelocity = 50.0f;
					}
				}
			}
		}

		if (m_PrevColor != m_CurColor)
		{
			m_HasTarget = false;
			m_PrevColor = m_CurColor;
		}
	}

	m_Position.x += m_Velocity.x * elapsedSec;
	m_Position.y += m_Velocity.y * elapsedSec;

	pGrid->HandleCollision(m_Velocity, m_Position, m_Size);

	/*if (m_SubmitColor)
	{
		const float distanceToTileX{ abs(m_Position.x - tileSize / 2.0f - floor(m_Position.x / tileSize) * tileSize) };
		const float distanceToTileY{ abs(m_Position.y - tileSize / 2.0f - floor(m_Position.y / tileSize) * tileSize) };
		if (distanceToTileX < 5.0f && distanceToTileY < 5.0f)
		{
			const Point2i curPosition{ int(floor(m_Position.x / tileSize)), int(floor(m_Position.y / tileSize)) };

			if (m_pPreviousGroup == nullptr || !m_pPreviousGroup->Contains(curPosition))
			{
				m_SubmitColor = false;
				m_Jumping = true;
				m_JumpVelocity = 50.0f;
			}

			m_SubmitColor = false;
		}
	}*/

	Jump(elapsedSec, tileSize, pGrid, pParticleManager);

	UpdateSprite(elapsedSec);
}
