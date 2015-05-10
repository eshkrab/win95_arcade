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
int level=0;
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
      
      if(aData) {
        aData->bHit = true;
      }
      
      if(bData) {
        bData->bHit = true;
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
      
      //BULLETS
      for(int i=0; i<bullets.size(); i++) {
        bullets[i].get()->age++;
        if(bullets[i].get()->age > 100){
          bullets.erase(bullets.begin()+i);
        }
      }
      for(int i=0; i<customParticles.size();i++){
        SoundData * sd = (SoundData*)customParticles[i].get()->getData();
        ofLog()<<ofToString();
        if(sd->bHit){
          customParticles[i].get()->hurt(10);
        }
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
//    ofRemove(bullets,bulletDies);
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
      
      for(int i=0; i<customParticles.size(); i++) {
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
  //HARDCODING
  
  switch(lvl){
    case 0:{
      //LEVEL 0
      ofLog()<<"setPlat"+ofToString(lvl);
      windows.push_back(ofPtr<Platform>(new Platform(0, 0)));
      Platform * p = windows.back().get();
      p->x = 7.2;
      p->y = 379;
      p->setPhysics(0, 0, 0);
      p->setup(box2d.getWorld(), p->x, p->y, p->width, p->height);
      
      windows.push_back(ofPtr<Platform>(new Platform(0, 1)));
      Platform * p1= windows.back().get();
      p1->x = 175.7;
      p1->y = 379;
      p1->setPhysics(0, 0, 0);
      p1->setup(box2d.getWorld(), p->x, p->y, p->width, p->height);
      
      windows.push_back(ofPtr<Platform>(new Platform(0, 2)));
      Platform * p2 = windows.back().get();
      p2->x = 381.2;
      p2->y = 379;
      p2->setPhysics(0, 0, 0);
      p2->setup(box2d.getWorld(), p->x, p->y, p->width, p->height);
      
      windows.push_back(ofPtr<Platform>(new Platform(0, 3)));
      Platform * p3= windows.back().get();
      p3->x = 524.2;
      p3->y = 379;
      p3->setPhysics(0, 0, 0);
      p3->setup(box2d.getWorld(), p->x, p->y, p->width, p->height*1.5);
    }
      break;
    case 1:{
      ofLog()<<"setPlat"+ofToString(lvl);
      //LEVEL 0
      windows.push_back(ofPtr<Platform>(new Platform(1, 0)));
      Platform * p = windows.back().get();
      p->x = 669-level*640;
      p->y = 374.;
      p->setPhysics(0, 0, 0);
      p->setup(box2d.getWorld(), p->x, p->y, p->width, p->height);
      
      windows.push_back(ofPtr<Platform>(new Platform(1, 1)));
      Platform * p1= windows.back().get();
      p1->x = 820-level*640;
      p1->y = 374;
      p1->setPhysics(0, 0, 0);
      p1->setup(box2d.getWorld(), p->x, p->y, p->width, p->height);
    
      windows.push_back(ofPtr<Platform>(new Platform(1, 2)));
      Platform * p2 = windows.back().get();
      p2->x = 1111.3-level*640;
      p2->y = 374.4;
      p2->setPhysics(0, 0, 0);
      p2->setup(box2d.getWorld(), p->x, p->y, p->width, p->height);
      
      windows.push_back(ofPtr<Platform>(new Platform(1, 3)));
      Platform * p3= windows.back().get();
      p3->x = 961.6-level*640;
      p3->y = 419.8;
      p3->setPhysics(0, 0, 0);
      p3->setup(box2d.getWorld(), p->x, p->y, p->width, p->height*1.5);
    }
      break;
    case 2:{
      ofLog()<<"setPlat"+ofToString(lvl);
      //LEVEL 0
      windows.push_back(ofPtr<Platform>(new Platform(lvl, 0)));
      Platform * p = windows.back().get();
      p->x = 1280-level*640;
      p->y = 374.;
      p->setPhysics(0, 0, 0);
      p->setup(box2d.getWorld(), p->x, p->y, p->width, p->height);
      
      windows.push_back(ofPtr<Platform>(new Platform(lvl, 1)));
      Platform * p1= windows.back().get();
      p1->x = 1453-level*640;
      p1->y = 374;
      p1->setPhysics(0, 0, 0);
      p1->setup(box2d.getWorld(), p->x, p->y, p->width, p->height);
      
      windows.push_back(ofPtr<Platform>(new Platform(lvl, 2)));
      Platform * p2 = windows.back().get();
      p2->x = 1937-level*640;
      p2->y = 374.4;
      p2->setPhysics(0, 0, 0);
      p2->setup(box2d.getWorld(), p->x, p->y, p->width, p->height);
      
      windows.push_back(ofPtr<Platform>(new Platform(lvl, 3)));
      Platform * p3= windows.back().get();
      p3->x = 2101-level*640;
      p3->y = 438;
      p3->setPhysics(0, 0, 0);
      p3->setup(box2d.getWorld(), p->x, p->y, p->width, p->height*1.5);
    }
      break;
    case 3:{
      ofLog()<<"setPlat"+ofToString(lvl);
      //LEVEL 0
      windows.push_back(ofPtr<Platform>(new Platform(lvl, 0)));
      Platform * p = windows.back().get();
      p->x = 1789-level*640;
      p->y = 370;
      p->setPhysics(0, 0, 0);
      p->setup(box2d.getWorld(), p->x, p->y, p->width, p->height);
      
      windows.push_back(ofPtr<Platform>(new Platform(lvl, 1)));
      Platform * p1= windows.back().get();
      p1->x = 1644-level*640;
      p1->y = 402;
      p1->setPhysics(0, 0, 0);
      p1->setup(box2d.getWorld(), p->x, p->y, p->width, p->height);
    }
      break;
  }
}
void testApp::setLevel(int lvl){
  level = lvl;
  
  switch(level){
    case 0:{
      customParticles.push_back(ofPtr<CustomParticle>(new CustomParticle(1, 0)));
      CustomParticle * p = customParticles.back().get();
      
      float r = ofRandom(3, 10);		// a random radius 4px - 20px
      p->setPhysics(0.4, 0.53, 0.31);
      p->setup(box2d.getWorld(), (int)ofRandom(100,640), 400, p->width, p->height*1.5);
    }
      break;
    case 1:{
      customParticles.push_back(ofPtr<CustomParticle>(new CustomParticle(1, 1)));
      CustomParticle * p = customParticles.back().get();
      
      float r = ofRandom(3, 10);		// a random radius 4px - 20px
      p->setPhysics(0.4, 0.53, 0.31);
      p->setup(box2d.getWorld(), mouseX, mouseY, p->width, p->height);
    }
      
      break;
    case 2:{
      customParticles.push_back(ofPtr<CustomParticle>(new CustomParticle(1, 2)));
      CustomParticle * p = customParticles.back().get();
      
      float r = ofRandom(3, 10);		// a random radius 4px - 20px
      p->setPhysics(0.4, 0.53, 0.31);
      p->setup(box2d.getWorld(), mouseX, mouseY, p->width, p->height);
    }
      
      break;
    case 3:{
      customParticles.push_back(ofPtr<CustomParticle>(new CustomParticle(1, 3)));
      CustomParticle * p = customParticles.back().get();
      
      float r = ofRandom(3, 10);		// a random radius 4px - 20px
      p->setPhysics(0.4, 0.53, 0.31);
      p->setup(box2d.getWorld(), mouseX, mouseY, p->width, p->height);
    }
      break;
      
  }

  
//  windows.clear();
//  setPlat(level);
  
  
//  for(int i=0; i<windows.size(); i++){
//    ofLog()<< ofToString(i)+" b4 "+ofToString(windows[i].get()->getPosition().x);
//    int x = windows[i].get()->getPosition().x - level*640;
//    windows[i].get()->setPosition(x, windows[i].get()->getPosition().y);
//    ofLog()<< "af"+ ofToString(windows[i].get()->getPosition().x);
//  }
}
//--------------------------------------------------------------
void testApp::keyPressed(int key) {

	
	if(key == 'b') {
    customParticles.push_back(ofPtr<CustomParticle>(new CustomParticle(1, (int)ofRandom(4))));
    CustomParticle * p = customParticles.back().get();
    
    float r = ofRandom(3, 10);		// a random radius 4px - 20px
    p->setPhysics(0.4, 0.53, 0.31);
    p->setup(box2d.getWorld(), mouseX, mouseY, p->width, p->height);
	}
//  if(key=='1'){
//    setLevel(0);
//    for(int i=0; i<windows.size(); i++){
//      ofLog()<<"x: "+ofToString(windows[i].get()->x);
//    }
//
//  }
//  if(key=='2'){
//    setLevel(1);
//    for(int i=0; i<windows.size(); i++){
//      ofLog()<<"x: "+ofToString(windows[i].get()->x);
//    }
//  }
//  if(key=='3'){
//    setLevel(2);
//    for(int i=0; i<windows.size(); i++){
//      ofLog()<<"x: "+ofToString(windows[i].get()->x);
//    }
//  }
//  if(key=='4'){
//    setLevel(3);
//    for(int i=0; i<windows.size(); i++){
//      ofLog()<<"x: "+ofToString(windows[i].get()->x);
//    }
//  }

//	if(key == 'w') {
//      windows.push_back(ofPtr<Platform>(new Platform(3, 0)));
//      Platform * p = windows.back().get();
//      p->x = 1789-level*640;
//      p->y = 370;
//      p->setPhysics(0, 0, 0);
//      p->setup(box2d.getWorld(), p->x, p->y, p->width, p->height);
//      
//      windows.push_back(ofPtr<Platform>(new Platform(3, 1)));
//      Platform * p1= windows.back().get();
//      p1->x = 1644-level*640;
//      p1->y = 402;
//      p1->setPhysics(0, 0, 0);
//      p1->setup(box2d.getWorld(), p->x, p->y, p->width, p->height);
//
//	}
//  if(key=='o'){
//    for (int i = 0; i < (int)outlines.size(); i++){
//      ofPolyline & line = outlines[i];
//      ofPoint pt = line.getPointAtLength(0);
//      
//      ofLog()<< ofToString(pt.x)+",  "+ofToString(pt.y);
//      
//      
//      windows.push_back(ofPtr<Plat form>(new Platform(i/4, i%4)));
//      Platform * p = windows.back().get();
//      p->x =(int)pt.x;
//      p->y =(int)pt.y;
//      p->setPhysics(0, 0, 0);
//      p->setup(box2d.getWorld(), p->x, p->y, p->width, p->height*1.5);
//      
//    }
//  }
	
	if(key == 'p') {
    mode=PLAY;
    
//    
//    
//    windows.push_back(ofPtr<Platform>(new Platform(3, 0)));
//    Platform * p = windows.back().get();
//    p->x = 0;
//    p->y = 370;
//    p->setPhysics(0, 0, 0);
//    p->setup(box2d.getWorld(), p->x, p->y, p->width, p->height);
//    
//    windows.push_back(ofPtr<Platform>(new Platform(3, 1)));
//    Platform * p1= windows.back().get();
//    p1->x = p->width;
//    p1->y = 402;
//    p1->setPhysics(0, 0, 0);
//    p1->setup(box2d.getWorld(), p->x, p->y, p->width, p->height);
//    
//    
//    
//    //setLevel(0);
//    for(int i=0; i<windows.size(); i++){
//      ofLog()<<"x: "+ofToString(windows[i].get()->x);
//    }
    
		customParticles.push_back(ofPtr<CustomParticle>(new CustomParticle(0, 1)));
    CustomParticle * p5 = customParticles.back().get();
    
    p5->setPhysics(0.4, 0.53, 0.31);
    p5->setup(box2d.getWorld(), 10, 300, p5->width, p5->height*1.5);
    
    SoundData * s = new SoundData();
    ofLog()<<ofToString(s);
    p5->setData(s);
    ofLog()<<ofToString(p5->getData());
    SoundData * sd = (SoundData*)p5->getData();
    sd->type = GUY;
    sd->bHit	= false;
    

    
    setLevel(0);
		
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
      p->addForce(ofVec2f(1,0.8), 25);
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

