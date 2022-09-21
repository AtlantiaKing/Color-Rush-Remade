#include "pch.h"
#include "Player.h"
#include "Grid.h"
#include <iostream>
#include "ColorGroup.h"
#include "Texture.h"
#include "ParticleManager.h"

Player::Player(Color color, const Point2i& position, float tileSize, InputType inputType)
	: m_ClaimColor{ color }
	, m_Position{ position.x * tileSize + tileSize / 2.0f, position.y * tileSize + tileSize / 2.0f }
	, m_MoveSpeed{ tileSize * 2.0f }
	, m_Size{ tileSize * 0.5f }
	, m_CurColor{ Color(rand() % (int(Color::brown) - int(Color::cyan) + 1) + int(Color::cyan)) }
	, m_pSpinTexture{ new Texture{ "Resources/PlayerSpin.png" } }
	, m_pGlowTexture{ new Texture{ "Resources/PlayerSpinGlow.png" } }
	, m_InputType{ inputType }
{
	m_FrameTime = (1.0f/1.5f) / m_NrFrames;
}

Player::~Player()
{
	delete m_pSpinTexture;
	delete m_pGlowTexture;
}

void Player::Update(float elapsedSec, float tileSize, Grid* pGrid, ParticleManager* pParticleManager)
{
	if (m_InputType == InputType::Arrows)
	{
		HandleInputKeys(tileSize);
	}
	else if (m_InputType == InputType::WASD)
	{
		HandleInputLetters(tileSize);
	}
	else
	{
		HandleInputBoth(tileSize);
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

void Player::SubmitColor(bool pressedSpace)
{
	if (m_Jumping)
	{
		return;
	}

	if (pressedSpace)
	{
		if (m_InputType != InputType::WASD && m_InputType != InputType::Both)
		{
			return;
		}
	}
	else
	{
		if (m_InputType != InputType::Arrows && m_InputType != InputType::Both)
		{
			return;
		}
	}

	m_Jumping = true;
	m_JumpVelocity = 50.0f;
	//m_SubmitColor = true;
}

void Player::Draw(float tileSize) const
{
	const float radius{ m_pSpinTexture->GetHeight() / 2.0f };
	const Point2f bottomLeft{ m_Position.x - radius, m_Position.y - radius };

	const Rectf srcRect
	{
		radius * 2.0f * m_CurFrame,
		radius * 2.0f,
		radius * 2.0f,
		radius * 2.0f
	};

	const float scale{ m_JumpHeight / 10.0f + 1.0f / (radius * 1.25f / m_Size)};

	glPushMatrix();
	{
		glTranslatef(m_Position.x, m_Position.y, 0.0f);
		
		switch (m_Direction)
		{
		case Player::Direction::Left:
			glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
			break;
		case Player::Direction::Right:
			glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
			break;
		case Player::Direction::Down:
			glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
			break;
		}

		/*if (m_IsGlowing)
		{
			glPushMatrix();
			{
				glScalef(scale * 2.0f, scale * 2.0f, 1.0f);
				const Rectf glowDstRect
				{
					-tileSize / 2.0f,
					-tileSize / 2.0f,
					tileSize,
					tileSize
				};

				m_pGlowTexture->Draw(glowDstRect);
			}
			glPopMatrix();
		}*/

		glScalef(scale, scale, 1.0f);

		glTranslatef(-radius, -radius, 0.0f);

		m_pSpinTexture->Draw(Point2f{ 0.0f, 0.0f }, srcRect, true, m_CurColor);
		if (m_IsGlowing)
		{
			m_pGlowTexture->Draw(Point2f{ 0.0f, 0.0f }, srcRect, false);
		}
	}
	glPopMatrix();

	/*const float radius{ m_Size / 2.0f };

	utils::SetColor(m_CurColor);
	utils::FillEllipse(m_Position, radius, radius);

	utils::SetColor(Color4f{ 0.0f, 0.0f, 0.0f, 1.0f });
	utils::DrawEllipse(m_Position, radius, radius);*/
}

bool Player::CanPlace(Grid* pGrid) const
{
	return pGrid->CanPlace(m_CurColor, m_pPreviousGroup);
}

//void Player::Move(int x, int y, Grid* pGrid)
//{
//	m_Position.x += x;
//	m_Position.y += y;
//
//	if (!pGrid->IsValidPosition(m_Position))
//	{
//		m_Position.x -= x;
//		m_Position.y -= y;
//	}
//}

void Player::HandleInputKeys(float tileSize)
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_RIGHT])
	{
		if (abs(m_Velocity.y) < FLT_EPSILON)
		{
			m_Velocity.x = m_MoveSpeed;
		}
	}
	else if (pStates[SDL_SCANCODE_LEFT])
	{
		if (abs(m_Velocity.y) < FLT_EPSILON)
		{
			m_Velocity.x = -m_MoveSpeed;
		}
	}
	else
	{
		const float distanceToTile{ abs(m_Position.x - tileSize / 2.0f - floor(m_Position.x / tileSize) * tileSize) };
		if (distanceToTile < 5.0f)
		{
			m_Velocity.x = 0.0f;
			m_Position.x = floor(m_Position.x / tileSize) * tileSize + tileSize / 2.0f;
		}
	}

	if (pStates[SDL_SCANCODE_UP])
	{
		if (abs(m_Velocity.x) < FLT_EPSILON)
		{
			m_Velocity.y = m_MoveSpeed;
		}
	}
	else if (pStates[SDL_SCANCODE_DOWN])
	{
		if (abs(m_Velocity.x) < FLT_EPSILON)
		{
			m_Velocity.y = -m_MoveSpeed;
		}
	}
	else
	{
		const float distanceToTile{ abs(m_Position.y - tileSize / 2.0f - floor(m_Position.y / tileSize) * tileSize) };
		if (distanceToTile < 5.0f)
		{
			m_Velocity.y = 0.0f;
			m_Position.y = floor(m_Position.y / tileSize) * tileSize + tileSize / 2.0f;
		}
	}
}

void Player::HandleInputLetters(float tileSize)
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_D])
	{
		if (abs(m_Velocity.y) < FLT_EPSILON)
		{
			m_Velocity.x = m_MoveSpeed;
		}
	}
	else if (pStates[SDL_SCANCODE_A] || pStates[SDL_SCANCODE_Q])
	{
		if (abs(m_Velocity.y) < FLT_EPSILON)
		{
			m_Velocity.x = -m_MoveSpeed;
		}
	}
	else
	{
		const float distanceToTile{ abs(m_Position.x - tileSize / 2.0f - floor(m_Position.x / tileSize) * tileSize) };
		if (distanceToTile < 5.0f)
		{
			m_Velocity.x = 0.0f;
			m_Position.x = floor(m_Position.x / tileSize) * tileSize + tileSize / 2.0f;
		}
	}

	if (pStates[SDL_SCANCODE_W] || pStates[SDL_SCANCODE_Z])
	{
		if (abs(m_Velocity.x) < FLT_EPSILON)
		{
			m_Velocity.y = m_MoveSpeed;
		}
	}
	else if (pStates[SDL_SCANCODE_S])
	{
		if (abs(m_Velocity.x) < FLT_EPSILON)
		{
			m_Velocity.y = -m_MoveSpeed;
		}
	}
	else
	{
		const float distanceToTile{ abs(m_Position.y - tileSize / 2.0f - floor(m_Position.y / tileSize) * tileSize) };
		if (distanceToTile < 5.0f)
		{
			m_Velocity.y = 0.0f;
			m_Position.y = floor(m_Position.y / tileSize) * tileSize + tileSize / 2.0f;
		}
	}
}

void Player::HandleInputBoth(float tileSize)
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_D])
	{
		if (abs(m_Velocity.y) < FLT_EPSILON)
		{
			m_Velocity.x = m_MoveSpeed;
		}
	}
	else if (pStates[SDL_SCANCODE_A] || pStates[SDL_SCANCODE_Q])
	{
		if (abs(m_Velocity.y) < FLT_EPSILON)
		{
			m_Velocity.x = -m_MoveSpeed;
		}
	}
	else if (pStates[SDL_SCANCODE_RIGHT])
	{
		if (abs(m_Velocity.y) < FLT_EPSILON)
		{
			m_Velocity.x = m_MoveSpeed;
		}
	}
	else if (pStates[SDL_SCANCODE_LEFT])
	{
		if (abs(m_Velocity.y) < FLT_EPSILON)
		{
			m_Velocity.x = -m_MoveSpeed;
		}
	}
	else
	{
		const float distanceToTile{ abs(m_Position.x - tileSize / 2.0f - floor(m_Position.x / tileSize) * tileSize) };
		if (distanceToTile < 5.0f)
		{
			m_Velocity.x = 0.0f;
			m_Position.x = floor(m_Position.x / tileSize) * tileSize + tileSize / 2.0f;
		}
	}

	if (pStates[SDL_SCANCODE_W] || pStates[SDL_SCANCODE_Z])
	{
		if (abs(m_Velocity.x) < FLT_EPSILON)
		{
			m_Velocity.y = m_MoveSpeed;
		}
	}
	else if (pStates[SDL_SCANCODE_S])
	{
		if (abs(m_Velocity.x) < FLT_EPSILON)
		{
			m_Velocity.y = -m_MoveSpeed;
		}
	}
	else if (pStates[SDL_SCANCODE_UP])
	{
		if (abs(m_Velocity.x) < FLT_EPSILON)
		{
			m_Velocity.y = m_MoveSpeed;
		}
	}
	else if (pStates[SDL_SCANCODE_DOWN])
	{
		if (abs(m_Velocity.x) < FLT_EPSILON)
		{
			m_Velocity.y = -m_MoveSpeed;
		}
	}
	else
	{
		const float distanceToTile{ abs(m_Position.y - tileSize / 2.0f - floor(m_Position.y / tileSize) * tileSize) };
		if (distanceToTile < 5.0f)
		{
			m_Velocity.y = 0.0f;
			m_Position.y = floor(m_Position.y / tileSize) * tileSize + tileSize / 2.0f;
		}
	}
}

void Player::Jump(float elapsedSec, float tileSize, Grid* pGrid, ParticleManager* pParticleManager)
{
	if (m_Jumping)
	{
		const float gravity{ 120.0f };

		m_JumpHeight += m_JumpVelocity * elapsedSec;
		m_JumpVelocity -= gravity * elapsedSec;
		if (m_JumpHeight <= 0.0f)
		{
			m_JumpHeight = 0.0f;
			m_Jumping = false;
			const Point2i curPosition{ int(floor(m_Position.x / tileSize)), int(floor(m_Position.y / tileSize)) };
			pParticleManager->CreateParticle(ParticleManager::ParticleType::Explosion, Point2f{ curPosition.x * tileSize, curPosition.y * tileSize }, m_ClaimColor);
			if (m_pPreviousGroup == nullptr || !m_pPreviousGroup->Contains(curPosition))
			{
				const std::pair<bool, ColorGroup*> submitResults{ pGrid->TryChangeColor(curPosition, m_CurColor) };
				if (submitResults.first)
				{
					if (m_IsGlowing)
					{
						m_CurColor = m_ClaimColor;
						m_IsGlowing = false;
					}
					else
					{
						if (rand() % 6 == 0)
						{
							m_IsGlowing = true;
						}
						m_CurColor = Color(rand() % (int(Color::brown) - int(Color::cyan) + 1) + int(Color::cyan));
					}
					m_pPreviousGroup = submitResults.second;
				}
			}
		}
	}
}

void Player::UpdateSprite(float elapsedSec)
{
	if (abs(m_Velocity.x) > FLT_EPSILON || abs(m_Velocity.y) > FLT_EPSILON)
	{
		m_AccuSec += elapsedSec;
		if (m_AccuSec > m_FrameTime)
		{
			m_AccuSec -= m_FrameTime;
			++m_CurFrame %= m_NrFrames;
		}

		if (m_Velocity.x > 0.0f)
		{
			m_Direction = Direction::Right;
		}
		else if (m_Velocity.x < 0.0f)
		{
			m_Direction = Direction::Left;
		}
		if (m_Velocity.y > 0.0f)
		{
			m_Direction = Direction::Up;
		}
		if (m_Velocity.y < 0.0f)
		{
			m_Direction = Direction::Down;
		}
	}
	else
	{
		m_CurFrame = 0;
		m_AccuSec = 0.0f;
	}
}
