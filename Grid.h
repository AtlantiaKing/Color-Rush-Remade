#pragma once
#include <vector>
#include <unordered_set>
#include "utils.h"
#include "Colors.h"

class ColorGroup;

class Grid final
{
public:
	Grid(int gridSize, float tileSize);
	Grid(const Grid& other) = delete;
	Grid(Grid&& other) = delete;
	~Grid();
	Grid& operator=(const Grid& other) = delete;
	Grid& operator=(Grid&& other) = delete;

	void Draw() const;
	std::pair<bool, ColorGroup*> TryChangeColor(const Point2i& position, Color color);
	Color GetColorAt(const Point2i& position) const;
	void HandleCollision(Vector2f& velocity, Point2f& position, float objectSize) const;
	int GetSize() const;
	bool CanPlace(Color color, ColorGroup* skipGroup) const;
	int GetTilesAmount(Color color) const;
	bool IsValidPosition(const Point2i& position);
	bool Contains(ColorGroup* pGroup);
	int GetCorruptionAmount() const;
	ColorGroup* GetGroupAt(const Point2i& pos) const;
	ColorGroup* GetLargestTarget(ColorGroup* pSkipGroup) const;
private:
	const float m_TileSize;
	const int m_GridSize;
	std::vector<ColorGroup*> m_pColorGroups{};

	int m_NrCorruptions{};

	void ExtendGroup(int x, int y, int gridSize, Color* pGrid, std::unordered_set<int>& handledTiles, ColorGroup* curGroup);
	void ExtendGroup(int x, int y, int gridSize, std::unordered_set<int>& handledTiles, ColorGroup* curGroup, ColorGroup* originalGroup);
	void MergeGroups(ColorGroup* group);
	void AddCorruption();
};

