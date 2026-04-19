#pragma once

#include <limits>
#include <vector>

enum CellType
{
	EMPTY,
	WALL,
	START,
	TARGET
};

struct Cell {
	//Cell() = default;
	~Cell() = default;
	int x { 0 };
	int y { 0 };

	CellType type = EMPTY;
	float g = std::numeric_limits<float>::infinity();
	float f = std::numeric_limits<float>::infinity(); 
	float h = 0;

	int parentX { -1 };
	int parentY { -1 };

};



void AStarPathFinder(std::vector<Cell> & grid, int gridDimension, int startIndex, int targetIndex);

// Reconstruct ordered path indices from outGrid using parentX/parentY
std::vector<int> ReconstructPath(const std::vector<Cell> & outGrid, int gridDimension, int startIndex, int targetIndex);
