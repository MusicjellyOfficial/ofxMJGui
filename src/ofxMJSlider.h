//
//  ofxMJSlider.h
//  MJServer
//
//  Created by Oscar Wyatt on 08/04/2016.
//
//

#ifndef ofxMJSlider_h
#define ofxMJSlider_h

#include "ofMain.h"
#include <stdio.h>

struct ofxMJSliderMode {
    const std::string normal = "NORMAL";
    const std::string twin = "TWIN";
    std::string mode = normal;
};

class ofxMJSlider {
public:
    ofxMJSlider();
    //normal mode
    void setup(float _x, float _y, float _minValue, float _maxValue, bool _visible, float _currentValue, float size, bool _horizontal);
    // twin mode
    void setupTwin(float _x, float _y, float _minValue, float _maxValue, bool _visible, float _smallerValue, float _largerValue, float size, bool _horizontal);
    void setMinMaxPos(float base, float size);
    
    void draw();
    
    ofEvent<float>valueChanged;
    ofEvent<float>smallerValueChanged;
    ofEvent<float>largerValueChanged;
    
    //events
    void mouseDragged(ofMouseEventArgs& e);
    void mousePressed(ofMouseEventArgs& e);
    void mouseReleased(ofMouseEventArgs& e);
    
    void setVisibility(bool _isVisible);
    
    //value
    void updateValue();
    float getValue();
    
    void getValues(float& _smallerValue, float& _largerValue);
    void setValues(float smallerValue, float largerValue);
    void setSmallerValue(float _smallerValue);
    void setLargerValue(float _largerValue);
private:
    void updateDotPosition();
    bool hasBeenClicked(float mouseX, float mouseY);
    int dotHasBeenClicked(float mouseX, float mouseY);
    float getNewDotPos(float newPos, float minPos, float maxPos);
    float getMappedValue(float value);
    float getMappedDotPos(float value);
    
    int category;
    float x;
    float y;
    float width = 3;
    float height = 3;
    float minValue = 0;
    float maxValue = 100;
    float value = 0;
    bool horizontal = true;
    
    //twin mode
    float smallerValue = 0;
    float largerValue = 0;
    float smallerDotX = 0;
    float largerDotX = 0;
    float smallerDotY = 0;
    float largerDotY = 0;
    int dotClicked = -1;
    
    float dotX;
    float dotY;
    float dotMargin = 5; //margin between dot and the bounds of the slider
    float dotRadius = 10;
    
    float dotMinPos;
    float dotMaxPos;
    
    bool isPressed = false;
    bool visible = false;
    
    ofxMJSliderMode mode;
    
};

#endif /* ofxMJSlider_h */