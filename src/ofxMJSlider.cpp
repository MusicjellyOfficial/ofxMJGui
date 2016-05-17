//
//  ofxMJSlider.cpp
//  MJServer
//
//  Created by Oscar Wyatt on 08/04/2016.
//
//

#include "ofxMJSlider.h"

ofxMJSlider::ofxMJSlider() {
    mode.mode = mode.normal; //defaults to normal mode
}

void ofxMJSlider::setup(float _x, float _y, float _minValue, float _maxValue, bool _visible, float _currentValue, float _size, bool _horizontal) {
    
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
    
    //these have to be in an instance method NOT a constructor
    ofAddListener(ofEvents().mousePressed, this, &ofxMJSlider::mousePressed);
    ofAddListener(ofEvents().mouseReleased, this, &ofxMJSlider::mouseReleased);
    ofAddListener(ofEvents().mouseDragged, this, &ofxMJSlider::mouseDragged);
}

void ofxMJSlider::setupTwin(float _x, float _y, float _minValue, float _maxValue, bool _visible, float _smallerValue, float _largerValue, float size, bool _horizontal ) {
    
    setup(_x, _y, _minValue, _maxValue, _visible, 0, size, _horizontal);
    
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

void ofxMJSlider::setMinMaxPos(float base, float size) {
    dotMinPos = base + dotRadius;
    dotMaxPos = base + size - 2 * dotRadius;
}

void ofxMJSlider::draw() {
    if (visible) {
        if (mode.mode == mode.normal) {
            ofSetColor(30);
            ofDrawRectRounded(x, y, width, height, height / 2);
            ofSetColor(0);
            ofDrawCircle(dotX, dotY, dotRadius);
        } else {
            ofSetColor(30);
            ofDrawRectRounded(x, y, width, height, height / 2);
            ofSetColor(0);
            ofDrawCircle(smallerDotX, smallerDotY, dotRadius);
            ofDrawCircle(largerDotX, largerDotY, dotRadius);
        }
    }
}

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

void ofxMJSlider::mouseReleased(ofMouseEventArgs &e) {
    if (isPressed) {
        isPressed = false;
        dotClicked = -1;
        updateValue();
        updateDotPosition();
    }
}

void ofxMJSlider::setVisibility(bool _visible) {
    visible = _visible;
}

void ofxMJSlider::updateValue() {
    if ( mode.mode == mode.normal ) { //normal mode
        value = getMappedValue(dotX);
        ofNotifyEvent(valueChanged, value);
    } else { //twin mode
        float tempSmaller = (horizontal) ? getMappedValue(smallerDotX) : getMappedValue(smallerDotY);
        float tempLarger = (horizontal) ? getMappedValue(largerDotX) : getMappedValue(largerDotY);
        if ( tempSmaller != smallerValue ) {
            smallerValue = tempSmaller;
            ofNotifyEvent(smallerValueChanged, smallerValue);
        }
        if ( tempLarger != largerValue ) {
            largerValue = tempLarger;
            ofNotifyEvent(largerValueChanged, largerValue);
        }
    }
}

float ofxMJSlider::getValue() {
    return value;
}

void ofxMJSlider::getValues(float& _smallerValue, float& _largerValue) {
    _smallerValue = smallerValue;
    _largerValue = largerValue;
}

void ofxMJSlider::setValues(float _smallerValue, float _largerValue) {
    smallerValue = _smallerValue;
    largerValue = _largerValue;
    updateDotPosition();
}

void ofxMJSlider::setSmallerValue(float _smallerValue) {
    smallerValue = _smallerValue;
    updateDotPosition();
}

void ofxMJSlider::setLargerValue(float _largerValue) {
    largerValue = _largerValue;
    updateDotPosition();
}

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

bool ofxMJSlider::hasBeenClicked(float mouseX, float mouseY) {
    if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height) {
        return true;
    } else {
        return false;
    }
}

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

float ofxMJSlider::getMappedDotPos(float value) {
    return ofMap(value, minValue, maxValue, dotMinPos, dotMaxPos);
}