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
int mode=START;
int level=0;
int new_lvl=0;
int idx=0;
ofVec2f movePt;
//--------------------------------------------------------------
void testApp::setup() {
  
  screen.loadImage("bg.png");
  ofSetVerticalSync(true);
  ofSetLogLevel(OF_LOG_NOTICE);
  ofBackground(0);
  ofSetColor(255);
	
  //SVG
  svg.load("lay.svg");
  for (int i = 0; i < svg.getNumPath(); i++){
    ofPath p = svg.getPathAt(i);
    p.setPolyWindingMode(OF_POLY_WINDING_ODD);
    vector<ofPolyline>& lines = p.getOutline();
    for(int j=0;j<(int)lines.size();j++){
      outlines.push_back(lines[j].getResampledBySpacing(1));
    }
  }
  

  
	bMouseForce = false;
	
	box2d.init();
    box2d.enableEvents();   // <-- turn on the event listener
	box2d.setGravity(0, 10);
	box2d.createGround();
	box2d.setFPS(30.0);
	box2d.registerGrabbing();
  
  ofAddListener(box2d.contactStartEvents, this, &testApp::contactStart);
  ofAddListener(box2d.contactEndEvents, this, &testApp::contactEnd);
  
  for (int i=0; i<N_SOUNDS; i++) {
    sound[i].loadSound(ofToString(i)+".mp3");
    sound[i].setMultiPlay(true);
    sound[i].setLoop(false);
  }
  
  edgeLine.addVertex(0, 0);
  edgeLine.addVertex(0, 440);
  edgeLine.addVertex(640, 440);

  edgeLine.setPhysics(0.0, 0.5, 0.5);
  edgeLine.create(box2d.getWorld());
  
  
  
	
}
bool testApp::bulletDies(Bullet &bul){
  return (bul.age > 10000);
}

bool testApp::isLevelPlat(ofPtr<Platform> bul){
  return !(bul.get()->level != level);
}

//--------------------------------------------------------------
void testApp::contactStart(ofxBox2dContactArgs &e) {
  if(e.a != NULL && e.b != NULL) {
    
    // if we collide with the ground we do not
    // want to play a sound. this is how you do that
    if(e.a->GetType() == b2Shape::e_polygon && e.b->GetType() == b2Shape::e_polygon) {
      
//      ofLog()<<ofToString(e.a->GetUserData());
      SoundData * aData = (SoundData*)e.a->GetBody()->GetUserData();
      SoundData * bData = (SoundData*)e.b->GetBody()->GetUserData();
      if(aData->type == GUY && bData->type == HERO){
        aData->bHit = true;
      }
      if(aData->type == HERO && bData->type == GUY){
        bData->bHit = true;
      }
      
      if(aData->type == BIGGUY && bData->type == HERO){
        aData->bHit = true;
      }
      if(aData->type == HERO && bData->type == BIGGUY){
        bData->bHit = true;
      }
      
      if(aData->type == BULLET && bData->type == GUY){
        bData->bHit = true;
      }
      if(aData->type == GUY && bData->type == BULLET){
        aData->bHit = true;
      }
      if(aData->type == BADBULLET && bData->type == GUY){
        bData->bHit = true;
      }
      if(aData->type == GUY && bData->type == BADBULLET){
        aData->bHit = true;
      }
      
    }
  }
}

//--------------------------------------------------------------
void testApp::contactEnd(ofxBox2dContactArgs &e) {
  if(e.a != NULL && e.b != NULL) {
    
    SoundData * aData = (SoundData*)e.a->GetBody()->GetUserData();
    SoundData * bData = (SoundData*)e.b->GetBody()->GetUserData();
    
    if(aData) {
      aData->bHit = false;
    }
    
    if(bData) {
      bData->bHit = false;
    }
  }
}



//--------------------------------------------------------------
void testApp::update() {

	
	
	box2d.update();
  
  switch(mode){
    case PLAY:
      
      //BOSS BATTLE
      if(level==4){
        idx++;
        if(idx%(int)ofRandom(20, 70)){
          customParticles[1].get()->addAttractionPoint(movePt, -4.0f);
          movePt.x=(int)ofRandom(100,620);
          movePt.y=(int)ofRandom(10,400);
          customParticles[1].get()->addAttractionPoint(movePt, 4.0f);
        }
      }
      
      //BULLETS
      for(int i=0; i<bullets.size(); i++) {
        bullets[i].get()->age++;
        if(bullets[i].get()->age > 100){
          bullets.erase(bullets.begin()+i);
        }
      }
      
      
      for(int i=0; i<customParticles.size();i++){
        CustomParticle * p = customParticles[i].get();
        if(i==0){
          if(p->getPosition().x > 630){
            p->setVelocity(-p->getVelocity().x, p->getVelocity().y);
          }
        }
        SoundData * sd = (SoundData*)p->getData();
        if(sd->bHit){
          customParticles[i].get()->hurt(10);
        }
        if(p->hp < 0 && i!= 0){
          p->addForce(ofVec2f(2,1), 40);
          

        }
        if(customParticles.size()<2){
          if(level<4){
            level++;
            setLevel(level);
          }
        }
      }
      break;
  }

	
    // remove shapes offscreen
    ofRemove(bullets, ofxBox2dBaseShape::shouldRemoveOffScreen);
//    ofRemove(bullets,bulletDies);
    ofRemove(customParticles, ofxBox2dBaseShape::shouldRemoveOffScreen);
    
    
}


//--------------------------------------------------------------
void testApp::draw() {
  ofBackground(0);
  ofSetColor(255);
  screen.draw(0,0,ofGetWidth(), ofGetHeight());
  
	
  switch(mode){
      
    case PLAY:
      
      for(int i=0; i<customParticles.size();i++){
        SoundData * sd = (SoundData*)customParticles[i].get()->getData();
        if(sd->bHit){
          customParticles[i].get()->color.r = 150;
          customParticles[i].get()->color.g = 0;
          customParticles[i].get()->color.b = 0;
        }else{
          customParticles[i].get()->color.r = 0;
          customParticles[i].get()->color.g = 0;
          customParticles[i].get()->color.b = 0;
          
        }
          customParticles[i].get()->draw();
      }
      
      for(int i=0; i<bullets.size(); i++) {
      
        bullets[i].get()->draw();
      }

      for(int i=0; i<windows.size(); i++) {
        windows[i].get()->draw();
      }
      
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
  
	
	
}
void testApp::setPlat(int lvl){
 
}
void testApp::setLevel(int lvl){
  ofLog()<<"NEW LEVEL" +ofToString(level)+" "+ofToString(lvl);
  switch(lvl){
    case 0:{
      customParticles.push_back(ofPtr<CustomParticle>(new CustomParticle(1, 0, 100)));
      CustomParticle * p = customParticles.back().get();
      
      float r = ofRandom(3, 10);		// a random radius 4px - 20px
      p->setPhysics(0.4, 0.53, 0.31);
      p->setup(box2d.getWorld(), (int)ofRandom(100,600), 400, p->width, p->height);
      
      
      SoundData * s = new SoundData();
      ofLog()<<ofToString(s);
      p->setData(s);
      ofLog()<<ofToString(p->getData());
      SoundData * sd = (SoundData*)p->getData();
      sd->type = GUY;
      sd->bHit	= false;
      
      
    }
      break;
    case 1:{
      customParticles.push_back(ofPtr<CustomParticle>(new CustomParticle(1, 1, 150)));
      CustomParticle * p = customParticles.back().get();
      
      float r = ofRandom(3, 10);		// a random radius 4px - 20px
      p->setPhysics(0.4, 0.53, 0.31);
      p->setup(box2d.getWorld(), (int)ofRandom(100,600), 300, p->width, p->height);
      
      
      SoundData * s = new SoundData();
      ofLog()<<ofToString(s);
      p->setData(s);
      ofLog()<<ofToString(p->getData());
      SoundData * sd = (SoundData*)p->getData();
      sd->type = GUY;
      sd->bHit	= false;
    }
      
      break;
    case 2:{
      customParticles.push_back(ofPtr<CustomParticle>(new CustomParticle(1, 2, 200)));
      CustomParticle * p = customParticles.back().get();
      
      float r = ofRandom(3, 10);		// a random radius 4px - 20px
      p->setPhysics(0.4, 0.53, 0.31);
      p->setup(box2d.getWorld(),(int)ofRandom(100,600), 400, p->width, p->height);
      
      
      SoundData * s = new SoundData();
      ofLog()<<ofToString(s);
      p->setData(s);
      ofLog()<<ofToString(p->getData());
      SoundData * sd = (SoundData*)p->getData();
      sd->type = GUY;
      sd->bHit	= false;
    }
      
      break;
    case 3:{
      customParticles.push_back(ofPtr<CustomParticle>(new CustomParticle(1, 3, 250)));
      CustomParticle * p = customParticles.back().get();
      
      float r = ofRandom(3, 10);		// a random radius 4px - 20px
      p->setPhysics(0.4, 0.53, 0.31);
      p->setup(box2d.getWorld(), mouseX, mouseY, p->width, p->height);
      
      
      SoundData * s = new SoundData();
      ofLog()<<ofToString(s);
      p->setData(s);
      ofLog()<<ofToString(p->getData());
      SoundData * sd = (SoundData*)p->getData();
      sd->type = GUY;
      sd->bHit	= false;
    }
      break;
    case 4:{
      customParticles.push_back(ofPtr<CustomParticle>(new CustomParticle(1, 4, 500)));
      CustomParticle * p = customParticles.back().get();
      movePt.x=(int)ofRandom(100,600);
      movePt.y=(int)ofRandom(10,400);
      
      float r = ofRandom(3, 10);		// a random radius 4px - 20px
      p->setPhysics(0, 0.53, 0.31);
      p->setup(box2d.getWorld(), movePt.x, movePt.y, p->width, p->height);
      
      
      SoundData * s = new SoundData();
      ofLog()<<ofToString(s);
      p->setData(s);
      ofLog()<<ofToString(p->getData());
      SoundData * sd = (SoundData*)p->getData();
      sd->type = BIGGUY;
      sd->bHit	= false;
    }
      break;

      
  }

  
}
//--------------------------------------------------------------
void testApp::keyPressed(int key) {

	
//	if(key == 'b') {
//    customParticles.push_back(ofPtr<CustomParticle>(new CustomParticle(1, (int)ofRandom(4))));
//    CustomParticle * p = customParticles.back().get();
//    
//    float r = ofRandom(3, 10);		// a random radius 4px - 20px
//    p->setPhysics(0.4, 0.53, 0.31);
//    p->setup(box2d.getWorld(), mouseX, mouseY, p->width, p->height);
//	}
	
	if(key == 'p') {
    if(mode==START){
      mode=PLAY;
      
      customParticles.push_back(ofPtr<CustomParticle>(new CustomParticle(0, 1, 10000)));
      CustomParticle * p5 = customParticles.back().get();
      
      p5->setPhysics(0.4, 0.53, 0.31);
      p5->setup(box2d.getWorld(), 10, 350, p5->width, p5->height);
      
      SoundData * s = new SoundData();
      ofLog()<<ofToString(s);
      p5->setData(s);
      ofLog()<<ofToString(p5->getData());
      SoundData * sd = (SoundData*)p5->getData();
      sd->type = HERO;
      sd->bHit	= false;
      

      
      setLevel(0);
    }
		
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
      bullets.push_back(ofPtr<Bullet>(new Bullet));
      Bullet * p = bullets.back().get();
      
      p->setPhysics(0.4, 0.53, 0.31);
      p->setup(box2d.getWorld(), customParticles[0]->getPosition().x, customParticles[0]->getPosition().y-10, p->width, p->height);
      p->addForce(customParticles[0]->getVelocity(), 100);
      p->addForce(ofVec2f(1,0.8), 25);
      if(level==4){
        p->addAttractionPoint(customParticles[1].get()->getPosition(), 10.0f);
      }
      
      SoundData * s = new SoundData();
      ofLog()<<ofToString(s);
      p->setData(s);
      ofLog()<<ofToString(p->getData());
      SoundData * sd = (SoundData*)p->getData();
      sd->type = BULLET;
      sd->bHit	= false;

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
  ofLog()<<ofToString(mouseX)+", "+ofToString(mouseY);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
	
}

