#include "testApp.h"

#include "Rule.h"
#include "LineAction.h"
#include "PointAction.h"

namespace itg
{
    //--------------------------------------------------------------
    void testApp::setup()
    {
        ofBackground(0);
        
        cam.setNearClip(1e-2);
        cam.setFarClip(1e4);
        cam.setDistance(80);
        cam.lookAt(ofVec3f(0, 20, 0));
       
        structure.setMaxDepth(300);
        
        {
            Rule::Ptr rule = structure.addRule("test", 100);
            
            LineAction::Ptr action = rule->addAction<LineAction>("test");
            action->translate(0, 0.1, 0);
            action->rotate(1, 0, 0);
        }
       
        {
            Rule::Ptr rule = structure.addRule("test", 100);
            
            LineAction::Ptr action = rule->addAction<LineAction>("test");
            action->translate(0, 0.1, 0);
            action->rotate(0, 20, 0);
        }
        
        {
            Rule::Ptr rule = structure.addRule("test", 100);
            
            LineAction::Ptr action = rule->addAction<LineAction>("test");
            action->translate(0, 0.4, 0);
            action->rotate(1, 0, -2);
        }
        
        {
            Rule::Ptr rule = structure.addRule("test", 6);
            
            TransformAction::Ptr left = rule->addAction<TransformAction>("test");
            left->rotate(0, 180, 0);
            
            TransformAction::Ptr right = rule->addAction<TransformAction>("test");
            left->rotate(15, 0, 0);
        }
       
        structure.addBranch("test");
        
        mesh.setUsage(GL_DYNAMIC_DRAW);
        mesh.setMode(OF_PRIMITIVE_LINES);
    }

    //--------------------------------------------------------------
    void testApp::update()
    {
        ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));
        
        structure.step(mesh);
    }

    //--------------------------------------------------------------
    void testApp::draw()
    {
        //svg.draw();
        cam.begin();
        mesh.draw();
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
}