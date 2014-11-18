#include "ofApp.h"

/*
// OPENFRAMEWORKS + LEAP MOTION SDK 2.0 HAND SKELETON DEMO
// By Golan Levin (@golan), http://github.com/golanlevin
// Uses ofxLeapMotion addon by Theo Watson, with assistance from Dan Wilcox
// Supported in part by the Frank-Ratchye STUDIO for Creative Inquiry at CMU
*/



/* Note on OS X, you must have this in the Run Script Build Phase of your project.
where the first path ../../../addons/ofxLeapMotion/ is the path to the ofxLeapMotion addon.

cp -f ../../../addons/ofxLeapMotion/libs/lib/osx/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/libLeap.dylib"; install_name_tool -change ./libLeap.dylib @executable_path/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/$PRODUCT_NAME";

   If you don't have this you'll see an error in the console: dyld: Library not loaded: @loader_path/libLeap.dylib
*/




//--------------------------------------------------------------
void testApp::setup(){

 //    // All data will be in SharedData
 //    string basePath = ofToDataPath("", true);
 //    ofSetDataPathRoot("../../../../../SharedData/");
/*
    cameraWidth = 1024;
    cameraHeight = 768;*/
	drawW = 640;
    drawH = 480;

    //--------------- Setup leap
	leap.open();
    leapVisualizer.setup();
    //leapRecorder.setup();
    //prevLeapFrameRecorder.setup();
	cam.setOrientation(ofPoint(-20, 0, 0));

    bShowText = true;
/*
    playingFrame = 0;
    bPlaying = false;
	bEndRecording = false;
    playing = false;
    bUseVirtualProjector = false;
    bUseFbo = false;
    bInputMousePoints = false;
    bShowCalibPoints = true;
    bRecordingForCalibration = false;
    bRecordThisCalibFrame = false;
    bUseCorrectedCamera = false;
    bShowText = false;
    bShowLargeCamImageOnTop = false;    // temp for quickly showing on hand video only
    bShowOffBy1Frame = false;
    framesBackToPlay = 1;

    folderName = ofGetTimestampString();
    lastIndexVideoPos.set(0,0,0);
    lastIndexLeapPos.set(0,0,0);
*/

    //--------------- App settings
    // ofSetFrameRate(60);
    ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_WARNING);
	ofSetCylinderResolution (16, 1, 16);

   // ofEnableAlphaBlending();
    glEnable(GL_NORMALIZE); // needed??
	//glEnable(GL_DEPTH);
    glEnable(GL_DEPTH_TEST); // why is this messing the render up in the projector cam??????


    string versionDisplay = "Using openFrameworks version: " + ofToString( ofGetVersionInfo());
	cout << versionDisplay;

    //setup OSC
    oscSender.setup(HOST, PORT);


}


//--------------------------------------------------------------
void testApp::update(){
    //-------------  Leap update
	leap.markFrameAsOld();

}


//--------------------------------------------------------------
void testApp::draw(){

    ofBackground(0,0,0);
    drawLiveForRecording();

/*
    if(bShowText){
       drawText();
    }
*/
}

//--------------------------------------------------------------
void testApp::drawLiveForRecording(){

    // draw live image
    ofSetColor(ofColor::white);

    // draw leap
    drawLeapWorld();

}


void testApp::drawLeapWorld(){

    cam.begin();

    // draw grid
    ofSetColor(255);
    leapVisualizer.drawGrid();

    // draw live leap
    leapVisualizer.drawFrame(leap);

    //send
    // leapVisualizer.sendOsc(oscSender);
    vector <Hand> hands = leap.getLeapHands();

    if (hands.size() > 0) {
        for (int h=0; h<hands.size(); h++){
            ofxOscMessage m;
            m.setAddress( "/test" );
            m.addIntArg( h );
            // m.addFloatArg( 3.5f );
            m.addStringArg( "hands found" );
            // m.addFloatArg( ofGetElapsedTimef() );
            oscSender.sendMessage( m );
            m.clear();

            // Get the current hand
            Hand & hand = hands[h];

            string handType = hand.isLeft() ? "Left" : "Right";     //pretty sweet way of knowing which hand

            //cout << handType;

            ////send handType over OSC
            // m.setAddress("/handType");
            // m.addStringArg(handType);
            // oscSender.sendMessage(m);
            // m.clear();

            //get palm position
            //cout << ", palm" << hand.palmPosition() << endl;

            m.setAddress("/" + handType);
            m.addStringArg("/palm");
            m.addFloatArg(hand.palmPosition()[0]);      // X
            m.addFloatArg(hand.palmPosition()[1]);      // Y
            m.addFloatArg(hand.palmPosition()[2]);      // Z
            oscSender.sendMessage(m);
            m.clear();


            //get grab Strength
            m.setAddress("/" + handType);
            m.addStringArg("/grabStrength");
            m.addFloatArg(hand.grabStrength());
            oscSender.sendMessage(m);
            m.clear();


            // Calculate the hand's pitch, roll, and yaw angles
            const Vector normal = hand.palmNormal();
            const Vector direction = hand.direction();

            // cout << string(2, ' ') <<  "pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
            //             << "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
            //             << "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << endl;

            m.setAddress("/" + handType);
            m.addStringArg("/pitch");
            m.addFloatArg(direction.pitch()*RAD_TO_DEG);
            oscSender.sendMessage(m);
            m.clear();

            m.setAddress("/" + handType);
            m.addStringArg("/roll");
            m.addFloatArg(normal.roll()*RAD_TO_DEG);
            oscSender.sendMessage(m);
            m.clear();

            m.setAddress("/" + handType);
            m.addStringArg("/yaw");
            m.addFloatArg(direction.yaw()*RAD_TO_DEG);
            oscSender.sendMessage(m);
            m.clear();


            // Get the Arm bone
            Arm arm = hand.arm();

            // cout << string(2, ' ') <<  "Arm direction: " << arm.direction()
            //   << " wrist position: " << arm.wristPosition()
            //   << " elbow position: " << arm.elbowPosition() << endl;

            m.setAddress("/" + handType);
            m.addStringArg("/armDirection");
            m.addFloatArg(arm.direction()[0]);       // x
            m.addFloatArg(arm.direction()[1]);       // y
            m.addFloatArg(arm.direction()[2]);       // z
            oscSender.sendMessage(m);
            m.clear();

            m.setAddress("/" + handType);
            m.addStringArg("/wristPosition");
            m.addFloatArg(arm.wristPosition()[0]);       // x
            m.addFloatArg(arm.wristPosition()[1]);       // y
            m.addFloatArg(arm.wristPosition()[2]);       // z
            oscSender.sendMessage(m);
            m.clear();

            m.setAddress("/" + handType);
            m.addStringArg("/elbowPosition");
            m.addFloatArg(arm.elbowPosition()[0]);       // x
            m.addFloatArg(arm.elbowPosition()[1]);       // y
            m.addFloatArg(arm.elbowPosition()[2]);       // z
            oscSender.sendMessage(m);
            m.clear();


            // Get fingers
            const FingerList fingers = hand.fingers();
            for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
                const Finger finger = *fl;

                // cout << string(4, ' ') <<  fingerNames[finger.type()]
                //         << " finger, id: " << finger.id()
                //         << ", length: " << finger.length()
                //         << "mm, width: " << finger.width() << endl;

                // m.setAddress("/" + handType);
                // m.addStringArg("/fingers");
                // m.addStringArg("/" + fingerNames[finger.type()]);
                // m.addFloatArg(finger.length());
                // m.addFloatArg(finger.width());
                // oscSender.sendMessage(m);
                // m.clear();

                // Get finger bones
                for (int b = 0; b < 4; ++b) {
                    Bone::Type boneType = static_cast<Bone::Type>(b);
                    Bone bone = finger.bone(boneType);

                    // cout << string(6, ' ') <<  boneNames[boneType]
                    //           << " bone, start: " << bone.prevJoint()
                    //           << ", end: " << bone.nextJoint()
                    //           << ", direction: " << bone.direction() << endl;

                    //sending current bones' previous joint
                    m.setAddress("/" + handType);
                    m.addStringArg("/fingers");
                    m.addStringArg("/" + fingerNames[finger.type()]);
                    m.addStringArg("/" + boneNames[boneType]);
                    m.addStringArg("/prevJoint");
                    m.addFloatArg(bone.prevJoint()[0]);       // x
                    m.addFloatArg(bone.prevJoint()[1]);       // y
                    m.addFloatArg(bone.prevJoint()[2]);       // z
                    oscSender.sendMessage(m);
                    m.clear();

                    //sending current bones' next joint
                    m.setAddress("/" + handType);
                    m.addStringArg("/fingers");
                    m.addStringArg("/" + fingerNames[finger.type()]);
                    m.addStringArg("/" + boneNames[boneType]);
                    m.addStringArg("/nextJoint");
                    m.addFloatArg(bone.nextJoint()[0]);       // x
                    m.addFloatArg(bone.nextJoint()[1]);       // y
                    m.addFloatArg(bone.nextJoint()[2]);       // z
                    oscSender.sendMessage(m);
                    m.clear();
                }
            }

            // drawHand (hand,leap);
        }
        // ofxOscMessage m;
        // m.setAddress( "/test" );
        // m.addIntArg( 1 );
        // // m.addFloatArg( 3.5f );
        // m.addStringArg( "hands found" );
        // // m.addFloatArg( ofGetElapsedTimef() );
        // oscSender.sendMessage( m );
    }

    cam.end();
}



/*

//--------------------------------------------------------------
void testApp::drawText(){

	float textY = 500;

	ofSetColor(ofColor::white);
	//ofDrawBitmapString("Display, record & playback Leap 2.0 Controller data.", 20, textY); textY+=15;
	//ofDrawBitmapString("Built in openFrameworks by Golan Levin, golan@flong.com", 20, textY); textY+=15;
	//textY+=15;

	ofSetColor( (leap.isConnected()) ?  ofColor::green : ofColor(255,51,51)) ;
	ofDrawBitmapString( (leap.isConnected() ? "Leap is Connected!" : "Leap is NOT CONNECTED!"), 20, textY);
	ofDrawBitmapString( (leap.isConnected() ? "Leap is Connected!" : "Leap is NOT CONNECTED!"), 21, textY); textY+=15;
	textY+=15;

	ofSetColor(ofColor::white);
	ofDrawBitmapString("Press '1' to open playback recording from a directory", 20, textY); textY+=15;
    ofDrawBitmapString("Press '2' to apply calibration from a directory", 20, textY); textY+=15;
    ofDrawBitmapString("Press '3' to open calibration frames for calibration input", 20, textY); textY+=15;
    textY+=15;
    ofDrawBitmapString("Press 's' to toggle hand skeleton/cylinders", 20, textY); textY+=15;
	ofDrawBitmapString("Press 'c' to restore easy-cam orientation", 20, textY); textY+=15;
	ofDrawBitmapString("Press 'g' to toggle grid", 20, textY); textY+=15;
    ofDrawBitmapString("Press 'v' to toggle virtual projector", 20, textY); textY+=15;
    ofDrawBitmapString("Press 'w' to toggle calibration points draw", 20, textY); textY+=15;
    ofDrawBitmapString("Press 'C' to load current playback folder's calibration", 20, textY); textY+=15;
    ofDrawBitmapString("Press 'm' to allow mouse input points", 20, textY); textY+=15;
    ofDrawBitmapString("Press 'left/right' to advance frame by frame", 20, textY); textY+=15;
    ofDrawBitmapString("Press '' (space) to pause/play", 20, textY); textY+=15;

    string usePrev = bShowOffBy1Frame ? "on" : "off";
    ofDrawBitmapString("Press 'o' toggle use prev "+usePrev+" {} frames behind: "+ofToString(framesBackToPlay), 20, textY); textY+=15;


	textY+=15;

    textY = 500;
    int textX = 610;

    ofDrawBitmapString("All folders must be in SharedData/recordings", textX, textY); textY+=15;
    textY+=15;

    ofDrawBitmapString("Press 'l' to return to live mode", textX, textY); textY+=15;

    if (leap.isConnected()){
		ofDrawBitmapString("Press 'r' to toggle RECORDING", textX, textY); textY+=15;
        ofDrawBitmapString("Press 'R' to toggle RECORDING for CALIBRATION", textX, textY); textY+=15;
        ofDrawBitmapString("Press ' ' to record CALIBRATION frame", textX, textY); textY+=15;

	}
	if (leapVisualizer.XML.getNumTags("FRAME") > 0){
		ofDrawBitmapString("Press 'p' to pause PLAYBACK",  textX, textY); textY+=15;
	}

	if (bPlaying){
		ofSetColor(ofColor::green);
		ofDrawBitmapString("PLAYING! " + ofToString(playingFrame), textX, textY); textY+=15;
	} else if (bRecording){
		ofSetColor(ofColor::red);
		//ofDrawBitmapString("RECORDING! " + ofToString(leapRecorder.recordingFrameCount), textX, textY); textY+=15;
	}

    textY+=15;
    ofSetColor(ofColor::white);
    ofDrawBitmapString("Playback folder: "+folderName,  textX, textY); textY+=15;
    // ofDrawBitmapString("Calibration file: "+leapCameraCalibrator.dirNameLoaded,  textX, textY); textY+=15;

}
*/



//--------------------------------------------------------------
// void testApp::loadPlaybackFromDialogForCalibration(){

//     //Open the Open File Dialog
//     ofFileDialogResult openFileResult= ofSystemLoadDialog("Choose a recording folder:",true);

//     if (openFileResult.bSuccess){

//         string filePath = openFileResult.getName();
//         folderName = filePath;
//         loadAndPlayRecording(filePath);
//         video.setPlaying(true);
//         video.update();
//         playing = false;
//         indexRecorder.setup("recordings/"+folderName+"/calib","fingerCalibPts.xml");
//         indexRecorder.setDrawOffsets(drawW,0,cameraWidth/drawW,cameraHeight/drawH);
//     }
// }

//--------------------------------------------------------------
// void testApp::loadPlaybackFromDialog(){

//     //Open the Open File Dialog
//     ofFileDialogResult openFileResult= ofSystemLoadDialog("Choose a recording folder:",true);

//     if (openFileResult.bSuccess){

//         string filePath = openFileResult.getName();
//         folderName = filePath;
//         loadAndPlayRecording(filePath);

//     }
// }

//--------------------------------------------------------------
// void testApp::loadCalibrationFromDialog(){
//     //Open the Open File Dialog
//     ofFileDialogResult openFileResult= ofSystemLoadDialog("Choose a recording folder:",true);

//     if (openFileResult.bSuccess){
//         string filePath = openFileResult.getName();
//         calibrateFromXML(filePath);
//     }
// }

//--------------------------------------------------------------
// void testApp::finishRecording(){

//     bRecording = false;
//     bEndRecording = false;

//     ofFileDialogResult openFileResult= ofSystemSaveDialog(folderName,"Make a folder in SharedData/recordings:");

//     if (openFileResult.bSuccess){
//         folderName = openFileResult.getName();

//         int totalImage = MIN(currentFrameNumber,imageSequence.size());
//         for(int i = 0; i < totalImage; i++) {
//             if(imageSequence[i].getWidth() == 0) break;
//             ofSaveImage(imageSequence[i], "recordings/"+folderName+"/camera/"+ofToString(i, 3, '0') + ".jpg");
//         }

//         ofDirectory dir;
//         dir.open("recordings/"+folderName+"/leap");
//         if(!dir.exists())dir.createDirectory("recordings/"+folderName+"/leap");
//         leapRecorder.endRecording("recordings/"+folderName+"/leap/leap.xml");

//         loadAndPlayRecording(folderName);

//         if(bRecordingForCalibration){
//             playing = false;
//         }

//     }



//     imageSequence.clear();
//     imageSequence.resize(300);
// 	for(int i = 0; i < imageSequence.size(); i++) {
// 		imageSequence[i].allocate(cameraWidth,cameraHeight, OF_IMAGE_COLOR);
// 	}
//     currentFrameImg.clear();
//     currentFrameImg.allocate(cameraWidth,cameraHeight, OF_IMAGE_COLOR);

// }

//--------------------------------------------------------------
// void testApp::loadAndPlayRecording(string folder){

//     leapVisualizer.loadXmlFile("recordings/"+folder+"/leap/leap.xml");
//     video.load("recordings/"+folder+"/camera");

//     indexRecorder.setup("recordings/"+folder+"/calib","fingerCalibPts.xml");
//     indexRecorder.setDrawOffsets(drawW,0,cameraWidth/drawW,cameraHeight/drawH);

//     // open calibration if exists?

//     bPlaying = true;
//     playing = true;
//     currentFrameNumber = 0;

// }

//--------------------------------------------------------------
// void testApp::calibrateFromXML( string calibFolderName ){

//     leapCameraCalibrator.setup(cameraWidth, cameraHeight);
//     leapCameraCalibrator.loadFingerTipPoints("recordings/"+calibFolderName+"/calib/fingerCalibPts.xml");

//     if(useCorrectedCam()){
//         leapCameraCalibrator.correctCameraPNP(myCalibration);
//     }else{
//         leapCameraCalibrator.correctCamera();

//     }
// }

// bool testApp::useCorrectedCam(){

// #ifdef _USE_CORRECTED_CAMERA
//     if(bUseCorrectedCamera) return true;
//     else return false;
// #else
//     return false;
// #endif

// }

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    //testing OSC messaging
    if ( key =='a' || key == 'A' )
    {
        ofxOscMessage m;
        m.setAddress( "/test" );
        m.addIntArg( 1 );
        m.addFloatArg( 3.5f );
        m.addStringArg( "hello" );
        m.addFloatArg( ofGetElapsedTimef() );
        oscSender.sendMessage( m );
    }

// 	if ((key == 'r') || (key == 'R')){
// 		if (leap.isConnected()){
// 			// Toggle Recording.
// 			//reset so we don't store extra tags
// 			if(bRecording){
//                 bEndRecording = true;
//             }else{
//                 bRecording = !bRecording;
//                 folderName = ofGetTimestampString();
// 				leapRecorder.startRecording();
//                 leapVisualizer.XML.clear();
//                 bPlaying = false;
//                 currentFrameNumber = 0;
//                 if( key == 'R') bRecordingForCalibration = true;
//                 else bRecordingForCalibration = false;
// 			}

// 		}

// 	}

//     switch(key){
//         case OF_KEY_LEFT:
//             if(video.isLoaded()) video.goToPrevious();
//             break;
//         case OF_KEY_RIGHT:
//              if(video.isLoaded()) video.goToNext();
//             break;
//         case 'c':
//             cam.reset();
//             break;
//         case 'C':
//             calibrateFromXML(folderName);
//             break;
//         case 'F':
//             bUseFbo = !bUseFbo;
//             break;
//         case 'g':
//             leapVisualizer.bDrawGrid = !leapVisualizer.bDrawGrid;
//             break;
//         case 'l':
//             if(bPlaying) bPlaying = false;
//             break;
//         case 'm':
//             bInputMousePoints = !bInputMousePoints;
//             break;
//         case 'p':
//             if(bPlaying) playing = !playing;
//             break;
//         case 's':
//             leapVisualizer.bDrawSimple = !leapVisualizer.bDrawSimple;
//             break;
//         case 'v':
//             bUseVirtualProjector = !bUseVirtualProjector;
//             break;
//         case '1':
//             loadPlaybackFromDialog();
//             break;
//         case '2':
//             loadCalibrationFromDialog();
//             bUseVirtualProjector = true;
//             break;
//         case '3':
//             loadPlaybackFromDialogForCalibration();
//             break;
//         case 'w':
//             bShowCalibPoints = !bShowCalibPoints;
//             break;
//         case ' ':
//             if(bPlaying) playing = !playing;
//             else if(bRecordingForCalibration) bRecordThisCalibFrame = true;
//             break;
//         case 'u':
//             bUseCorrectedCamera = !bUseCorrectedCamera;
//             break;
//         case '0':
//             bShowLargeCamImageOnTop = !bShowLargeCamImageOnTop;
//             break;
//         case 'f':
//             ofToggleFullscreen();
//             break;
//         case '9':
//             if(drawW == 640){
//                 drawW = 1024;
//                 drawH = 768;
//                 bShowText = false;
//             }else{
//                 drawW = 640;
//                 drawH = 480;
//                 bShowText = true;
//             }
//             break;
//         case 'o':
//             bShowOffBy1Frame= !bShowOffBy1Frame;
//             break;
//         case '{':
//             framesBackToPlay--;
//             if(framesBackToPlay <0) framesBackToPlay = 5;
//         case '}':
//             framesBackToPlay++;
//             if(framesBackToPlay > 5) framesBackToPlay = 1;
//             break;
//     }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){



}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
/*
    if(bPlaying && bInputMousePoints){
        if(x > indexRecorder.xOffset && y > indexRecorder.yOffset &&
           x < indexRecorder.xOffset+drawW && y < drawH){
            indexRecorder.recordPosition(x, y-20, leapVisualizer.getIndexFingertipFromXML(video.getCurrentFrameID()),video.getCurrentFrameID());
            lastIndexVideoPos.set(x,y-20);
            lastIndexLeapPos = leapVisualizer.getIndexFingertipFromXML(video.getCurrentFrameID());
        }
    }
*/
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
}

//--------------------------------------------------------------
void testApp::exit(){
    // let's close down Leap and kill the controller
    leap.close();
}



/// makeTimeStampFbo();
//fboTimeStamp.readToPixels(processFrameImg.getPixelsRef());

/*
void testApp::makeTimeStampFbo(){

    fboTimeStamp.begin();
    ofClear(0,255);
    ofSetColor(255);
    processFrameImg.draw(0,0);

    int cameraNowTime = cameraLibdc.getTimestamp()-cameraRecordTimestampStart;
    cout << cameraNowTime<< " camera time" << endl << endl;;
    ofDrawBitmapString( ofToString(cameraNowTime), 10, cameraHeight-30);

    fboTimeStamp.end();
}

*/
