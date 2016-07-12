//
//  ofxMJSlider.cpp
//  MJServer
//
//  Created by Oscar Wyatt on 08/04/2016.
//
//

#include "ofxMJSlider.h"


//--------------------------------------------------------------
ofxMJSlider::ofxMJSlider() {
    mode.mode = mode.normal; //defaults to normal mode
}


//--------------------------------------------------------------
void ofxMJSlider::setup(int _ID, float _x, float _y, float _minValue, float _maxValue, bool _visible, float _currentValue, float _size, bool _horizontal) {
    
    ID = _ID;
    
    x = _x;
    y = _y;
    
    minValue = _minValue;
    maxValue = _maxValue;
    
    value = _currentValue;
    
    horizontal = _horizontal;
    if ( horizontal ) {
        width = _size;
        //set dot values based on value passed to it
        setMinMaxPos(x, width);
        dotX = getMappedDotPos(value);
        dotY = (y + height / 2);
    } else {
        height = _size;
        //set dot values based on value passed to it
        setMinMaxPos(y, height);
        dotX = (x + width / 2);
        dotY = getMappedDotPos(value);
    }
    
    visible = _visible;
    
    ofRegisterMouseEvents(this, OF_EVENT_ORDER_BEFORE_APP);
}


//--------------------------------------------------------------
void ofxMJSlider::setupTwin(int _ID, float _x, float _y, float _minValue, float _maxValue, bool _visible, float _smallerValue, float _largerValue, float size, bool _horizontal ) {
    
    setup(_ID, _x, _y, _minValue, _maxValue, _visible, 0, size, _horizontal);
    
    mode.mode = mode.twin;
    
    smallerValue = _smallerValue;
    largerValue = _largerValue;
    
    if ( horizontal ) {
        smallerDotX = getMappedDotPos(smallerValue);
        smallerDotY = y + height / 2;
        largerDotX = getMappedDotPos(largerValue);
        largerDotY = y + height / 2;
    } else {
        smallerDotX = x + width / 2;
        smallerDotY = getMappedDotPos(smallerValue);
        largerDotX = x + width / 2;
        largerDotY = getMappedDotPos(largerValue);
    }
    
}


//--------------------------------------------------------------
void ofxMJSlider::setMinMaxPos(float base, float size) {
    dotMinPos = base + dotRadius;
    dotMaxPos = base + size - dotRadius;
}


//--------------------------------------------------------------
void ofxMJSlider::draw() {
    if (visible) {
        if (mode.mode == mode.normal) {
            ofSetColor(255);
            ofDrawRectRounded(x, y, width, height, height / 2);
            ofSetColor(0);
            ofDrawCircle(dotX, dotY, dotRadius);
            if( dotImage.isAllocated() ){
                ofSetColor(255);
                dotImage.draw(dotX, dotY);
            }
        } else {
            ofSetColor(30);
            ofDrawRectRounded(x, y, width, height, height / 2);
            ofSetColor(255);
            ofDrawCircle(smallerDotX, smallerDotY, dotRadius);
            ofDrawCircle(largerDotX, largerDotY, dotRadius);
        }
    }
}


//--------------------------------------------------------------
void ofxMJSlider::mouseDragged(ofMouseEventArgs &e) {
    if (isPressed) {
        if ( mode.mode == mode.normal ) {
            //normal mode
            float mousePos = (horizontal) ? e.x : e.y;
            float& dotVar = (horizontal) ? dotX : dotY;
            dotVar = getNewDotPos(mousePos, dotMinPos, dotMaxPos);
            updateValue();
        } else {
            //twin mode
            float mousePos = (horizontal) ? e.x : e.y;
            float& smallerPos = (horizontal) ? smallerDotX : smallerDotY;
            float& largerPos = (horizontal) ? largerDotX : largerDotY;
            if ( dotClicked == 0 ) { //if clicked on smaller dot
                smallerPos = getNewDotPos(mousePos, dotMinPos, largerPos);
            } else if ( dotClicked == 1 ) { //if clicked on larger dot
                largerPos = getNewDotPos(mousePos, smallerPos, dotMaxPos);
            }
            updateValue();
        }
        
    }
}


//--------------------------------------------------------------
float ofxMJSlider::getNewDotPos(float newPos, float minPos, float maxPos) {
    float result = 0;
    if ( newPos >= minPos && newPos <= maxPos ) {
        result = newPos;
    } else if( newPos < minPos ){
        result = minPos;
    } else if ( newPos > maxPos ) {
        result = maxPos;
    }
    return result;
}


//--------------------------------------------------------------
void ofxMJSlider::mousePressed(ofMouseEventArgs& e) {
    //in normal mode, as long as the user has clicked anywhere on the slider the dot will move to that point
    //in twin mode, the user has to explicitly click on a dot
    if (visible) {
        if (mode.mode == mode.normal) {
            if ( hasBeenClicked(e.x, e.y) ) {
                isPressed = true;
                mouseDragged(e);
            }
        } else {
            dotClicked = dotHasBeenClicked(e.x, e.y);
            if (dotClicked > -1) { //return val of -1 indicates no dot has been clicked, so one has been clicked
                isPressed = true;
            }
        }
    }
}


//--------------------------------------------------------------
void ofxMJSlider::mouseReleased(ofMouseEventArgs &e) {
    if (isPressed) {
        isPressed = false;
        dotClicked = -1;
        updateValue();
        updateDotPosition();
    }
}


//--------------------------------------------------------------
void ofxMJSlider::setVisibility(bool _visible) {
    visible = _visible;
}


//--------------------------------------------------------------
bool ofxMJSlider::setImage(string path){
    bool result = dotImage.load(path);
    
    if(result){
        dotImage.resize(dotRadius * 1.4, dotRadius * 1.4);
        dotImage.setAnchorPoint(dotRadius * 0.7, dotRadius * 0.7);
    }
    
    return result;
}

//--------------------------------------------------------------
void ofxMJSlider::updateValue() {
    if ( mode.mode == mode.normal ) { //normal mode
        
        value = getMappedValue(dotX);

    } else { //twin mode
        
        smallerValue = (horizontal) ? getMappedValue(smallerDotX) : getMappedValue(smallerDotY);
        largerValue = (horizontal) ? getMappedValue(largerDotX) : getMappedValue(largerDotY);

    }
    ofNotifyEvent(triggerEvent, ID);
}


//--------------------------------------------------------------
float ofxMJSlider::getValue() {
    return value;
}


//--------------------------------------------------------------
void ofxMJSlider::getValues(float& _smallerValue, float& _largerValue) {
    _smallerValue = smallerValue;
    _largerValue = largerValue;
}


//--------------------------------------------------------------
void ofxMJSlider::setValues(float _smallerValue, float _largerValue) {
    smallerValue = _smallerValue;
    largerValue = _largerValue;
    updateDotPosition();
}


//--------------------------------------------------------------
void ofxMJSlider::setSmallerValue(float _smallerValue) {
    smallerValue = _smallerValue;
    updateDotPosition();
}


//--------------------------------------------------------------
void ofxMJSlider::setLargerValue(float _largerValue) {
    largerValue = _largerValue;
    updateDotPosition();
}


//--------------------------------------------------------------
void ofxMJSlider::setMinValue(float _minValue) {
	minValue = _minValue;
	updateDotPosition();
}


//--------------------------------------------------------------
void ofxMJSlider::setMaxValue(float _maxValue) {
	maxValue = _maxValue;
	updateDotPosition();
}


//--------------------------------------------------------------
void ofxMJSlider::updateDotPosition() {
    if ( mode.mode == mode.normal ) {
        value = getMappedDotPos(value);
    } else {
        if ( horizontal ) {
            smallerDotX = getMappedDotPos(smallerValue);
            largerDotX = getMappedDotPos(largerValue);
        } else {
            smallerDotY = getMappedDotPos(smallerValue);
            largerDotY = getMappedDotPos(largerValue);
        }
    }
}


//--------------------------------------------------------------
bool ofxMJSlider::hasBeenClicked(float mouseX, float mouseY) {
    if (mouseX >= x && mouseX <= x + width && mouseY >= y - dotRadius && mouseY <= y + dotRadius) {
        return true;
    } else {
        return false;
    }
}


//--------------------------------------------------------------
int ofxMJSlider::dotHasBeenClicked(float mouseX, float mouseY) {
    int result = -1; //-1 indicates neither has been clicked
    float smallerDist = ofDist(mouseX, mouseY, smallerDotX, smallerDotY);
    float largerDist = ofDist(mouseX, mouseY, largerDotX, largerDotY);
    if (smallerDist <= dotRadius) {
        result = 0; // 0  indicates smaller has been clicked
    }
    if (largerDist <= dotRadius) {
        result = 1;
    }
    return result;
}


//--------------------------------------------------------------
float ofxMJSlider::getMappedValue(float value) {
    //if ( mode.mode == mode.normal ) {
    return ofMap(value, dotMinPos, dotMaxPos, minValue, maxValue);
    //} else {
    //	if ( dotClicked == 0 ){ //if its the smaller dot
    //		//float& largerDotPos = (horizontal) ? largerDotX : largerDotY;
    //		return ofMap(value, dotMinPos, dotMaxPos, minValue, maxValue);
    //	} else {
    
    //	}
    //}
    
}


//--------------------------------------------------------------
float ofxMJSlider::getMappedDotPos(float value) {
    return ofMap(value, minValue, maxValue, dotMinPos, dotMaxPos);
}