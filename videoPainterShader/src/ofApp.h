#pragma once

#define WEBCAM

#include "ofMain.h"

class ofApp : public ofBaseApp{
public:
    
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
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
	

    ofImage     brushImg;
    ofImage     brushImg2;
    
    ofVideoPlayer andreas;
    ofVideoPlayer torben;
    
    ofFbo       maskFbo;
    ofFbo       fbo;

    ofFbo       maskFbo2;
    ofFbo       fbo2;
    
    ofShader    shader;
    ofShader    shader2;
    
    bool        bBrushDown;
    
    bool        eraserBrush = false;
    
    int layer = 0;
    
    
};
