#include "pch.h"
#include "ColorGroup.h"
#include "utils.h"

ColorGroup::ColorGroup(Color color)
	: m_Color{ color }
{
}

void ColorGroup::AddTile(const Point2i& tile)
{
	m_Tiles.push_back(tile);
}

void ColorGroup::Merge(ColorGroup* other)
{
	for (const Point2i& tile : other->m_Tiles)
	{
		m_Tiles.push_back(tile);
	}
	delete other;
}

void ColorGroup::SetColor(Color color)
{
	m_Color = color;
}

void ColorGroup::RemoveTileAt(int index)
{
	if (index >= int(m_Tiles.size()))
	{
		return;
	}
	m_Tiles[index] = m_Tiles[m_Tiles.size() - 1];
	m_Tiles.pop_back();
}

void ColorGroup::RemoveTile(const Point2i& tile)
{
	for (int i{ 0 }; i < int(m_Tiles.size()); ++i)
	{
		if (m_Tiles[i].x == tile.x && m_Tiles[i].y == tile.y)
		{
			m_Tiles[i] = m_Tiles[m_Tiles.size() - 1];
			m_Tiles.pop_back();
		}
	}
}

Color ColorGroup::GetColor() const
{
	return m_Color;
}

void ColorGroup::Draw(float tileSize) const
{
	const Color4f curColor{ utils::GetColor(m_Color) };
	const float blackMultiplier{ 0.95f };
	const Color4f darkColor
	{ 
		curColor.r * blackMultiplier, 
		curColor.g * blackMultiplier, 
		curColor.b * blackMultiplier, 
		curColor.a * blackMultiplier 
	};

	for (const Point2i& tile : m_Tiles)
	{
		utils::SetColor(curColor);

		const Point2f curPos{ tile.ToFloat() * tileSize };
		utils::FillRect(curPos, tileSize, tileSize);

		utils::SetColor(darkColor);

		const float stripSize{ tileSize / 25.0f };

		if (!Contains(Point2i{ tile.x - 1, tile.y }))
		{
			utils::FillRect(curPos, stripSize, tileSize);
		}
		if (!Contains(Point2i{ tile.x + 1, tile.y }))
		{
			utils::FillRect(curPos.x + tileSize - stripSize, curPos.y, stripSize, tileSize);
		}
		if (!Contains(Point2i{ tile.x, tile.y - 1 }))
		{
			utils::FillRect(curPos.x, curPos.y, tileSize, stripSize);
		}
		if (!Contains(Point2i{ tile.x, tile.y + 1 }))
		{
			utils::FillRect(curPos.x, curPos.y + tileSize - stripSize, tileSize, stripSize);
		}
	}
}

bool ColorGroup::Contains(const Point2i& position) const
{
	for (const Point2i tile : m_Tiles)
	{
		if (tile.x == position.x && tile.y == position.y)
		{
			return true;
		}
	}
	return false;
}

const std::vector<Point2i>& ColorGroup::GetTiles() const
{
	return m_Tiles;
}

const size_t ColorGroup::GetSize() const
{
	return m_Tiles.size();
}

Point2i ColorGroup::GetTileAtIndex(int index) const
{
	return m_Tiles[index];
}
