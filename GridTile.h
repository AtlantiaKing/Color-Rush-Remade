#pragma once
#include "Colors.h"
#include <stdlib.h>

struct GridTile
{
	GridTile(int xPos, int yPos)
		: x{ xPos }
		, y{ yPos }
	{
		const int minColor = int(Color::cyan);
		const int maxColor = int(Color::brown);

		color = Color(rand() % (maxColor - minColor + 1) + minColor);
	};

	int x;
	int y;
	Color color;
};