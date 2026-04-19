#include "ofMain.h"
#include "ofApp.h"


//========================================================================
int main( ){

    // Use GLFW window settings to control resizable flag
    ofGLFWWindowSettings settings;
	settings.setSize(1024, 1024);
	settings.windowMode = OF_WINDOW; // can also be OF_FULLSCREEN
	settings.resizable = false;
	auto window = ofCreateWindow(settings);

	ofRunApp(window, std::make_shared<ofApp>());
	ofRunMainLoop();

}
