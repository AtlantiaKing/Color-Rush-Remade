#pragma once
#include "Player.h"
class Bot :  public Player
{
public:
	Bot(Color color, const Point2i& position, float tileSize);

	virtual void Update(float elapsedSec, float tileSize, Grid* pGrid, ParticleManager* pParticleManager) override;
private:
	bool m_HasTarget{};
	Point2i m_Target{};
	Color m_PrevColor;
};

