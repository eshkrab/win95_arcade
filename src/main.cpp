#include "ofMain.h"
#include "testApp.h"

int main() {
    ofSetupOpenGL(800, 640, OF_WINDOW);
	ofRunApp(new testApp());
}
