#pragma once
#include "Colors.h"

class Texture;

class Particle final
{
public:
	Particle(Texture* pTexture, const Point2f& position, float frameTime, int nrFrames, Color color);
	
	void Update(float elapsedSec);
	void Draw(float scale) const;
	bool IsDone() const;
	float GetSize() const;
private:
	const Point2f m_Position;
	const float m_FrameTime;
	const int m_NrFrames;
	const Color m_Color;

	float m_AccuSec{};
	int m_CurFrame{};
	bool m_IsDone{};

	Texture* m_pTexture;
};

