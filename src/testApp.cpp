#include "testApp.h"

#define IDLE 0
#define START 1
#define PLAY 2
#define BOSS 3
#define WON 4
#define LOST 5

static int pts[] = {257,219,257,258,259,274,263,325,266,345,266,352,269,369,276,387,286,415,291,425,302,451,308,462,316,472,321,480,328,488,333,495,339,501,345,505,350,507,365,515,370,519,377,522,382,525,388,527,405,534,426,538,439,539,452,539,468,540,485,540,496,541,607,541,618,539,625,537,641,530,666,513,682,500,710,476,723,463,727,457,729,453,732,450,734,447,738,440,746,423,756,404,772,363,779,343,781,339,784,327,789,301,792,278,794,267,794,257,795,250,795,232,796,222,796,197,797,195,797,188,796,188};
static int nPts  = 61*2;


ofImage screen;
int mode=0;
//--------------------------------------------------------------
void testApp::setup() {
  screen.loadImage("bg.png");
  ofSetVerticalSync(true);
  ofSetLogLevel(OF_LOG_NOTICE);
  ofBackground(0);
  ofSetColor(255);
	
	bMouseForce = false;
	
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.createGround();
	box2d.setFPS(30.0);
	box2d.registerGrabbing();
	
}
int testApp::bulletDies(ofPtr<Bullet> bul){
  return !(bul.get()->age< 10000);
}

//--------------------------------------------------------------
void testApp::update() {
	
	
	
	box2d.update();
  
  switch(mode){
    case PLAY:

      //BULLETS
      for(int i=0; i<bullets.size(); i++) {
        bullets[i].get()->age++;
      }
      break;
  }

	
	
	if(bMouseForce) {
		float strength = 8.0f;
		float damping  = 0.7f;
		float minDis   = 100;
		for(int i=0; i<circles.size(); i++) {
			circles[i].get()->addAttractionPoint(mouseX, mouseY, strength);
			circles[i].get()->setDamping(damping, damping);
		}
		for(int i=0; i<customParticles.size(); i++) {
			customParticles[i].get()->addAttractionPoint(mouseX, mouseY, strength);
			customParticles[i].get()->setDamping(damping, damping);
		}
		
	}
	
    // remove shapes offscreen
    ofRemove(bullets, ofxBox2dBaseShape::shouldRemoveOffScreen);
    ofRemove(customParticles, ofxBox2dBaseShape::shouldRemoveOffScreen);
    
    
}


//--------------------------------------------------------------
void testApp::draw() {
  ofBackground(0);
  ofSetColor(255);
  screen.draw(0,0,ofGetWidth(), ofGetHeight());
  
  
	
	for(int i=0; i<circles.size(); i++) {
		ofFill();
		ofSetHexColor(0x90d4e3);
		circles[i].get()->draw();
	}
	
  switch(mode){
    case PLAY:
      
//      hero.draw();
//      
//      for(int i=0; i<platforms.size(); i++) {
//        platforms[i].get()->draw();
//      }
//      
//      for(int i=0; i<badguys.size(); i++) {
//        badguys[i].get()->draw();
//      }
      
      break;
  }
  
	for(int i=0; i<boxes.size(); i++) {
		ofFill();
		ofSetHexColor(0xe63b8b);
		boxes[i].get()->draw();
	}
	
	for(int i=0; i<customParticles.size(); i++) {
		customParticles[i].get()->draw();
	}
  
  for(int i=0; i<bullets.size(); i++) {
    bullets[i].get()->draw();
  }
	
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {

	
	if(key == 'b') {
    customParticles.push_back(ofPtr<CustomParticle>(new CustomParticle(0)));
    CustomParticle * p = customParticles.back().get();
    
    float r = ofRandom(3, 10);		// a random radius 4px - 20px
    p->setPhysics(0.4, 0.53, 0.31);
    p->setup(box2d.getWorld(), mouseX, mouseY, p->width, p->height);
	}
	
	if(key == 'p') {
    mode=PLAY;
		customParticles.push_back(ofPtr<CustomParticle>(new CustomParticle(0)));
    CustomParticle * p = customParticles.back().get();
    
    p->setPhysics(0.4, 0.53, 0.31);
		p->setup(box2d.getWorld(), 50, 300, p->width, p->height*1.5);
		
	}
  if(key == ' ') {
    if(mode == PLAY){
    //customParticles[0]->jump();
      customParticles[0]->addForce(ofVec2f(0, 7), 100);
    }
    
  }

  if(key == OF_KEY_RIGHT) {
    if(mode == PLAY){
      customParticles[0]->addForce(ofVec2f(1,0), 100);
    }
  }
  if(key == OF_KEY_LEFT) {
    if(mode == PLAY){
      customParticles[0]->addForce(ofVec2f(-1,0), 100);
    }
  }
  if(key == OF_KEY_RETURN) {
    if(mode == PLAY){
      ofLog()<<"SHOOTING";
      bullets.push_back(ofPtr<Bullet>(new Bullet));
      Bullet * p = bullets.back().get();
      
      p->setPhysics(0.4, 0.53, 0.31);
      p->setup(box2d.getWorld(), customParticles[0]->getPosition().x, customParticles[0]->getPosition().y-10, p->width, p->height);
      p->addForce(customParticles[0]->getVelocity(), 100);
      p->addForce(ofVec2f(1,0.8), 50);
    }
    
  }

	if(key == 'f') bMouseForce = !bMouseForce;
	if(key == 't') ofToggleFullscreen();
	
	
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
	drawing.addVertex(x, y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	
	if(edgeLine.isBody()) {
		drawing.clear();
		edgeLine.destroy();
	}
	
	drawing.addVertex(x, y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
	
	drawing.setClosed(false);
	drawing.simplify();
	
	edgeLine.addVertexes(drawing);
	//polyLine.simplifyToMaxVerts(); // this is based on the max box2d verts
	edgeLine.setPhysics(0.0, 0.5, 0.5);
	edgeLine.create(box2d.getWorld());
		
	drawing.clear();

}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
	
}

