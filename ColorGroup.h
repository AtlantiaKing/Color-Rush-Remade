#pragma once
#include "GridTile.h"
#include <vector>
#include "structs.h"

class ColorGroup final
{
public:
	ColorGroup(Color color);

	void AddTile(const Point2i& tile);
	void Merge(ColorGroup* other);
	void SetColor(Color color);
	void RemoveTileAt(int index);
	void RemoveTile(const Point2i& tile);

	Color GetColor() const;
	void Draw(float tileSize) const;
	bool Contains(const Point2i& position) const;
	const std::vector<Point2i>& GetTiles() const;
	const size_t GetSize() const;
	Point2i GetTileAtIndex(int index) const;
private:
	Color m_Color;
	std::vector<Point2i> m_Tiles{};
};

