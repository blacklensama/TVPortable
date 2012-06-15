//
//  Layer.cpp
//  TVPortable
//
//  Created by Ruiwei Bu on 6/14/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#include "Layer.h"

namespace TVPortable {
    
    namespace Visual {
        
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
                                                             CCDirector::sharedDirector()->getWinSize().height - (mClippingRect.origin.y + getPosition().y),
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
                    case CCMouse::Release:
                    case CCMouse::Wheel:
                    case CCMouse::Drag:
                    case CCMouse::Move:
                        printf("Mouse down !!!!!, with eve %d\n", evt.state);
                        break;
                }
                return true;
            }
            return false;
        }
        
        bool Layer::onKeyEvent(const cocos2d::CCKeyEvent& evt) {
            if(mFocused) {
                switch(evt.state) {
                    
                }
            }
            return false;
        }
        
        bool Layer::onJoystickEvent(const cocos2d::CCJoyStickEvent& evt) {
            return false;
        }
        
        void Layer::adjustGamma(float rgamma, int rfloor, int rceil, float ggamma, int gfloow, int gceil, float bgamma, int bfloor, int bceil)  {
            
        }
        
        void Layer::bringToFront() {
            
        }
        
        void Layer::bringToBack() {
            
        }
        
        void Layer::colorRect(float left, float right, float width, float height, uint32 color, int opacity) {
            
        }
        
        void Layer::doBoxBlur(float xblur/* = 1*/, float yblur/* = 1*/) {
            
        }
        
        void Layer::doGrayScale() {
            
        }
        
        void Layer::drawText(float x, float y, const char* text, uint32 color, int opacity/* = 255*/, bool aa/* = true*/, int shadowlevel/* = 0*/, uint32 shadowColor/*=0x00000000*/, float shadowWidth/*= 0*/, float shadowfsx/* = 0*/, float shadowfsy/* = 0*/) {
            
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
            
        }
        
        uint32 Layer::getMainPixel(int x, int y) {
            
        }
        
        uint32 Layer::getMaskPixel(int x, int y) {
            
        }
        
        uint32 Layer::getProvincePixel(int x, int y) {
            
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
        
        void Layer::saveImage(const char* name, const char* type/* = "bmp"*/) {
            
        }
        
        void Layer::setAttentionPos(float left, float top) {
            
        }
        
        void Layer::setClip(float x, float y, float width, float height) {
            
        }
        
        void Layer::setCursorPos(float x, float y) {
            
        }
        
        void Layer::setImageSize(float width, float height) {
            
        }
        
        void Layer::setMainPixel(int x, int y, uint32 color) {
            
        }
        
        void Layer::setMaskPixel(int x, int y, uint32 value) {
            
        }
        
        void Layer::setMode() {
            
        }
        
        void Layer::setPos(float left, float top, float width, float height) {
            
        }
        
        void Layer::setProvincePixel(int x, int y, uint32 value) {
            
        }
        
        void Layer::setSize(float width, float height) {
            
        }
        
        void Layer::setSizeToImageSize() {
            
        }
        
        void Layer::stopTransition() {
            
        }
        
        void Layer::update(float left, float top, float width, float height) {
            
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
        
        void Layer::onKeyDown(uint8 key, int shift, bool process/* = true*/) {
            
        }
        
        void Layer::onKeyPress(uint8 key, bool process/* = true*/) {
            
        }
        
        void Layer::onKeyUp(uint8 key, int shift, bool process/* = true*/) {
            
        }
        
        void Layer::onMouseDown(float x, float y, int btn, int shift) {
            
        }
        
        void Layer::onMouseEnter() {
            
        }
        
        void Layer::onMouseLeave() {
            
        }
        
        void Layer::onMouseMove(float x, float y, int shift) {
            
        }
        
        void Layer::onMouseUp(float x, float y, int button, int shift) {
            
        }
        
        void Layer::onMouseWheel(int shift, int delta, float x, float y) {
            
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
        
    }
    
}
