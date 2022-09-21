#include "pch.h"
#include "Particle.h"
#include "Texture.h"

Particle::Particle(Texture* pTexture, const Point2f& position, float frameTime, int nrFrames, Color color)
	: m_pTexture{ pTexture }
	, m_NrFrames{ nrFrames }
	, m_Position{ position }
	, m_FrameTime{ frameTime }
	, m_Color{ color }
{
}

void Particle::Update(float elapsedSec)
{
	m_AccuSec += elapsedSec;
	if (m_AccuSec >= m_FrameTime)
	{
		m_AccuSec -= m_FrameTime;
		++m_CurFrame;
		if (m_CurFrame >= m_NrFrames)
		{
			m_IsDone = true;
		}
	}
}

void Particle::Draw(float scale) const
{
	const float clipSize{ m_pTexture->GetWidth() / m_NrFrames };

	const Rectf srcRect
	{
		clipSize * m_CurFrame,
		clipSize,
		clipSize,
		clipSize
	};

	const Rectf dstRect
	{
		m_Position.x,
		m_Position.y,
		clipSize * scale,
		clipSize * scale
	};

	m_pTexture->Draw(dstRect, srcRect, true, m_Color);
}

bool Particle::IsDone() const
{
	return m_IsDone;
}

float Particle::GetSize() const
{
	return m_pTexture->GetHeight();
}
