#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLFWWindowSettings settings;
	settings.setGLVersion(4, 6);
#ifdef PRODUCTION
	settings.setSize(1280,720);
	settings.setPosition({ 4000, 100 });
#else
	settings.setSize(2340, 1440);
#endif
	ofCreateWindow(settings);
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
