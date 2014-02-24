#pragma once

#include "ofxTeensyOcto.h"
#include "ofMain.h"

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    // LED - Teensy stuff
    //-----------------------------
    ofxTeensyOcto teensy;
    int stripWidth;
    int stripHeight;
    int stripsPerPort;
    int numPorts;
    int brightness;
    float waveSpeed;
    
    // FBO stuff
    //-----------------------------
    ofFbo fbo;
    void updateFbo();
    void drawFbo();
    int fboRotate;
    bool fboFlip;
    ofTexture tex;
    ofPixels guiPixels;
    
    // gui views
    //-----------------------------
    void drawPanels();
    
    // Graphic functions
    //-----------------------------
    void drawDemos();
    void drawDebug();
    void drawRainbowH();
    void drawRainbowV();
    void drawWaves();
    void drawVideos();
    void drawImages();
    void drawTestPattern();
    int drawModes;
    int demoModes;
    float counterShape;
    int hue;
    
    // Test Pattern
    //-----------------------------
    ofTrueTypeFont font;
    
    // Video player
    //-----------------------------
    ofDirectory dirVid;
    vector<ofVideoPlayer> vid;
    int currentVideo;
    bool videoOn;
    
    // Image player
    //-----------------------------
    ofDirectory dirImg;
    vector<ofImage> img;
    int currentImage;
		
};
