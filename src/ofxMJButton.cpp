//
//  ofxMJButton.cpp
//  MJServer
//
//  Created byOscar Wyatt on 31/03/2016.
//
//

#include "ofxMJButton.h"

//default constructor
ofxMJButton::ofxMJButton(){}

void ofxMJButton::setup(string _shape, string _contentType, string _contentStr, string _textSize, float _x, float _y, float _widthOrRadius, float _height, bool _visible){
    shape = _shape;
    contentType = _contentType;
    visible = _visible;
    
    x = _x;
    y = _y;
    
    if(shape == CIRCLE){
        radius = _widthOrRadius;
    } else {
        width = _widthOrRadius;
        height = _height;
    }
    
    if(contentType == IMG){
        img.load(_contentStr);
        if(shape == CIRCLE){
            img.resize(radius, radius);
        } else {
            img.resize(width, height);
        }
        
        float imgWidth = img.getWidth();
        float imgHeight = img.getHeight();
        contentX = x + imgWidth / 2;
        contentY = y + imgHeight / 2;
    } else {
        text = _contentStr;
        textSize = _textSize;
        
        if(textSize == SML){
            helvetica.load("helvetica.otf", 12);
        } else {
            helvetica.load("helvetica.otf", 28);
        }
        
        
        float textWidth = helvetica.stringWidth(text);
        float textHeight = helvetica.stringHeight(text);
        
        if(shape == CIRCLE){
            //don't have to increase x or y because they are the centre of the circle
            contentX = x - textWidth / 2;
            contentY = y + textHeight / 2;
        } else {
            contentX = (x + width / 2) - textWidth / 2;
            contentY = (y + height / 2) + textHeight / 2;
        }
    }
    //these have to be in an instance method NOT a constructor
    ofAddListener(ofEvents().mousePressed, this, &ofxMJButton::mousePressed);
    ofAddListener(ofEvents().mouseReleased, this, &ofxMJButton::mouseReleased);
}


void ofxMJButton::setReleasedFillColor(ofColor color){
    releasedFillColor = color;
}


void ofxMJButton::setColors(ofColor _pressedColor, ofColor _releasedColor, ofColor _pressedFillColor, ofColor _releasedFillColor){
    pressedColor = _pressedColor;
    releasedColor = _releasedColor;
    pressedFillColor = _pressedFillColor;
    releasedFillColor = _releasedFillColor;
}

void ofxMJButton::setSwitchMode(bool _switchMode, bool initialValue, ofColor onColor, ofColor offColor, ofColor onFillColor, ofColor offFillColor){
    switchMode = _switchMode;
    switchValue = initialValue;
    pressedColor = onColor;
    releasedColor = offColor;
    pressedFillColor = onFillColor;
    releasedFillColor = offFillColor;
}

//@brief: set the button to show some detail text rather than behave like a normal button
//at the moment only works if button is rectangle
void ofxMJButton::setDetailMode(bool _detailMode, string text){
    if(shape == RECTANGLE){
        detailMode = _detailMode;
        detailText = text;
        detailTextWidth = helvetica.stringWidth(detailText);
        detailWidth = detailTextWidth;
        if(detailTextWidth < width){
            detailWidth = width;
        }
        
        //for some reason, without the +20 the button is slightly too small
        detailHeight = helvetica.stringHeight(detailText) + 20;
    }
}

//@brief: draw the button. Returns any extra y offset that should be sent to any further buttons. It will pass on any value it gets unless it should be greater
float ofxMJButton::draw(float _yOffset){
    if(visible){
        yOffset = _yOffset;
        //update yOffset for comparision when checking if it has been drawn

        //if we're showing detail, we need different dimensions
        float drawWidth = (showDetail) ? detailWidth : width;
        float drawHeight = (showDetail) ? detailHeight : height;
        
        if(isPressed || (switchMode && switchValue) ){
            ofSetColor(pressedFillColor);
        } else {
            ofSetColor(releasedFillColor);
        }

        if(shape == CIRCLE){
            ofNoFill();
            ofDrawCircle(x, y + yOffset, radius);
        } else {
            ofFill();
            ofDrawRectRounded(x, y + yOffset, drawWidth, drawHeight, 3);
        }
        
        if(isPressed || (switchMode && switchValue) ){
            ofSetColor(pressedColor);
        } else {
            ofSetColor(releasedColor);
        }
        
        //if it has been set to show detail text, show it
        if(detailMode && showDetail){
            float tempHeight = height + detailHeight;
            float tempX = x + (detailWidth - detailTextWidth) / 2;
            
            helvetica.drawString(detailText, tempX, contentY + _yOffset);
            
            //here we update the passed in yOffset that is normally ignored
            _yOffset += tempHeight;
            
        } else {
            //otherwise, behave as normal
            if(contentType == TEXT){
                helvetica.drawString(text, contentX, contentY + yOffset);
            } else {
                img.draw(contentX, contentY + yOffset, width, height);
            }
        }
    }
    return _yOffset;
}



void ofxMJButton::mousePressed(ofMouseEventArgs& e){
    if( hasBeenClicked(e.x, e.y) ){
        isPressed = true;
    } else {
        isPressed = false;
    }
}

void ofxMJButton::mouseReleased(ofMouseEventArgs& e){
    isPressed = false;
    if( hasBeenClicked(e.x, e.y) ){
        if( detailMode ){
            showDetail = !showDetail;
        } else {
            //if it's in detail mode don't trigger any notifications
            if(switchMode) switchValue = !switchValue;
            handleMouseReleased();
        }
        
    }
}

void ofxMJButton::handleMouseReleased(){
    int foo = 0;
    ofNotifyEvent(wasReleased, foo, this);
}


bool ofxMJButton::hasBeenClicked(float mouseX, float mouseY){
    bool result = false;
    if(visible){
        if(shape == CIRCLE){
            float dist = ofDist(x, y, mouseX, mouseY);
            if( abs(dist) <= radius){
                result = true;
            }
        } else {
            float centreX = x + width / 2;
            float centreY = y + yOffset + height / 2;
            
            //if the detail is being shown, the button will be of different dimensions so we need to reflect that
            if(showDetail){
                centreX = x + detailWidth / 2;
                centreY = y + yOffset + detailHeight / 2;
            }
            
            float xDist = abs(centreX - mouseX);
            float yDist = abs(centreY - mouseY);
            
            float compWidth = (showDetail) ? detailWidth : width;
            float compHeight = (showDetail) ? detailHeight : height;
            
            if(xDist <= compWidth / 2 && yDist <= compHeight / 2){
                result = true;
            }
        }
    }
    
    return result;
}

void ofxMJButton::setVisible(bool _visibile){
    visible = _visibile;
}

void ofxMJButton::setSwitchValue(bool value){
    switchValue = value;
}

//these are fairly self explanatory
float ofxMJButton::getX(){
    return x;
}

float ofxMJButton::getY(){
    return y;
}