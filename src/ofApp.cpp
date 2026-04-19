#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetBackgroundColor(50);
	ofxGuiSetDefaultWidth(500); // Double the default width
	ofxGuiSetDefaultHeight(30); // Make bars taller

	// 1. Setup the panel
	m_gui.setup("Settings");
	m_gui.add(m_addStartCellLabel.setup("", "Press 's' and click to select Start cell"));
	m_gui.add(m_addTargetCellLabel.setup("", "Press 't' and click to select Target cell"));
	m_gui.add(m_addWallCellLabel.setup("", "Press 'w' and click to select Wall(s)"));
	m_gui.add(m_AStarSearchLabel.setup("", "Press 'a' to start A* search"));
	m_gui.add(m_resetLabel.setup("", "Press 'r' to reset the grid"));
	m_gui.add(m_gridDimension.setup("Grid Dimension", 16, 8, 64));

	m_gridDimension.addListener(this, &ofApp::onDimensonChanged);

	resetGrid();
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::onDimensonChanged(int & newValue) {
	resetGrid();
}

//--------------------------------------------------------------
void ofApp::resetGrid() {

	m_wallsIndices.clear();
	m_pathIndices.clear();
	m_startCellIndex = -1;
	m_targetCellIndex = -1;

	m_grid.clear();
	m_grid.resize(m_gridDimension * m_gridDimension, Cell());
	for (auto i = 0; i < m_grid.size(); i++) {
		m_grid[i].x = i % m_gridDimension;
		m_grid[i].y = i / m_gridDimension;
	}
	m_cellSize = ofGetWidth() / m_gridDimension;
}

//--------------------------------------------------------------
void ofApp::drawPath() {
	for (auto i = 0; i < m_pathIndices.size(); i++) {
		int pathIndex = m_pathIndices[i];

		const Cell & pathCell = m_grid[pathIndex];
		int indexX = pathCell.x * m_cellSize;
		int indexY = pathCell.y * m_cellSize;
		ofSetColor(150, 150, 100);
		ofDrawRectangle(indexX, indexY, m_cellSize, m_cellSize);
	}
}

//--------------------------------------------------------------
void ofApp::drawCells() {
	if (m_startCellIndex != -1) {
		const Cell & startCell = m_grid[m_startCellIndex];
		int indexX = startCell.x * m_cellSize;
		int indexY = startCell.y * m_cellSize;
		ofSetColor(0, 255, 0);
		ofDrawRectangle(indexX, indexY, m_cellSize, m_cellSize);
	}
	if (m_targetCellIndex != -1) {
		const Cell & targetCell = m_grid[m_targetCellIndex];
		int indexX = targetCell.x * m_cellSize;
		int indexY = targetCell.y * m_cellSize;
		ofSetColor(255, 0, 0);
		ofDrawRectangle(indexX, indexY, m_cellSize, m_cellSize);
	}

	for (int idx : m_wallsIndices) {
		const Cell & wallCell = m_grid[idx];
		int indexX = wallCell.x * m_cellSize;
		int indexY = wallCell.y * m_cellSize;
		ofSetColor(0, 0, 0);
		ofDrawRectangle(indexX, indexY, m_cellSize, m_cellSize);
	}
}

//--------------------------------------------------------------
void ofApp::drawGrid() {

	ofSetColor(255, 255, 255, 100);
	int w = ofGetWidth();
	int h = ofGetHeight();

	for (int x = 0; x < w; x += m_cellSize) {
		ofDrawLine(x, 0, x, h);
	}
	for (int y = 0; y < h; y += m_cellSize) {
		ofDrawLine(0, y, w, y);
	}
}

//--------------------------------------------------------------

void ofApp::onHover() {
	int x = ofGetMouseX() / m_cellSize;
	int y = ofGetMouseY() / m_cellSize;
	int indexX = x * m_cellSize;
	int indexY = y * m_cellSize;

	ofSetColor(200, 100, 200, 100);
	ofDrawRectangle(indexX, indexY, m_cellSize, m_cellSize);
}
//--------------------------------------------------------------
void ofApp::draw() {
	ofSetBackgroundColor(50, 120);

	drawGrid();
	drawPath();
	onHover();
	drawCells();
	m_gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	//reset the grid
	if (ofGetKeyPressed('r')) {

		resetGrid();
	}

	if (ofGetKeyPressed('a')) {
		if (m_startCellIndex == -1 || m_targetCellIndex == -1) {
			ofSystemAlertDialog("Starting cell or Target cell not selected");
		} else {
			AStarPathFinder(m_grid, m_gridDimension, m_startCellIndex, m_targetCellIndex);
			m_pathIndices = ReconstructPath(m_grid, m_gridDimension, m_startCellIndex, m_targetCellIndex);
			if (m_pathIndices.empty()) {
				ofSystemAlertDialog("Unable to reach target");
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	int xx = x / m_cellSize;
	int yy = y / m_cellSize;

	if (xx >= 0 && xx < m_gridDimension && yy >= 0 && yy < m_gridDimension && !m_gui.getShape().inside(x, y)) {
		int index = yy * m_gridDimension + xx;
		if (ofGetKeyPressed('w')) {
			m_grid[index].type = WALL;
			m_wallsIndices.insert(index);
		}
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	int xx = x / m_cellSize;
	int yy = y / m_cellSize;

	int index = yy * m_gridDimension + xx;

	if (xx >= 0 && xx < m_gridDimension && yy >= 0 && yy < m_gridDimension && !m_gui.getShape().inside(x, y)) {

		if (ofGetKeyPressed('s')) {
			m_grid[index].type = START;
			if (m_startCellIndex != -1) {
				m_grid[m_startCellIndex].type = EMPTY;
			}
			m_startCellIndex = index;
		}

		else if (ofGetKeyPressed('t')) {
			m_grid[index].type = TARGET;
			if (m_targetCellIndex != -1) {
				m_grid[m_targetCellIndex].type = EMPTY;
			}
			m_targetCellIndex = index;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
}
