#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxSVG.h"

#define N_SOUNDS 1
#define BULLET 0
#define HERO 3
#define GUY 1
#define BIGGUY 2
#define BADBULLET 4

class SoundData {
public:
  bool bHit;
  int ID;
  int type;
};

class Platform : public ofxBox2dRect {
  
public:
  Platform(int n1, int n2) {
    img.loadImage("plat_"+ofToString(n1)+"_"+ofToString(n2)+".gif");
    level = n1;
    if(n1!= 2){
      height= img.getHeight()*0.35;
      width = img.getWidth()*0.35;
      img.resize(width, height);
    }
    
    height= img.getHeight();
    width = img.getWidth();
    
  }
  int level;
  float x, y;
  ofImage img;
  int height;
  int width;

  
  void draw() {
    glPushMatrix();
    glTranslatef(getPosition().x, getPosition().y, 0);
    ofSetColor(255,255,255);
    img.draw(-width/2,-height/2);
    //ofRect(0, 0, width, height);
    glPopMatrix();
    
  }
};


class Bullet : public ofxBox2dRect {
  
public:
  Bullet() {
    cur.loadImage("cursor.gif");
    height= cur.getHeight()*0.05;
    width = cur.getWidth()*0.05;
    cur.resize(width, height);
    cur.rotate90(1);
    height= cur.getHeight();
    width = cur.getWidth();
    age = 0;
    
  }
  ofImage cur;
  int height;
  int width;
  int age;
  
  void draw() {
    glPushMatrix();
    glTranslatef(getPosition().x, getPosition().y, 0);
    ofSetColor(255,255,255);
    cur.draw(-width/2,-height/2);
    glPopMatrix();
  }
};
// ------------------------------------------------- a simple extended box2d circle
class CustomParticle : public ofxBox2dRect {
	
public:
	CustomParticle(int n, int t, int health) {

//    SoundData * sd = (SoundData*)getData();
//    sd->type = GUY;
//    sd->bHit	= false;
    hp=health;
    type=t;
    if(!n){
      cur.loadImage("hero.gif");
      cur.resize(cur.getWidth()*0.05, cur.getHeight()*0.05);
      height= cur.getHeight();
      width = cur.getWidth();
    }else{
      cur.loadImage("guy_"+ofToString(t)+".gif");
      cur.resize(cur.getWidth(), cur.getHeight());
      height= cur.getHeight();
      width = cur.getWidth();
      
    }
	}
  ofImage cur;
  ofColor color;
  int type;
  int hp;

  
  int height;
  int width;

  void update(){
    
  }

	void draw() {
		glPushMatrix();
		glTranslatef(getPosition().x, getPosition().y, 0);
        ofSetColor(255,255,255);
        if(color.r != 0) ofSetColor(color);
        cur.draw(-width/2,-height/2);
		glPopMatrix();
		
	}
  
  void hurt(int dmg){
    hp -= dmg;
    ofLog()<<"HURT"+ofToString(hp);
  }
  void jump(){
    addForce(ofVec2f(0, 3), 300);
  }
};


// -------------------------------------------------
class testApp : public ofBaseApp {
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void resized(int w, int h);
  bool bulletDies(Bullet &bul);
  bool isLevelPlat(ofPtr<Platform> bul);
  void setLevel(int lvl);
  
  // this is the function for contacts
  void contactStart(ofxBox2dContactArgs &e);
  void contactEnd(ofxBox2dContactArgs &e);
  
  // when the ball hits we play this sound
  ofSoundPlayer  sound[N_SOUNDS];


  void setPlat(int lel);
	
	float                                   px, py;
	bool                                    bDrawLines;
	bool                                    bMouseForce;
  
  ofxSVG svg;
  vector<ofPolyline> outlines;
	
	ofxBox2d                                box2d;			  //	the box2d world
	ofPolyline                              drawing;		  //	we draw with this first
	ofxBox2dEdge                            edgeLine;		  //	the box2d edge/line shape (min 2 points)
	vector		<ofPtr<ofxBox2dCircle> >	circles;		  //	default box2d circles
	vector		<ofPtr<ofxBox2dRect> >		boxes;			  //	defalut box2d rects
	vector		<ofPtr<CustomParticle> >    customParticles;  //	this is a custom particle the extends a cirlce
  vector		<ofPtr<Bullet> >          bullets;
  vector		<ofPtr<Platform> >          windows;
    
	
	
};
