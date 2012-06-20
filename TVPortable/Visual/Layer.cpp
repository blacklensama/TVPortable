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

#include "MenuItem.h"
#include "Window.h"

#include "ukn/Color.h"

TVP_NS_VISUAL_BEGIN
        
    using namespace cocos2d;

    struct LayerOrderComparer {
        bool operator()(Layer* l1, Layer* l2) {
            int order1 = l1->getOrder();
            int order2 = l2->getOrder();
            
            return order1 < order2;
        }
    };

    Layer::Layer(Window* window, Layer* parent):
    mImage(0),
    mCallOnPaint(false),
    mAbsoluteOrderMode(false),
    mAttentionLeft(0),
    mAttentionTop(0),
    mCursor(0),
    mCursorX(0),
    mCursorY(0),
    mEnabled(true),
    mFace(0),
    mFocusable(true),
    mHasImage(false),
    mParentLayer(parent),
    mFocused(false),
    mCached(false),
    mFont(0),
    mRenderLayer(0),
    mWindow(window) {
        CCInputDispatcher::Instance().addListener(this, 1);
        
        setClippingRect(CCRect(0,
                               0,
                               window->getWidth(),
                               window->getHeight()));
        setPos(0, 0, window->getWidth(), window->getHeight());
        
        if(parent) {
            parent->addChild(this);
        }
    }

    Layer::~Layer() {
        
    }

    CCRect Layer::_getClippingRect() {
        CCRect realRect = CCRectMake(mClippingRect.origin.x + getPosition().x, 
                                     mWindow->getHeight() - (mClippingRect.origin.y + getPosition().y) - mClippingRect.size.height,
                                     mClippingRect.size.width,
                                     mClippingRect.size.height);
        if(getParentLayer() != NULL) {
            CCRect parentRect = getParentLayer()->getClippingRect();
            
            if(realRect.origin.x < parentRect.origin.x) {
                realRect.size.width -= parentRect.origin.x - realRect.origin.x;
                realRect.origin.x = parentRect.origin.x;
            }
            else if(realRect.origin.x > parentRect.origin.x + parentRect.size.width) {
                realRect.size.width = 0;
            } else {
                if(realRect.origin.x + realRect.size.width > parentRect.origin.x + parentRect.size.width) {
                    realRect.size.width = (parentRect.origin.x + parentRect.size.width) - realRect.origin.x;
                }
            }
            
            if(realRect.origin.y < parentRect.origin.y) {
                realRect.size.height -= parentRect.origin.y - realRect.origin.y;
                realRect.origin.y = parentRect.origin.y;
            }
            else if(realRect.origin.y > parentRect.origin.y + parentRect.size.height) {
                realRect.size.height = 0;
            } else {
                if(realRect.origin.y + realRect.size.height > parentRect.origin.y + parentRect.size.height) {
                    realRect.size.height = (parentRect.origin.y + parentRect.size.height) - realRect.origin.y;
                }
            }
            
        }
        return realRect;
    }

    void Layer::visit() {
        // quick return if not visible
        if (!getIsVisible()) {
            return;
        }
        
        glPushMatrix(); 
        glEnable(GL_SCISSOR_TEST);
        
        CCRect clippingRect = _getClippingRect();
        /*
         convert origin point to upper-left and set scissor rect
         */
        CCEGLView::sharedOpenGLView().setScissorInPoints(clippingRect.origin.x,
                                                         clippingRect.origin.y,
                                                         clippingRect.size.width,
                                                         clippingRect.size.height);
        
        if(getParentLayer() == NULL) {
            draw();
        }
        
        glDisable(GL_SCISSOR_TEST);
        glPopMatrix();
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
        // draw children recursively
        if(!mChildren.empty()) {
            ChildList::iterator it = mChildren.end();
            for(; it != mChildren.begin(); --it) {
                (*it)->draw();
            }
        }
        
        if(mDirty) {
            mRenderLayer->begin();
        
            mImage->draw();
        
            if(!mChildren.empty()) {
                ChildList::iterator it = mChildren.end();
                for(; it != mChildren.begin(); --it) {
                    (*it)->getRenderLayer()->getSprite()->draw();
                }   
            }
            
            mRenderLayer->end();
            mDirty = false;
        }
        
        mRenderLayer->getSprite()->setFlipY(true);
        mRenderLayer->getSprite()->draw();
    }

    void Layer::onEnter() {
        CCNode::onEnter();
    }

    void Layer::onExit() {
        CCNode::onExit();
    }

    void Layer::addChild(cocos2d::CCNode* node) {
        Layer::addChild(node, 0, -1);
    }
    
    void Layer::addChild(cocos2d::CCNode* node, int zOrder) {
        Layer::addChild(node, zOrder, -1);
    }

    void Layer::addChild(cocos2d::CCNode* node, int zOrder, int tag) {
        CCNode::addChild(node, zOrder, tag);
        
        if(dynamic_cast<Layer*>(node) != NULL) {
            mChildren.push_back((Layer*)node);
            
            ((Layer*)node)->setParentLayer(this);
            ((Layer*)node)->mOrder = mChildren.size();
        }
    }
    
    void Layer::removeChild(cocos2d::CCNode* node, bool cleanup) {
        if(dynamic_cast<Layer*>(node) != NULL) {
            mChildren.remove((Layer*)node);
            ((Layer*)node)->setParentLayer(0);
        }
        
        CCNode::removeChild(node, cleanup);
    }

    void Layer::removeAllChildrenWithCleanup(bool cleanup) {
        ChildList::iterator it = mChildren.begin();
        for(; it != mChildren.end(); ++it) {
            (*it)->setParentLayer(0);
        }
        
        mChildren.clear();
        
        CCNode::removeAllChildrenWithCleanup(cleanup);
    }

    void Layer::setOrder(int order) {
        mOrder = order;
        
        adjustOrder(getOrder());
    }

    void Layer::adjustOrder(int order) {
        
        if(getParentLayer() != NULL) {
            ChildList& friends = getParentLayer()->mChildren;
            ChildList::iterator it = friends.begin();
            
            friends.remove(this);
            
            // insert self into the right position
            for(; it != friends.end(); ++it) {
                if((*it)->getOrder() < order) {
                    continue;
                } else {
                    friends.insert(it, this);
                    break;
                }
            }
        }
        
    }

    int Layer::getOrder() {
        int order = mOrder;
        if(!this->getAbsoluteOrderMode() && this->getParent())
            order += this->getParentLayer()->getOrder();
        return order;
    }

    bool Layer::onMouseEvent(const cocos2d::CCMouseEvent& evt) {
        if(mFocused) {
            switch(evt.state) {
                case CCMouse::Enter:
                case CCMouse::Leave:
                case CCMouse::Press:
                    
                    printf("Mouse down !!!!!, with button %d\n", evt.button);
                    if(evt.button == CCMouse::RightButton) {
                        Window::MainWindow()->getMenu()->popup(0, evt.x, evt.y);
                    }
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
                    if(evt.key == CCKey::K) {
                        if(mRenderLayer && mImage) {
                            CCTexture2DMutable* texture = mRenderLayer->getTexture();
                            int width = texture->getContentSize().width;
                            int height = texture->getContentSize().height;
                            unsigned int* data = texture->getDataRGBA();
                            assert(data);
                            for(int y = 0; y < height; ++y) {
                                for(int x = 0; x < width; ++x) {
                                    uint32 color = 
                                    data[y * texture->getPixelsWide() + x];
                                    
                                    uint8 r = COLOR_GETR_RGBA(color);
                                    uint8 g = COLOR_GETG_RGBA(color);
                                    uint8 b = COLOR_GETB_RGBA(color);
                                    uint8 a = COLOR_GETA_RGBA(color);
                                    
                                    uint8 avg = 0.3 * r + 0.59 * g + 0.11 * b;
                                    
                                    data[y * texture->getPixelsWide() + x] -= COLOR_RGBA(avg, avg, avg, a);
                                    
                                }
                            }
                            texture->putDataRGBA();
                        }
                    }
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

    void Layer::affineBlend(Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight, bool affine, float A, float B, float C, float D, float E, float F, int32 opa, int32 type) {
        
    }

    void Layer::affineCopy(Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight, bool affine, float A, float B, float C, float D, float E, float F, int32 type, bool clear) {
        
    }

    void Layer::affinePile(Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight, bool affine, float A, float B, float C, float D, float E, float F, int32 opa, int32 type) {
        
    }

    void Layer::assignImages(Layer* src) {
        assert(src != NULL);
        
        CCSprite* image = src->getImage();
        if(image) {
            // copy 
            this->setImage(CCSprite::spriteWithTexture(image->getTexture()));
            
            setDirty();
        }
    
        
        // to do with province and mask 
    }

    void Layer::beginTransition(const char* name, bool withchildren, Layer* transsrc, const char* options) {
        
        // begin transition?
        setDirty();
    }

    void Layer::blendRect(int32 dleft, int32 dtop, Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight, int32 opa) {
        
    }

    void Layer::bringToFront() {
        if(getParentLayer() != NULL) {
            ChildList& friends = getParentLayer()->mChildren;
            
            this->setOrder(friends.front()->getOrder() + 1);
        }
    }

    void Layer::bringToBack() {
        if(getParentLayer() != NULL) {
            ChildList& friends = getParentLayer()->mChildren;
            
            this->setOrder(friends.back()->getOrder() - 1);
        }
    }

    void Layer::colorRect(float left, float right, float width, float height, uint32 color, int32 opacity) {
        
        
        
    }

    void Layer::doBoxBlur(float xblur/* = 1*/, float yblur/* = 1*/) {
        
    }

    void Layer::doGrayScale() {
        
    }

    void Layer::drawText(float x, float y, const char* text, uint32 color, int32 opacity/* = 255*/, bool aa/* = true*/, int32 shadowlevel/* = 0*/, uint32 shadowColor/*=0x00000000*/, float shadowWidth/*= 0*/, float shadowfsx/* = 0*/, float shadowfsy/* = 0*/) {

        setDirty();

    }

    void Layer::fillRect(float left, float top, float width, float height, uint32 color) {
        
    }

    void Layer::flipLR() {
        mRenderLayer->getSprite()->setFlipX(true);
    }

    void Layer::flipUD() {
        mRenderLayer->getSprite()->setFlipY(true);
    }

    void Layer::focus(bool direction/* = true*/) {
        onBeforeFocus(this, false, direction);
        
        mFocused = true;
        
        onFocus(mFocused, direction);
    }

    void Layer::focusNext() {
        if(getParentLayer() != NULL) {
            ChildList& friends = getParentLayer()->mChildren;
            
            ChildList::iterator pos = std::find(friends.begin(),
                                                friends.end(),
                                                this);
            (*(pos++))->focus();
        }
    }

    void Layer::focusPrev() {
        if(getParentLayer() != NULL) {
            ChildList& friends = getParentLayer()->mChildren;
            
            ChildList::iterator pos = std::find(friends.begin(),
                                                friends.end(),
                                                this);
            (*(pos--))->focus();
        }
    }

    Layer* Layer::getLayerAt(float x, float y, bool exclude_self/* = false*/, bool get_disabled/* = false*/) {
        
        ChildList::iterator it = mChildren.begin();
        for(; it != mChildren.end(); ++it) {
            if(!(*it)->getEnabled() && !get_disabled)
                continue;
            
            CCRect contentRect = (*it)->getClippingRect();
            
            if(CCRect::CCRectContainsPoint(contentRect, ccp(x, y))) {
                return *it;
            }
        }
        
        if(!exclude_self)
            return this;
        
        return 0;
    }

    uint32 Layer::getMainPixel(int32 x, int32 y) {
        return mRenderLayer->getTexture()->pixelAt(x,
                                                   mWindow->getHeight() - y);
        
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
        mImage = CCSprite::spriteWithFile(image);
        mImage->setPosition(ccp(0, mWindow->getHeight()));
        mImage->setAnchorPoint(ccp(0, 1));
        
        mRenderLayer->addChild(mImage);
        
        setDirty();
    }

    void Layer::loadProvinceImage(const char* image) {
        
    }

    void Layer::moveBefore(Layer* layer) {
        assert(layer);
        
        if(getParentLayer() &&
           getParentLayer() == layer->getParentLayer()) {
            setOrder(layer->getOrder() + 1);
        }
    }

    void Layer::moveBehind(Layer* layer) {
        assert(layer);
        
        if(getParentLayer() &&
           getParentLayer() == layer->getParentLayer()) {
            setOrder(layer->getOrder() - 1);
        }
    }

    void Layer::operateAffine(Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight, bool affine, float A, float B, float C, float D, float E, float F, int32 mode, int32 opa, int32 type) {
        
    }

    void Layer::operateRect(int32 dleft, int32 dtop, Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight, int32 mode, int32 opa) {
        
    }

    void Layer::operateStretch(int32 dleft, int32 dtop, int32 dwidth, int32 dheight, Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight, int32 mode, int32 opa, int32 type) {
        
    }

    void Layer::pileRect(int32 dleft, int32 dtop, Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight, int32 opa ) {
        
    }

    void Layer::piledCopy(int32 dleft, int32 dtop, Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight) {
        
    }

    void Layer::releaseCapture() {
        
    }

    void Layer::removeMode() {
        
    }

    void Layer::saveLayerImage(const char* name, const char* type/* = "bmp"*/) {
        mRenderLayer->saveBuffer(kCCImageFormatPNG, name);
    }

    void Layer::setAttentionPos(int32 left, int32 top) {
        mAttentionTop = top;
        mAttentionLeft = left;
    }

    void Layer::setClip(int32 x, int32 y, int32 width, int32 height) {
        setClippingRect(CCRectMake(x, y, width, height));
    }

    void Layer::setCursorPos(int32 x, int32 y) {
        mCursorX = x;
        mCursorY = y;
    }
        
    void Layer::setImagePos(int32 left, int32 top) {
        mImage->setPosition(ccp(left, 
                                mWindow->getHeight() - top));
        
        setDirty();
    }

    void Layer::setImageSize(int32 width, int32 height) {
        if(mImage) {
            mImage->setScaleX(((float)width / mImage->getContentSize().width));
            mImage->setScaleY(((float)height / mImage->getContentSize().height));
            
            setDirty();
        }
    }

    void Layer::setMainPixel(int32 x, int32 y, uint32 color) {
        mRenderLayer->getTexture()->setPixelAt(x, y, color);
    }

    void Layer::setMaskPixel(int32 x, int32 y, uint32 value) {
        
    }

    void Layer::setMode() {
        
    }

    void Layer::setPos(int32 left, int32 top, int32 width, int32 height) {
        this->setPosition(ccp(left, top));
        
        if(width != this->getContentSize().width ||
           height != this->getContentSize().height) {
            setSize(width, height);
        }
    }

    void Layer::setProvincePixel(int32 x, int32 y, uint32 value) {
        
    }

    void Layer::setSize(int32 width, int32 height) {
        if(mRenderLayer) 
            this->removeChild(mRenderLayer, true);
        mRenderLayer = CCRenderTextureMutable::renderTextureWithWidthAndHeight(width, height, kTexture2DPixelFormat_RGBA8888);
        this->addChild(mRenderLayer);

        this->setContentSize(CCSize(width, height));

        setClippingRect(CCRectMake(this->getPosition().x, 
                                   this->getPosition().y, 
                                   width,
                                   height));
        
        setDirty();
    }

    void Layer::setSizeToImageSize() {
        if(mImage) {
            setSize(mImage->getContentSize().width, 
                    mImage->getContentSize().height);
        }
    }

    void Layer::stopTransition() {
        
    }

    void Layer::stretchBlend(int32 dleft, int32 dtop, int32 dwidth, int32 dheight, Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight, int32 opa, int32 type) {
        
    }

    void Layer::stretchPile(int32 dleft, int32 dtop, int32 dwidth, int32 dheight, Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight, int32 opa, int32 type) {
        
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

    void Layer::setDirty() {
        mDirty = true;
    }

TVP_NS_VISUAL_END