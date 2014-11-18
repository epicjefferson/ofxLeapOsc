#pragma once

#include "ofMain.h"
#include "ofxLeapMotion.h"
#include "LeapVisualizer.h"
#include "ofxOsc.h"

#include "ofxXmlSettings.h"
#include "LeapFrame.h"
#include "LeapRecorder.h"

#define HOST "localhost"
#define PORT 44000


class testApp : public ofBaseApp{

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
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void exit();

    //------------------------------
    // Gui
    void drawText();

    //------------------------------
    // Leap
	ofxLeapMotion leap;
    LeapVisualizer leapVisualizer;
    ofEasyCam cam;

    //OSC
    ofxOscSender oscSender;

    bool bShowText;

    float drawW, drawH;

    void drawLiveForRecording();
    void drawPlayback();
    void drawLeapWorld();
};

const string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
const string boneNames[] = {"Metacarpal", "Proximal", "Middle", "Distal"};
const string stateNames[] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};
