#include "pch.h"
#include "ParticleManager.h"
#include "Particle.h"

ParticleManager::ParticleManager()
{
	m_pTextures[ParticleType::Explosion] = new Texture{ "Resources/Explosion.png" };
}

ParticleManager::~ParticleManager()
{
	for (const std::pair<ParticleType, Texture*> pTexture : m_pTextures)
	{
		delete pTexture.second;
	}
	for (Particle* pParticle : m_pParticles)
	{
		delete pParticle;
	}
}

void ParticleManager::CreateParticle(ParticleType type, const Point2f& position, Color color)
{
	float frameTime{};
	int nrFrames{};

	switch (type)
	{
	case ParticleManager::ParticleType::Explosion:
		frameTime = 0.05f;
		nrFrames = 15;
		break;
	}

	m_pParticles.push_back(new Particle{ m_pTextures[type], position, frameTime, nrFrames, color});
}

void ParticleManager::Update(float elapsedSec)
{
	for (int i{ int(m_pParticles.size()) - 1 }; i >= 0; --i)
	{
		Particle* pParticle{ m_pParticles[i] };
		pParticle->Update(elapsedSec);

		if (pParticle->IsDone())
		{
			delete pParticle;
			m_pParticles[i] = m_pParticles[m_pParticles.size() - 1];
			m_pParticles.pop_back();
		}
	}
}

void ParticleManager::Draw(float tileSize) const
{
	for (Particle* pParticle : m_pParticles)
	{
		const float scale{ tileSize / pParticle->GetSize() };

		pParticle->Draw(scale);
	}
}
