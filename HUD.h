#pragma once
#include "InputType.h"

class Texture;

class HUD final
{
public:
	HUD(const std::pair<InputType, InputType>& inputData);
	~HUD();

	void UpdateData(int blueTiles, int redTiles, int bluePercentage, int redPercentage);

	void Draw(float windowWidth, float windowHeight) const;
private:
	Texture* m_pNrRedTexture;
	Texture* m_pNrBlueTexture;

	Texture* m_pArrowsTexture;
	Texture* m_pWASDTexture;

	std::pair<InputType, InputType> m_InputData;
};

