//
//  ofxMJButton.h
//  MJServer
//
//  Created by Oscar Wyatt on 31/03/2016.
//
//

#ifndef ofxMJButton_h
#define ofxMJButton_h


//#include <stdio.h>
//#include "ofMain.h"

#include "ofxMJGui.h"

#include "ofGraphics.h"
#include "ofParameter.h"
#include "ofConstants.h"
#include "ofBaseTypes.h"


#include "ofImage.h"
#include "ofTrueTypeFont.h"

using namespace std;

const string CIRCLE = "CIRCLE";
const string RECTANGLE = "RECTANGLE";

const string TEXT = "TEXT";
const string IMG = "IMG";

const string LRG = "LRG";
const string SML = "SML";

static float defaultRadius = 30;
static float smallRadius = 15;
static float smallYMargin = 5;
static float smallYSpace = smallRadius + 2 * smallYMargin;

template<typename Type>
class ofxMJButton {
public:
    
    

    
    //METHODS
    ofxMJButton();
    ~ofxMJButton();

//    Type operator=(Type v);
//    operator const Type & ();
    
    template<class ListenerClass, typename ListenerMethod>
    void addListener(ListenerClass * listener, ListenerMethod method){
        ofAddListener(triggerEvent, listener, method);
    }


    void setup(string _shape, string _contentType, string _contentStr, string _textSize, float _x, float _y, float _widthOrRadius, float _height, bool _visible);
    void setReleasedFillColor(ofColor color);
    void setColors(ofColor _pressedColor, ofColor _releasedColor, ofColor _pressedFillColor, ofColor _releasedFillColor);
    void setStrokeColors(ofColor _pressedStrokeColor, ofColor _releasedStrokeColor);
    
    void setSwitchMode(bool _switchMode, bool initialValue, ofColor onColor, ofColor offColor, ofColor onFillColor, ofColor offFillColor);

    float draw(float yOffset = 0);

    void setShowDetail(string text);
    void setDetailMode(bool _detailMode, string text);

    void mousePressed(ofMouseEventArgs& e);
    void mouseReleased(ofMouseEventArgs& e);
    void mouseDragged(ofMouseEventArgs& e){ }
    void mouseMoved(ofMouseEventArgs& e){ }
    void mouseScrolled(ofMouseEventArgs& e){ }
    void mouseEntered(ofMouseEventArgs& e){ }
    void mouseExited(ofMouseEventArgs& e){ }
    
    virtual void handleMouseReleased();

    void setVisible(bool _visible);
    void setSwitchValue(bool _value);

    float getX();
    float getY();
    
    void setValue(Type _value);
    
private:
    ofParameter<Type> value;
    ofEvent<Type> triggerEvent;
    
    
    bool hasBeenClicked(float mouseX, float mouseY);
    
    float yOffset = 0; //stores yOffset from when it was last drawn for comparision when checking if it has been drawn
    
    float contentX;
    float contentY;
    
    string shape = RECTANGLE;
    string contentType = TEXT;
    
    float radius = defaultRadius; //default
    float width;
    float height;
    
    int libraryIndex;
    
    ofImage img;
    
    string text;
    string detailText = "";
    string textSize;
    ofTrueTypeFont helvetica;
    
    //to indicate that it has been held down
    bool isPressed = false;
    
    bool visible = true;
    
    bool switchMode = false;
    bool switchValue = false;
    //detail mode
    bool detailMode = false;
    bool showDetail = false;
    float detailTextWidth;
    float detailWidth;
    float detailHeight;

    float x;
    float y;
    
    //defaults
    ofColor pressedColor = ofColor::white;
    ofColor releasedColor = ofColor(30);
    ofColor pressedStrokeColor = ofColor::darkGray;
    ofColor releasedStrokeColor = ofColor::white;
    ofColor pressedFillColor = ofColor::darkGray;
    ofColor releasedFillColor = ofColor::white;
};

typedef ofxMJButton<bool> ofxMJBoolButton;
typedef ofxMJButton<int> ofxMJIntButton;


#endif /* ofxMJButton_h */
