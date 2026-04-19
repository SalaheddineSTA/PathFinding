#include "pathFinder.h"
#include <algorithm>
#include <cmath>

void AStarPathFinder(std::vector<Cell> & grid, int gridDimension, int startIndex, int targetIndex) {

	// basic validation to avoid crashes
	if (grid.empty()) return;
	int total = static_cast<int>(grid.size());
	if (startIndex < 0 || startIndex >= total) return;
	if (targetIndex < 0 || targetIndex >= total) return;
	if (gridDimension <= 0) return;

	// trivial case
	if (startIndex == targetIndex) {
		grid[startIndex].type = START;
		grid[targetIndex].type = TARGET;
		return;
	}

	// open/closed lists store indices into grid to avoid copying Cells
	std::vector<int> openList;
	std::vector<bool> inOpen(total, false);
	std::vector<bool> inClosed(total, false);

	Cell startCell = grid[startIndex];
	Cell targetCell = grid[targetIndex];
	startCell.g = 0;
	startCell.h = std::sqrt((targetCell.x - startCell.x) * (targetCell.x - startCell.x) + (targetCell.y - startCell.y) * (targetCell.y - startCell.y));
	startCell.f = startCell.g + startCell.h;

	// write back start into grid and add its index to open list
	grid[startIndex] = startCell;
	openList.push_back(startIndex);
	inOpen[startIndex] = true;

	while (!openList.empty()) {
		auto it = std::min_element(openList.begin(), openList.end(), [&](int a, int b) {
			return grid[a].f < grid[b].f;
		});
		int currentIndex = *it;
		Cell currentCell = grid[currentIndex];

		// if we've reached the target, we're done; leave reconstruction to ReconstructPath
		if (currentIndex == targetIndex) {
			// ensure endpoints retain their types
			grid[startIndex].type = START;
			grid[targetIndex].type = TARGET;
			return;
		}

		// remove from open list and mark membership
		openList.erase(it);
		inOpen[currentIndex] = false;
		inClosed[currentIndex] = true;

		// iterate neighbors (including diagonals)
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				if (i == 0 && j == 0) continue; // skip self

				int nx = currentCell.x + i;
				int ny = currentCell.y + j;
				if (nx < 0 || ny < 0 || nx >= gridDimension || ny >= gridDimension) continue;

				int neighborIndex = ny * gridDimension + nx;
				if (neighborIndex < 0 || neighborIndex >= static_cast<int>(grid.size())) continue;

				// skip walls
				if (grid[neighborIndex].type == WALL) continue;

				// skip if neighbor already in closed list
				if (inClosed[neighborIndex]) continue;

				// check if neighbor already in open list
				bool neighborInOpen = inOpen[neighborIndex];

				float distanceToTarget = std::sqrt((targetCell.x - grid[neighborIndex].x) * (targetCell.x - grid[neighborIndex].x) + (targetCell.y - grid[neighborIndex].y) * (targetCell.y - grid[neighborIndex].y));
				float moveCost = (i == 0 || j == 0) ? 1.0f : 1.41421356f; // std::sqrt(i * i + j * j); distance to neighbor from current

				if (neighborInOpen) {
					// Update g, h, f and parent if this path is better
					float tentativeG = currentCell.g + moveCost; // uniform_cost
					if (tentativeG < grid[neighborIndex].g) {
						grid[neighborIndex].parentX = currentCell.x;
						grid[neighborIndex].parentY = currentCell.y;
						grid[neighborIndex].g = tentativeG;
						grid[neighborIndex].h = distanceToTarget;
						grid[neighborIndex].f = grid[neighborIndex].g + grid[neighborIndex].h;
					}
				} else {
					// Add to open list and update grid
					grid[neighborIndex].parentX = currentCell.x;
					grid[neighborIndex].parentY = currentCell.y;
					grid[neighborIndex].g = currentCell.g + moveCost; // uniform_cost
					grid[neighborIndex].h = distanceToTarget;
					grid[neighborIndex].f = grid[neighborIndex].g + grid[neighborIndex].h;
					openList.push_back(neighborIndex);
					inOpen[neighborIndex] = true;
				}
			}
		}
	}
}

std::vector<int> ReconstructPath(const std::vector<Cell> & grid, int gridDimension, int startIndex, int targetIndex) {

	std::vector<int> pathIndices;
	int currentIndex = targetIndex;
	while (currentIndex != startIndex) {
		pathIndices.push_back(currentIndex);
		const Cell & currentCell = grid[currentIndex];
		if (currentCell.parentX == -1 || currentCell.parentY == -1) {
			// No valid parent, path reconstruction failed
			return {};
		}
		currentIndex = currentCell.parentY * gridDimension + currentCell.parentX;
	}
	std::reverse(pathIndices.begin(), pathIndices.end());
	return pathIndices;
}
