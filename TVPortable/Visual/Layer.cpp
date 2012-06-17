//
//  Layer.cpp
//  TVPortable
//
//  Created by Ruiwei Bu on 6/14/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#include "Layer.h"

#ifdef _MSC_VER
#include <GLES/gl.h>

#pragma comment(lib, "libgles_cm.lib")
#endif

TVP_NS_VISUAL_BEGIN
        
    using namespace cocos2d;

    Layer::Layer() {
        CCInputDispatcher::Instance().addListener(this, 1);
    }

    Layer::~Layer() {
        
    }

    void Layer::visit() {
        // quick return if not visible
        if (!getIsVisible()) {
            return;
        }
        
        glPushMatrix(); 
        glEnable(GL_SCISSOR_TEST);
        
        /*
         convert origin point to upper-left and set scissor rect
         */
        CCEGLView::sharedOpenGLView().setScissorInPoints(mClippingRect.origin.x + getPosition().x, 
                                                         CCDirector::sharedDirector()->getWinSize().height - (mClippingRect.origin.y + getPosition().y) - mClippingRect.size.height,
                                                         mClippingRect.size.width,
                                                         mClippingRect.size.height);
        
        CCNode::visit();
        
        glDisable(GL_SCISSOR_TEST);
        glPopMatrix();
    }

    Layer* Layer::LayerWithClippingRect(const cocos2d::CCRect& rc) {
        Layer* layer = new Layer();
        if(layer) {
            layer->setClippingRect(rc);
            layer->autorelease();
            return layer;
        }
        
        return NULL;
    }

    bool Layer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
        
        if(mFocused) {
            onMouseDown(pTouch->locationInView().x, 
                        pTouch->locationInView().y, 
                        mbLeft,
                        0);
            return true;
        }
        return false;
    }

    void Layer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
        if(mFocused) {
            onMouseMove(pTouch->locationInView().x,
                        pTouch->locationInView().y,
                        0);
        }
    }

    void Layer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
        if(mFocused) {
            onMouseUp(pTouch->locationInView().x, 
                      pTouch->locationInView().y,
                      mbRight,
                      0);
        }
    }

    void Layer::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
        
    }

    void Layer::draw() {
        CCNode::draw();
    }

    void Layer::onEnter() {
        
    }

    void Layer::onExit() {
        
    }

    bool Layer::onMouseEvent(const cocos2d::CCMouseEvent& evt) {
        if(mFocused) {
            switch(evt.state) {
                case CCMouse::Enter:
                case CCMouse::Leave:
                case CCMouse::Press:
                    
                    printf("Mouse down !!!!!, with button %d\n", evt.button);
                    break;
                    
                case CCMouse::Release:
                    printf("Mouse Release !!!!!, with button %d\n", evt.button);
                    break;
                    
                case CCMouse::Wheel:
                    printf("Mouse Wheel !!!!!, with wheel %d\n", evt.wheel);
                    break;
                    
                case CCMouse::Drag:
                case CCMouse::Move:
                    break;
            }
            return true;
        }
        return false;
    }

    bool Layer::onKeyEvent(const cocos2d::CCKeyEvent& evt) {
        if(mFocused) {
            switch(evt.state) {
                case CCKey::Press:
                    printf("Key down !!!!!, with button %d, char: %c\n", evt.key, evt.toCharCode());
                    break;
                    
                case CCKey::Release:
                    printf("Key Up !!!!!, with button %d, char: %c\n", evt.key, evt.toCharCode());
                    break;
            }
        }
        return false;
    }

    bool Layer::onJoystickEvent(const cocos2d::CCJoyStickEvent& evt) {
        return false;
    }

    void Layer::adjustGamma(float rgamma, int32 rfloor, int32 rceil, float ggamma, int32 gfloow, int32 gceil, float bgamma, int32 bfloor, int32 bceil)  {
        
    }

    void Layer::bringToFront() {
        
    }

    void Layer::bringToBack() {
        
    }

    void Layer::colorRect(float left, float right, float width, float height, uint32 color, int32 opacity) {
        
    }

    void Layer::doBoxBlur(float xblur/* = 1*/, float yblur/* = 1*/) {
        
    }

    void Layer::doGrayScale() {
        
    }

    void Layer::drawText(float x, float y, const char* text, uint32 color, int32 opacity/* = 255*/, bool aa/* = true*/, int32 shadowlevel/* = 0*/, uint32 shadowColor/*=0x00000000*/, float shadowWidth/*= 0*/, float shadowfsx/* = 0*/, float shadowfsy/* = 0*/) {
        
    }

    void Layer::fillRect(float left, float top, float width, float height, uint32 color) {
        
    }

    void Layer::flipLR() {
        
    }

    void Layer::flipUD() {
        
    }

    void Layer::focus(bool direction/* = true*/) {
        onBeforeFocus(this, false, direction);
        
        mFocused = true;
        
        onFocus(mFocused, direction);
    }

    void Layer::focusNext() {
        
    }

    void Layer::focusPrev() {
        
    }

    Layer* Layer::getLayerAt(float x, float y, bool exclude_self/* = false*/, bool get_disabled/* = false*/) {
        return 0;
    }

    uint32 Layer::getMainPixel(int32 x, int32 y) {
        return 0;
    }

    uint32 Layer::getMaskPixel(int32 x, int32 y) {
        return 0;
    }

    uint32 Layer::getProvincePixel(int32 x, int32 y) {
        return 0;
    }

    void Layer::independMainImage(bool copy/* = true*/) {
        
    }

    void Layer::independProvinceImage(bool copy/* = true*/) {
        
    }

    void Layer::loadImages(const char* image, uint32 color) {
        
    }

    void Layer::loadProvinceImage(const char* image) {
        
    }

    void Layer::moveBefore(Layer* layer) {
        
    }

    void Layer::moveBehind(Layer* layer) {
        
    }

    void Layer::releaseCapture() {
        
    }

    void Layer::removeMode() {
        
    }

    void Layer::saveLayerImage(const char* name, const char* type/* = "bmp"*/) {
        
    }

    void Layer::setAttentionPos(int32 left, int32 top) {
        
    }

    void Layer::setClip(int32 x, int32 y, int32 width, int32 height) {
        
    }

    void Layer::setCursorPos(int32 x, int32 y) {
        
    }

    void Layer::setImageSize(int32 width, int32 height) {
        
    }

    void Layer::setMainPixel(int32 x, int32 y, uint32 color) {
        
    }

    void Layer::setMaskPixel(int32 x, int32 y, uint32 value) {
        
    }

    void Layer::setMode() {
        
    }

    void Layer::setPos(int32 left, int32 top, int32 width, int32 height) {
        
    }

    void Layer::setProvincePixel(int32 x, int32 y, uint32 value) {
        
    }

    void Layer::setSize(int32 width, int32 height) {
        
    }

    void Layer::setSizeToImageSize() {
        
    }

    void Layer::stopTransition() {
        
    }

    void Layer::update(int32 left, int32 top, int32 width, int32 height) {
        
    }

    void Layer::onBeforeFocus(Layer* layer, bool blurred, bool direction) {
        
    }

    void Layer::onBlur(bool focused) {
        
    }

    void Layer::onClick(float x, float y) {
        
    }

    void Layer::onDoubleClick(float x, float y) {
        
    }

    void Layer::onFocus(bool focused, bool direction) {
        
    }

    void Layer::onHitTest(float x, float y, bool hit) {
        
    }

    void Layer::onKeyDown(uint8 key, int32 shift, bool process/* = true*/) {
        
    }

    void Layer::onKeyPress(uint8 key, bool process/* = true*/) {
        
    }

    void Layer::onKeyUp(uint8 key, int32 shift, bool process/* = true*/) {
        
    }

    void Layer::onMouseDown(float x, float y, int32 btn, int32 shift) {
        
    }

    void Layer::onMouseEnter() {
        
    }

    void Layer::onMouseLeave() {
        
    }

    void Layer::onMouseMove(float x, float y, int32 shift) {
        
    }

    void Layer::onMouseUp(float x, float y, int32 button, int32 shift) {
        
    }

    void Layer::onMouseWheel(int32 shift, int32 delta, float x, float y) {
        
    }

    void Layer::onNodeDisabled() {
        
    }

    void Layer::onNodeEnabled() {
        
    }

    void Layer::onPaint() {
        
    }

    void Layer::onSearchNextFocusable(Layer* layer) {
        
    }

    void Layer::onSearchPrevFocusable(Layer* layer) {
        
    }

    void Layer::onTransitionCompleted(Layer* dest, Layer* src) {
        
    }

TVP_NS_VISUAL_END