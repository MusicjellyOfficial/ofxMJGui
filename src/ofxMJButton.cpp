//
//  ofxMJButton.cpp
//  MJServer
//
//  Created by Oscar Wyatt on 31/03/2016.
//
//

#include "ofxMJButton.h"


//--------------------------------------------------------------
template<typename Type>
ofxMJButton<Type>::ofxMJButton(){}


//--------------------------------------------------------------
template<typename Type>
ofxMJButton<Type>::~ofxMJButton(){}


////--------------------------------------------------------------
//template<typename Type>
//Type ofxMJButton<Type>::operator=(Type v){
//    value = v;
//    return v;
//}
//
//
////--------------------------------------------------------------
//template<class ListenerClass, typename ListenerMethod>
//template<typename Type>
//Type ofxMJButton<Type>::operator const Type & (){
//    return value;
//}


//--------------------------------------------------------------
template<typename Type>
void ofxMJButton<Type>::setup(string _shape, string _contentType, string _contentStr, string _textSize, float _x, float _y, float _widthOrRadius, float _height, bool _visible){
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

    ofRegisterMouseEvents(this, OF_EVENT_ORDER_BEFORE_APP);
}


//--------------------------------------------------------------
template<typename Type>
void ofxMJButton<Type>::setReleasedFillColor(ofColor color){
    releasedFillColor = color;
}


//--------------------------------------------------------------
template<typename Type>
void ofxMJButton<Type>::setColors(ofColor _pressedColor, ofColor _releasedColor, ofColor _pressedFillColor, ofColor _releasedFillColor){
    pressedColor = _pressedColor;
    releasedColor = _releasedColor;
    pressedFillColor = _pressedFillColor;
    releasedFillColor = _releasedFillColor;
}


//--------------------------------------------------------------
template<typename Type>
void ofxMJButton<Type>::setStrokeColors(ofColor _pressedStrokeColor, ofColor _releasedStrokeColor){
    pressedStrokeColor = _pressedStrokeColor;
    releasedStrokeColor = _releasedStrokeColor;
}


//--------------------------------------------------------------
template<typename Type>
void ofxMJButton<Type>::setSwitchMode(bool _switchMode, bool initialValue, ofColor onColor, ofColor offColor, ofColor onFillColor, ofColor offFillColor){
    switchMode = _switchMode;
    switchValue = initialValue;
    pressedColor = onColor;
    releasedColor = offColor;
    pressedFillColor = onFillColor;
    releasedFillColor = offFillColor;
}


//--------------------------------------------------------------
//@brief: set the button to show some detail text rather than behave like a normal button
//at the moment only works if button is rectangle
template<typename Type>
void ofxMJButton<Type>::setDetailMode(bool _detailMode, string text){
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


//--------------------------------------------------------------
//@brief: draw the button. Returns any extra y offset that should be sent to any further buttons. It will pass on any value it gets unless it should be greater
template<typename Type>
float ofxMJButton<Type>::draw(float _yOffset){
    if(visible){
        yOffset = _yOffset;
        //update yOffset for comparision when checking if it has been drawn

        //if we're showing detail, we need different dimensions
        float drawWidth = (showDetail) ? detailWidth : width;
        float drawHeight = (showDetail) ? detailHeight : height;


        if(shape == CIRCLE){
            ofFill();
            
            if(isPressed || (switchMode && switchValue) ){
                ofSetColor(pressedFillColor);
            } else {
                ofSetColor(releasedFillColor);
            }
            
            ofDrawCircle(x, y + yOffset, radius);
            //first draw the filled circle
            
            ofNoFill();
            
            if(isPressed || (switchMode && switchValue) ){
                ofSetColor(pressedStrokeColor);
            } else {
                ofSetColor(releasedStrokeColor);
            }
            
            ofDrawCircle(x, y + yOffset, radius);
            //second draw an unfilled circle which has a stroke color
            
            
        } else {
            ofFill();
            
            if(isPressed || (switchMode && switchValue) ){
                ofSetColor(pressedFillColor);
            } else {
                ofSetColor(releasedFillColor);
            }
            
            ofDrawRectRounded(x, y + yOffset, drawWidth, drawHeight, 3);
            //first draw the filled rect
            
            
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


//--------------------------------------------------------------
template<typename Type>
void ofxMJButton<Type>::mousePressed(ofMouseEventArgs& e){
    if( visible && hasBeenClicked(e.x, e.y) ){
        isPressed = true;
    } else {
        isPressed = false;
    }
}


//--------------------------------------------------------------
template<typename Type>
void ofxMJButton<Type>::mouseReleased(ofMouseEventArgs& e){
    isPressed = false;
    if( visible && hasBeenClicked(e.x, e.y) ){
        if( detailMode ){
            showDetail = !showDetail;
        } else {
            //if it's in detail mode don't trigger any notifications
//            if(switchMode) switchValue = !switchValue;
//            value = !value;
            handleMouseReleased();
        }
    }
}


//--------------------------------------------------------------
template<typename Type>
void ofxMJButton<Type>::handleMouseReleased(){
    Type val = value.get();
    ofNotifyEvent(triggerEvent, val );
}



//--------------------------------------------------------------
template<typename Type>
void ofxMJButton<Type>::setVisible(bool _visible){
    visible = _visible;
}


//--------------------------------------------------------------
template<typename Type>
void ofxMJButton<Type>::setSwitchValue(bool _value){
    switchValue = _value;
}


//--------------------------------------------------------------
template<typename Type>
float ofxMJButton<Type>::getX(){
    return x;
}


//--------------------------------------------------------------
template<typename Type>
float ofxMJButton<Type>::getY(){
    return y;
}


//--------------------------------------------------------------
template<typename Type>
void ofxMJButton<Type>::setValue( Type _value){
    value = _value;
}


////--------------------------------------------------------------
//template<typename Type>
//float ofxMJButton<Type>::setBoolValue( bool value){
//    return y;
//}

template class ofxMJButton<bool>;
template class ofxMJButton<int>;





//--------------------------------------------------------------
//  PRIVATE
//--------------------------------------------------------------
#pragma MARK - PRIVATE


//--------------------------------------------------------------
template<typename Type>
bool ofxMJButton<Type>::hasBeenClicked(float mouseX, float mouseY){
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