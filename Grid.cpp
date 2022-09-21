#include "pch.h"
#include "Grid.h"
#include "ColorGroup.h"
#include "Colors.h"
#include <iostream>

Grid::Grid(int gridSize, float tileSize)
	: m_GridSize{ gridSize }
	, m_TileSize{ tileSize }
{
	const int nrTiles{ gridSize * gridSize };

	Color* pGrid = new Color[nrTiles];

	const int minColor = int(Color::cyan);
	const int maxColor = int(Color::brown);

	for (int i{ 0 }; i < nrTiles; ++i)
	{
		pGrid[i] = Color(rand() % (maxColor - minColor + 1) + minColor);
	}

	std::unordered_set<int> handledTiles{};

	ColorGroup* curGroup{ nullptr };
	for (int i{ 0 }; i < nrTiles; ++i)
	{
		if (handledTiles.contains(i))
		{
			continue;
		}
		curGroup = new ColorGroup(pGrid[i]);
		const int x{ i / gridSize };
		const int y{ i - x * gridSize };
		curGroup->AddTile(Point2i{ x,y });

		handledTiles.insert(i);
		ExtendGroup(x, y, gridSize, pGrid, handledTiles, curGroup);
		m_pColorGroups.push_back(curGroup);
	}

	delete[] pGrid;
}

Grid::~Grid()
{
	for (ColorGroup* group : m_pColorGroups)
	{
		delete group;
	}
}

void Grid::Draw() const
{
	utils::SetColor(Color4f{ 0.5f, 0.5f, 0.5f, 1.0f });
	const float mapSize{ m_GridSize * m_TileSize };
	utils::FillRect(0.0f, 0.0f, mapSize, mapSize);

	for (ColorGroup* group : m_pColorGroups)
	{
		group->Draw(m_TileSize);
	}
}

std::pair<bool, ColorGroup*> Grid::TryChangeColor(const Point2i& position, Color color)
{
	ColorGroup* pGroup{ GetGroupAt(position) };
	Color groupColor{ pGroup->GetColor() };
	if (groupColor == Color::blue || groupColor == Color::red || groupColor == color)
	{
		return std::make_pair(false, pGroup);
	}
	pGroup->SetColor(color);
	MergeGroups(pGroup);
	return std::make_pair(true, pGroup);
}

Color Grid::GetColorAt(const Point2i& position) const
{
	ColorGroup* pGroup{ GetGroupAt(position) };
	if (pGroup != nullptr)
	{
		return pGroup->GetColor();
	}
	else
	{
		return Color::red;
	}
}

void Grid::HandleCollision(Vector2f& velocity, Point2f& position, float objectSize) const
{
	if (position.x - objectSize / 2.0f < 0.0f)
	{
		velocity.x = -velocity.x;
		position.x = objectSize / 2.0f;
		return;
	}
	else if (position.x + objectSize / 2.0f >= m_GridSize * m_TileSize)
	{
		velocity.x = -velocity.x;
		position.x = m_GridSize * m_TileSize - objectSize / 2.0f;
		return;
	}
	
	if (position.y - objectSize / 2.0f < 0.0f)
	{
		velocity.y = -velocity.y;
		position.y = objectSize / 2.0f;
		return;
	}
	else if (position.y + objectSize / 2.0f >= m_GridSize * m_TileSize)
	{
		velocity.y = -velocity.y;
		position.y = m_GridSize * m_TileSize - objectSize / 2.0f;
		return;
	}

	const Point2i leftPos{ int(floor((position.x - objectSize / 2.0f) / m_TileSize)), int(floor(position.y / m_TileSize)) };
	const Point2i rightPos{ int(floor((position.x + objectSize / 2.0f) / m_TileSize)), int(floor(position.y / m_TileSize)) };
	const Point2i upPos{ int(floor(position.x / m_TileSize)), int(floor((position.y + objectSize / 2.0f) / m_TileSize)) };
	const Point2i bottomPos{ int(floor(position.x / m_TileSize)), int(floor((position.y - objectSize / 2.0f) / m_TileSize)) };

	if (GetGroupAt(leftPos) == nullptr)
	{
		velocity.x = -velocity.x;
		position = Point2f{ (leftPos.x+1) * m_TileSize + objectSize / 2.0f, leftPos.y * m_TileSize + m_TileSize / 2.0f };
		return;
	}
	if (GetGroupAt(rightPos) == nullptr)
	{
		velocity.x = -velocity.x;
		position = Point2f{ rightPos.x * m_TileSize - objectSize / 2.0f, rightPos.y * m_TileSize + m_TileSize / 2.0f };
		return;
	}
	if (GetGroupAt(upPos) == nullptr)
	{
		velocity.y = -velocity.y;
		position = Point2f{ upPos.x * m_TileSize + m_TileSize / 2.0f, upPos.y * m_TileSize - objectSize / 2.0f };
		return;
	}
	if (GetGroupAt(bottomPos) == nullptr)
	{
		velocity.y = -velocity.y;
		position = Point2f{ bottomPos.x * m_TileSize + m_TileSize / 2.0f, (bottomPos.y + 1) * m_TileSize + objectSize / 2.0f };
		return;
	}
}

int Grid::GetSize() const
{
	return m_GridSize;
}

bool Grid::CanPlace(Color color, ColorGroup* skipGroup) const
{
	for (ColorGroup* group : m_pColorGroups)
	{
		const Color curColor{ group->GetColor() };
		if (curColor == Color::red || curColor == Color::blue)
		{
			continue;
		}
		if (curColor == color)
		{
			continue;
		}
		if (group == skipGroup)
		{
			continue;
		}
		return true;
	}
	return false;
}

int Grid::GetTilesAmount(Color color) const
{
	int nrTiles{};
	for (ColorGroup* group : m_pColorGroups)
	{
		if (group->GetColor() == color)
		{
			nrTiles += int(group->GetSize());
		}
	}
	return nrTiles;
}

bool Grid::IsValidPosition(const Point2i& position)
{
	return GetGroupAt(position) != nullptr;
}

bool Grid::Contains(ColorGroup* pGroup)
{
	for (ColorGroup* pOther : m_pColorGroups)
	{
		if (pOther == pGroup)
		{
			return true;
		}
	}
	return false;
}

int Grid::GetCorruptionAmount() const
{
	return m_NrCorruptions;
}

//bool Grid::IsValidPosition(const Point2i& position) const
//{
//	if (position.x < 0 || position.x >= m_GridSize)
//	{
//		return false;
//	}
//	if (position.y < 0 || position.y >= m_GridSize)
//	{
//		return false;
//	}
//	return true;
//}

void Grid::ExtendGroup(int x, int y, int gridSize, Color* pGrid, std::unordered_set<int>& handledTiles, ColorGroup* curGroup)
{
	const int index{ x * gridSize + y };

	for (int extraX{ -1 }; extraX < 2; ++extraX)
	{
		for (int extraY{ -1 }; extraY < 2; ++extraY)
		{
			if (abs(extraX + extraY) != 1)
			{
				continue;
			}

			const int curX = x + extraX;
			const int curY = y + extraY;
			const int index = curX * gridSize + curY;

			if (curX < 0 || curX >= gridSize || curY < 0 || curY >= gridSize)
			{
				continue;
			}
			if (handledTiles.contains(index))
			{
				continue;
			}

			if (pGrid[index] == curGroup->GetColor())
			{
				curGroup->AddTile(Point2i{ curX, curY });
				handledTiles.insert(index);
				ExtendGroup(curX, curY, gridSize, pGrid, handledTiles, curGroup);
			}
		}
	}
}

void Grid::ExtendGroup(int x, int y, int gridSize, std::unordered_set<int>& handledTiles, ColorGroup* curGroup, ColorGroup* originalGroup)
{
	for (int extraX{ -1 }; extraX < 2; ++extraX)
	{
		for (int extraY{ -1 }; extraY < 2; ++extraY)
		{
			if (abs(extraX + extraY) != 1)
			{
				continue;
			}

			const int curX = x + extraX;
			const int curY = y + extraY;
			const int index = curX * gridSize + curY;

			if (curX < 0 || curX >= gridSize || curY < 0 || curY >= gridSize)
			{
				continue;
			}
			if (!originalGroup->Contains(Point2i{ curX,curY }))
			{
				continue;
			}
			if (handledTiles.contains(index))
			{
				continue;
			}

			curGroup->AddTile(Point2i{ curX, curY });
			handledTiles.insert(index);
			ExtendGroup(curX, curY, m_GridSize, handledTiles, curGroup, originalGroup);
		}
	}
}

void Grid::MergeGroups(ColorGroup* pGroup)
{
	for (const Point2i& tile : pGroup->GetTiles())
	{
		for (int extraX{ -1 }; extraX < 2; ++extraX)
		{
			for (int extraY{ -1 }; extraY < 2; ++extraY)
			{
				if (abs(extraX + extraY) != 1)
				{
					continue;
				}

				const int curX = tile.x + extraX;
				const int curY = tile.y + extraY;
				const int index = curX * m_GridSize + curY;

				if (curX < 0 || curX >= m_GridSize || curY < 0 || curY >= m_GridSize)
				{
					continue;
				}

				const Point2i curPos{ curX,curY };

				if (pGroup->Contains(curPos))
				{
					continue;
				}

				for (int i{ int(m_pColorGroups.size()) - 1 }; i >= 0; --i)
				{
					ColorGroup* pOther = m_pColorGroups[i];
					if (pOther == pGroup)
					{
						continue;
					}

					if(pOther->Contains(curPos))
					{
						if (pGroup->GetColor() == pOther->GetColor())
						{
							pGroup->Merge(pOther);
							m_pColorGroups[i] = m_pColorGroups[m_pColorGroups.size() - 1];
							m_pColorGroups.pop_back();
							MergeGroups(pGroup);
							if (rand() % 10 == 0)
							{
								const int nrCorruptions{ rand() % 3 + 1 };
								for (int i{ 0 }; i < nrCorruptions; ++i)
								{
									AddCorruption();
								}
							}
							return;
						}
						break;
					}
				}
			}
		}
	}
}

ColorGroup* Grid::GetGroupAt(const Point2i& tile) const
{
	for (int i{ int(m_pColorGroups.size()) - 1 }; i >= 0; i--)
	{
		ColorGroup* pGroup = m_pColorGroups[i];
		if (pGroup->Contains(tile))
		{
			return pGroup;
		}
	}
	return nullptr;
}

ColorGroup* Grid::GetLargestTarget(ColorGroup* pSkipGroup) const
{
	ColorGroup* pLargestGroup{};

	for (ColorGroup* pGroup : m_pColorGroups)
	{
		if (pGroup == pSkipGroup)
		{
			continue;
		}

		if (pGroup->GetColor() == Color::red || pGroup->GetColor() == Color::blue)
		{
			continue;
		}

		if (pLargestGroup == nullptr || pGroup->GetSize() > pLargestGroup->GetSize())
		{
			pLargestGroup = pGroup;
		}
	}

	return pLargestGroup;
}

void Grid::AddCorruption()
{
	++m_NrCorruptions;

	bool canPlaceCorruption{ false };
	int groupIndex{};
	ColorGroup* pGroup{};
	Point2i curTile{};
	do
	{
		canPlaceCorruption = true;

		groupIndex = rand() % int(m_pColorGroups.size());
		pGroup = m_pColorGroups[groupIndex];
		curTile = pGroup->GetTileAtIndex(rand() % int(pGroup->GetTiles().size()));

		for (int x{ curTile.x - 1 }; x <= curTile.x + 1; ++x)
		{
			if (x < 0 || x >= m_GridSize)
			{
				continue;
			}

			for (int y{ curTile.y - 1 }; y <= curTile.y + 1; ++y)
			{
				if (y < 0 || y >= m_GridSize)
				{
					continue;
				}

				if (!IsValidPosition(Point2i{ x,y }))
				{
					canPlaceCorruption = false;
					break;
				}
			}

			if (!canPlaceCorruption)
			{
				break;
			}
		}
	} while (!canPlaceCorruption);

	pGroup->RemoveTile(curTile);
	if (pGroup->GetSize() == 0)
	{
		delete pGroup;
		m_pColorGroups[groupIndex] = m_pColorGroups[m_pColorGroups.size() - 1];
		m_pColorGroups.pop_back();
		return;
	}

	if (pGroup->GetSize() > 1)
	{
		std::unordered_set<int> handledTiles{};
		const Color color{ pGroup->GetColor() };

		ColorGroup* curGroup{ nullptr };
		const std::vector<Point2i>& tiles{ pGroup->GetTiles() };
		for (int i{ 0 }; i < int(tiles.size()); ++i)
		{
			const Point2i curTile{ tiles[i] };
			const int index = curTile.x * m_GridSize + curTile.y;
			if (handledTiles.contains(index))
			{
				continue;
			}
			curGroup = new ColorGroup(color);
			curGroup->AddTile(curTile);

			handledTiles.insert(index);
			ExtendGroup(curTile.x, curTile.y, m_GridSize, handledTiles, curGroup, pGroup);
			m_pColorGroups.push_back(curGroup);
		}

		delete pGroup;
		m_pColorGroups[groupIndex] = m_pColorGroups[m_pColorGroups.size() - 1];
		m_pColorGroups.pop_back();
	}
}
