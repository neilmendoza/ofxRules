#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetFrameRate(60);
    
    light.setPosition(500, 500, 500);
    light.enable();
    
    cam.setTranslationKey('1');
    
    drawGui = true;
    
    selected = 0;
    dir.listDir("");
    dir.sort();
    for (unsigned i = 0; i < dir.size(); ++i)
    {
        fileNames.push_back(dir.getName(i));
    }
}

//--------------------------------------------------------------
void testApp::update()
{
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));
    
    rules.step();
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofSetColor(255, 255, 255);
    
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_DEPTH_TEST);
    
    cam.begin();
    rules.draw();
    cam.end();
    
    glPopAttrib();
    
    if (drawGui)
    {
        ofDisableLighting();
        for (unsigned i = 0; i < fileNames.size(); ++i)
        {
            if (i == selected) ofSetColor(0, 255, 0);
            else ofSetColor(255, 0, 0);
            ofDrawBitmapString(fileNames[i], 10, 20 * (i + 1));
        }
        ofEnableLighting();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    switch (key)
    {
        case OF_KEY_RETURN:
            rules.watchFile(fileNames[selected]);
            break;
        
        case OF_KEY_UP:
            selected = (selected - 1);
            if (selected < 0) selected = fileNames.size() - 1;
            break;
        
        case OF_KEY_DOWN:
            selected = (selected + 1) % fileNames.size();
            break;
            
        case ' ':
            drawGui = !drawGui;
            break;
            
        default:
            break;
    }
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