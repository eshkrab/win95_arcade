#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"

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
    
  }
  ofImage cur;
  int height;
  int width;
  
  void draw() {
    glPushMatrix();
    glTranslatef(getPosition().x, getPosition().y, 0);
    ofSetColor(255,255,255);
    cur.draw(0,0);
    glPopMatrix();
  }
};
// ------------------------------------------------- a simple extended box2d circle
class CustomParticle : public ofxBox2dRect {
	
public:
	CustomParticle(int n) {
    if(!n){
      cur.loadImage("hero.gif");
      cur.resize(cur.getWidth()*0.05, cur.getHeight()*0.05);
      height= cur.getHeight();
      width = cur.getWidth();
    }else{
      cur.loadImage("guy_"+ofToString(int(ofRandom(4)))+".gif");
      cur.resize(cur.getWidth(), cur.getHeight());
      height= cur.getHeight();
      width = cur.getWidth();
      
    }
	}
  ofImage cur;
  int name;
  int type;
  int hp;
  int state;

  
  int height;
  int width;

  void update(){
    
  }

	void draw() {
		glPushMatrix();
		glTranslatef(getPosition().x, getPosition().y, 0);
        ofSetColor(255,255,255);
        cur.draw(0,0);
		glPopMatrix();
		
	}
  
  void hurt(int dmg){
    hp -= dmg;
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
	
	float                                   px, py;
	bool                                    bDrawLines;
	bool                                    bMouseForce;
	
	ofxBox2d                                box2d;			  //	the box2d world
	ofPolyline                              drawing;		  //	we draw with this first
	ofxBox2dEdge                            edgeLine;		  //	the box2d edge/line shape (min 2 points)
	vector		<ofPtr<ofxBox2dCircle> >	circles;		  //	default box2d circles
	vector		<ofPtr<ofxBox2dRect> >		boxes;			  //	defalut box2d rects
	vector		<ofPtr<CustomParticle> >    customParticles;  //	this is a custom particle the extends a cirlce
  vector		<ofPtr<Bullet> >          bullets;
    
	
	
};
