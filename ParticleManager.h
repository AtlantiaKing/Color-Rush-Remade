#pragma once
#include <unordered_map>
#include "Texture.h"

class Particle;

class ParticleManager final
{
public:
	enum class ParticleType
	{
		Explosion
	};

	ParticleManager();
	~ParticleManager();

	void CreateParticle(ParticleType type, const Point2f& position, Color color);
	void Update(float elapsedSec);

	void Draw(float tileSize) const;
private:
	std::vector<Particle*> m_pParticles;
	std::unordered_map<ParticleType, Texture*> m_pTextures;
};

