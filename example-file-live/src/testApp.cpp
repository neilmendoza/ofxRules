#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    ofBackground(0);
    
    cam.setNearClip(1e-2);
    cam.setFarClip(1e4);
    cam.setDistance(80);
    cam.lookAt(ofVec3f(0, 20, 0));

//    mesh.setUsage(GL_DYNAMIC_DRAW);
//    mesh.setMode(OF_PRIMITIVE_LINES);
    
    generator.watchFile("test.xml");
    
    ofDirectory dir;
    dir.listDir("");
    dir.sort();
}

//--------------------------------------------------------------
void testApp::update()
{
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));
    
    generator.step();
}

//--------------------------------------------------------------
void testApp::draw()
{
    cam.begin();
    generator.draw();
    cam.end();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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