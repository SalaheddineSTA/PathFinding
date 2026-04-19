#pragma once

#include <ofMain.h>
#include <ofxGui.h> // Include the GUI addon
#include <vector>
#include <unordered_set>
#include "pathFinder.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);


		void drawGrid();
		void onHover();
		void drawCells();
		void drawPath();
		void resetGrid();
		void onDimensonChanged(int & newValue);

		ofxPanel m_gui;
		ofxIntSlider m_gridDimension;

		ofxLabel m_addStartCellLabel;
		ofxLabel m_addTargetCellLabel;
		ofxLabel m_addWallCellLabel;
		ofxLabel m_AStarSearchLabel;
		ofxLabel m_resetLabel;

		std::vector<Cell> m_grid;
		std::vector<int> m_pathIndices;

		std::unordered_set<int> m_wallsIndices;
		int m_startCellIndex {-1};
		int m_targetCellIndex {-1};

		int m_cellSize { 0 };
};
