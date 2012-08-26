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
#include "Font.h"

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
    mWindow(window),
    mText(0),
    mOpacity(255),
    mType(ltOpaque) {
        mFace = dfMain | dfOpaque;
        
   //     CCInputDispatcher::Instance().addListener(this, 1);
   //     CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 1, false);
        
        setClippingRect(CCRect(0,
                               0,
                               window->getWidth(),
                               window->getHeight()));
        setPos(0, 0, window->getWidth(), window->getHeight());
        
        if(parent) {
            parent->addChild(this);
        }
        
        mFont = Font::DefaultFont();
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
        if (!isVisible()) {
            return;
        }
        
       // glPushMatrix();
       // glEnable(GL_SCISSOR_TEST);
        
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
        
       // glDisable(GL_SCISSOR_TEST);
       // glPopMatrix();
    }


    bool Layer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
        CCMouseEvent evt;
        evt.button = CCMouse::LeftButton;
        evt.state = CCMouse::Press;
        evt.x = pTouch->locationInView().x;
        evt.y = pTouch->locationInView().y;
        evt.wheel = 0;
        evt.flag = 0;
        evt.deltaX = 0;
        evt.deltaY = 0;
        
        mPrevTouchPosX = evt.x;
        mPrevTouchPosY = evt.y;
        
        this->onMouseEvent(evt);
        
        return mFocused;
    }

    void Layer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
        CCMouseEvent evt;
        evt.button = CCMouse::LeftButton;
        evt.state = CCMouse::Drag;
        evt.x = pTouch->locationInView().x;
        evt.y = pTouch->locationInView().y;
        evt.wheel = 0;
        evt.flag = 0;
        // to do, touch moved delta position
        evt.deltaX = evt.x - mPrevTouchPosX;
        evt.deltaY = evt.y - mPrevTouchPosY;
        
        mPrevTouchPosX = evt.x;
        mPrevTouchPosY = evt.y;
        
        this->onMouseEvent(evt);
    }

    void Layer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
        CCMouseEvent evt;
        evt.button = CCMouse::LeftButton;
        evt.state = CCMouse::Release;
        evt.x = pTouch->locationInView().x;
        evt.y = pTouch->locationInView().y;
        evt.wheel = 0;
        evt.flag = 0;
        evt.deltaX = 0;
        evt.deltaY = 0;
        
        this->onMouseEvent(evt);
    }

    void Layer::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
        CCMouseEvent evt;
        evt.button = CCMouse::LeftButton;
        evt.state = CCMouse::Release;
        evt.x = pTouch->locationInView().x;
        evt.y = pTouch->locationInView().y;
        evt.wheel = 0;
        evt.flag = 0;
        evt.deltaX = 0;
        evt.deltaY = 0;
        
        this->onMouseEvent(evt);
    }

    void Layer::draw() {
        // render child layers first
        if(!mChildren.empty()) {
            ChildList::iterator it = mChildren.end();
            for(; it != mChildren.begin(); --it) {
                (*it)->draw();
            }
        }
        
        // if there are changes, RTT and save the buffer
        if(mDirty) {
            mRenderLayer->begin();
        
            if(mImage) {
                mImage->setFlipY(true);
                mImage->draw();
            }
            if(mText)
                mText->draw();
            
            mRenderLayer->end();
            mDirty = false;
        }
        
        // render children buffers, not neccessary here
        /*
        if(!mChildren.empty()) {
            ChildList::iterator it = mChildren.end();
            for(; it != mChildren.begin(); --it) {
                (*it)->getRenderLayer()->getSprite()->draw();
            }   
        }*/
        
        // render layer buffer
       // mRenderLayer->getSprite()->setFlipY(true);
        this->onPaint();
        
        if((mFace & dfAlpha) ||
           (mFace & dfAlphaAdd) ||
           (mFace & dfMain) ||
           (mFace & dfAuto)) {
			   ccBlendFunc blendFunc = {CC_BLEND_SRC, GL_ONE_MINUS_SRC_ALPHA};
            switch(mType) {
                case ltAlpha:
                    blendFunc.src = CC_BLEND_SRC;
                    blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
                    break;
                    
                case ltAddApha:
                    blendFunc.src = GL_ONE;
                    blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
                    break;
                    
                case ltAdditive:
                    blendFunc.src = GL_SRC_ALPHA;
                    blendFunc.dst = GL_ONE;
                    break;
                    
                case ltDarken:
                case ltLighten:
                case ltSubtractive:
                    break;
                    
                case ltDodge:
                    blendFunc.src = GL_ONE;
                    blendFunc.dst = GL_ONE;
                    break;
                    
                case ltMultiplicative:
                    blendFunc.src = GL_DST_COLOR;
                    blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
                    break;
                    
                case ltScreen:
                    blendFunc.src = GL_ONE;
                    blendFunc.dst = GL_ONE_MINUS_SRC_COLOR;
                    break;
            }
            
            mRenderLayer->getSprite()->setOpacity(mOpacity);
            mRenderLayer->getSprite()->setBlendFunc(blendFunc);
            mRenderLayer->getSprite()->draw();
        }
        if(mFace & dfProvince) {
            
        }
        if(mFace & dfMask) {
            
        }
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
                                        
					this->adjustGamma(1.5, 0, 255, 1.5, 0, 255, 1.5, 0, 255);
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

    void Layer::adjustGamma(float rgamma, int32 rfloor, int32 rceil, float ggamma, int32 gfloor, int32 gceil, float bgamma, int32 bfloor, int32 bceil)  {
        if(mRenderLayer) {
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
                    
                    uint8 newR = clampf(255 * ::pow((float)r/255.f, 1.f / rgamma), rfloor, rceil);
                    uint8 newG = clampf(255 * ::pow((float)g/255.f, 1.f / ggamma), gfloor, gceil);
                    uint8 newB = clampf(255 * ::pow((float)b/255.f, 1.f / bgamma), bfloor, bceil);
                    
                    data[y * texture->getPixelsWide() + x] = COLOR_RGBA(newR, newG, newB, a);
                }
            }
            texture->putDataRGBA();
        }
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
            if(mImage) {
                mRenderLayer->removeChild(mImage, true);
            }
            mImage = CCSprite::spriteWithTexture(src->getImage()->getTexture());
            mImage->setPosition(ccp(0, mWindow->getHeight()));
            mImage->setAnchorPoint(ccp(0, 1));
            
            mRenderLayer->addChild(mImage);
            
            setDirty();
        }
    
        
        // to do with province and mask 
    }

    void Layer::beginTransition(const char* name, bool withchildren, Layer* transsrc, const char* options) {
        
        // begin transition?
        setDirty();
    }

    void Layer::blendRect(int32 dleft, int32 dtop, Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight, int32 opacity) {
        if(mRenderLayer) {
            CCTexture2DMutable* texture = mRenderLayer->getTexture();
            int width = texture->getContentSize().width;
            int height = texture->getContentSize().height;
            unsigned int* data = texture->getDataRGBA();
            assert(data);
            
            CCTexture2DMutable* srcTexture = src->getRenderLayer()->getTexture();
            int srcWidth = srcTexture->getContentSize().width;
            int srcHeight = srcTexture->getContentSize().height;
            unsigned int* srcData = srcTexture->getDataRGBA();
            assert(srcData);
            
            int yend = dtop + sheight;
            if(yend > height) yend = height;
            int xend = dleft + swidth;
            if(xend > width) xend = width;
            
            for(int y = dtop; y < yend; ++y) {
                for(int x = dleft; x < xend; ++x) {
                    int srcX = x - dleft + sleft;
                    int srcY = y - dtop + stop;
                    
                    uint32 color = data[y * texture->getPixelsWide() + x];
                    uint32 srcColor = srcData[srcY * srcTexture->getPixelsWide() + srcX];
                    float opa = opacity / 255.f;
                    
                    uint8 r = COLOR_GETR_RGBA(color);
                    uint8 g = COLOR_GETG_RGBA(color);
                    uint8 b = COLOR_GETB_RGBA(color);
                    uint8 a = COLOR_GETA_RGBA(color);
                    
                    uint8 srcr = COLOR_GETR_RGBA(srcColor);
                    uint8 srcg = COLOR_GETG_RGBA(srcColor);
                    uint8 srcb = COLOR_GETB_RGBA(srcColor);
                    
                    data[y * texture->getPixelsWide() + x] = COLOR_RGBA(r * opa + srcr * (1.f - opa), 
                                                                        g * opa + srcg * (1.f - opa), 
                                                                        b * opa + srcb * (1.f - opa), 
                                                                        a);
                }
            }
        
            texture->putDataRGBA();
        }
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

    void Layer::colorRect(float left, float top, float width, float height, uint32 color, int32 opacity) {
        if(mRenderLayer) {
            mRenderLayer->getTexture()->fillRect(CCRectMake(left, top, width, height),
                                                 COLOR_SETA_RGBA(color, opacity));
        }
    }

    void Layer::doBoxBlur(float xblur/* = 1*/, float yblur/* = 1*/) {
        if(mRenderLayer) {
            CCTexture2DMutable* texture = mRenderLayer->getTexture();
            int width = texture->getContentSize().width;
            int height = texture->getContentSize().height;
			int padding = texture->getPixelsWide();
            unsigned int* data = texture->getDataRGBA();
            assert(data);
           
            uint8 filterSize = (xblur*2) * (yblur*2);
            
            // RGBA8888

            unsigned int* origin = (unsigned int*)malloc(texture->getPixelsWide() * texture->getPixelsHigh() * 4);
            memcpy(origin, data, texture->getPixelsWide() * texture->getPixelsHigh() * 4);
            
            for(int y = 0; y < height; ++y) {
                for(int x = 0; x < width; ++x) {
                    uint32 color = 
                    origin[y * texture->getPixelsWide() + x];
                    
                    uint16 r = 0;
                    uint16 g = 0;
                    uint16 b = 0;
                    uint8 a = COLOR_GETA_RGBA(color);
                    
                    if(a == 0)
                        continue;
                    
                    for(int i=-xblur; i<xblur; ++i) {
                        for(int j=-yblur; j<yblur; ++j) {
                            uint32 fx = clampf(x+i, 0, width);
                            uint32 fy = clampf(y+j, 0, height);
                            
                            uint32 fcolor = data[fy * texture->getPixelsWide() + fx];
                            
                            r += COLOR_GETR_RGBA(fcolor);
                            g += COLOR_GETG_RGBA(fcolor);
                            b += COLOR_GETB_RGBA(fcolor);
                        }
                    }
                    r /= filterSize;
                    g /= filterSize;
                    b /= filterSize;
                    
                    data[y * texture->getPixelsWide() + x] = COLOR_RGBA(r, g, b, a);
                }
            }
            texture->putDataRGBA();
        }
    }

    void Layer::doGrayScale() {
        if(mRenderLayer) {
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
                    
                    if(a == 0)
                        continue;
                    
                    uint8 avg = 0.3 * r + 0.59 * g + 0.11 * b;
                    
                    data[y * texture->getPixelsWide() + x] = COLOR_RGBA(avg, avg, avg, a);
                }
            }
            texture->putDataRGBA();
        }
    }

    void Layer::drawText(float x, float y, const char* text, uint32 color, int32 opacity/* = 255*/, bool aa/* = true*/, int32 shadowlevel/* = 0*/, uint32 shadowColor/*=0x00000000*/, float shadowWidth/*= 0*/, float shadowfsx/* = 0*/, float shadowfsy/* = 0*/) {

        if(!mText)
            mText = CCLabelTTF::labelWithString("", 
                                                mFont->getFace().c_str(), 
                                                mFont->getHeight());
        if(mText) {
            mText->setString(text);
            mText->setPosition(ccp(x, y));
            mText->setColor(ccc3(COLOR_GETR_RGBA(color), 
                                 COLOR_GETG_RGBA(color), 
                                 COLOR_GETB_RGBA(color)));
            mText->setOpacity(opacity);
            
            // to do with shadow;
            // and font render angle
            
            setDirty();
            
        }
    }

    void Layer::fillRect(float left, float top, float width, float height, uint32 color) {
        if(mRenderLayer) {
            mRenderLayer->getTexture()->fillRect(CCRectMake(left,
                                                            top,
                                                            width, 
                                                            height), 
                                                 color);
        }
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
        if(mImage) {
            mRenderLayer->removeChild(mImage, true);
        }
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
        mRenderLayer = CCRenderTextureMutable::renderTextureWithWidthAndHeight(width, height, kCCTexture2DPixelFormat_Default);
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
        this->publishListenerEvent(&LayerListener::onBeforeFocus, layer, blurred, direction);
    }

    void Layer::onBlur(bool focused) {
        this->publishListenerEvent(&LayerListener::onBlur, focused);
    }

    void Layer::onClick(float x, float y) {
        this->publishListenerEvent(&LayerListener::onClick, x, y);
    }

    void Layer::onDoubleClick(float x, float y) {
        this->publishListenerEvent(&LayerListener::onDoubleClick, x, y);
    }

    void Layer::onFocus(bool focused, bool direction) {
        this->publishListenerEvent(&LayerListener::onFocus, focused, direction);
    }

    void Layer::onHitTest(float x, float y, bool hit) {
        this->publishListenerEvent(&LayerListener::onHitTest, x, y, hit);
    }

    void Layer::onKeyDown(uint8 key, int32 shift, bool process/* = true*/) {
        this->publishListenerEvent(&LayerListener::onKeyDown, key, shift, process);
    }

    void Layer::onKeyPress(uint8 key, bool process/* = true*/) {
        this->publishListenerEvent(&LayerListener::onKeyPress, key, process);
    }

    void Layer::onKeyUp(uint8 key, int32 shift, bool process/* = true*/) {
        this->publishListenerEvent(&LayerListener::onKeyUp, key, shift, process);
    }

    void Layer::onMouseDown(float x, float y, int32 btn, int32 shift) {
        this->publishListenerEvent(&LayerListener::onMouseDown, x, y, btn, shift);
    }

    void Layer::onMouseEnter() {
        this->publishListenerEvent(&LayerListener::onMouseEnter);
    }

    void Layer::onMouseLeave() {
        this->publishListenerEvent(&LayerListener::onMouseLeave);
    }

    void Layer::onMouseMove(float x, float y, int32 shift) {
        this->publishListenerEvent(&LayerListener::onMouseMove, x, y, shift);
    }

    void Layer::onMouseUp(float x, float y, int32 button, int32 shift) {
        this->publishListenerEvent(&LayerListener::onMouseUp, x, y, button, shift);
    }

    void Layer::onMouseWheel(int32 shift, int32 delta, float x, float y) {
        this->publishListenerEvent(&LayerListener::onMouseWheel, shift, delta, x, y);
    }

    void Layer::onNodeDisabled() {
        this->publishListenerEvent(&LayerListener::onNodeDisabled);
    }

    void Layer::onNodeEnabled() {
        this->publishListenerEvent(&LayerListener::onNodeEnabled);
    }

    void Layer::onPaint() {
        this->publishListenerEvent(&LayerListener::onPaint);
    }

    void Layer::onSearchNextFocusable(Layer* layer) {
        this->publishListenerEvent(&LayerListener::onSearchNextFocusable, layer);
    }

    void Layer::onSearchPrevFocusable(Layer* layer) {
        this->publishListenerEvent(&LayerListener::onSearchPrevFocusable, layer);

    }

    void Layer::onTransitionCompleted(Layer* dest, Layer* src) {
        this->publishListenerEvent(&LayerListener::onTransitionCompleted, dest, src);

    }

    void Layer::setDirty() {
        mDirty = true;
    }

TVP_NS_VISUAL_END