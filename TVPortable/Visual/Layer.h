//
//  Layer.h
//  TVPortable
//
//  Created by Ruiwei Bu on 6/14/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#ifndef TVPortable_Layer_h
#define TVPortable_Layer_h

#include "cocos2d.h"

#include "Prerequisites.h"

#include "extensions/CCInputDispatcher.h"
#include "extensions/CCInputListener.h"

TVP_NS_VISUAL_BEGIN
        
    /**
     krkr Layer
     **/
    class Layer: public cocos2d::CCTargetedTouchDelegate, public cocos2d::CCInputListener, public cocos2d::CCNode {
    public:
        Layer();
        virtual ~Layer();
        
    public:
        /* properties */
        /**
         Set/Get clipping rect, relates to layer origin
         **/
        TVP_PROPERTY(ClippingRect, cocos2d::CCRect);
        TVP_PROPERTY(Image, cocos2d::CCSprite);
        TVP_PROPERTY(CallOnPaint, bool);
        TVP_PROPERTY(AbsoluteOrderMode, bool);
        TVP_PROPERTY(AttentionLeft, int32);
        TVP_PROPERTY(AttentionTop, int32);
        TVP_PROPERTY(Cursor, const char*);
        TVP_PROPERTY(CursorX, int32);
        TVP_PROPERTY(CursorY, int32);
        TVP_PROPERTY(Enabled, bool);
        TVP_PROPERTY(Face, int32);
        TVP_PROPERTY(Focusable, bool);
        TVP_PROPERTY(HasImage, bool);
        
        
        TVP_PROPERTY_READ_ONLY(Focused, bool);
        TVP_PROPERTY_READ_ONLY(Cached, bool);
        TVP_PROPERTY_READ_ONLY(Font, Font*);
        TVP_PROPERTY_READ_ONLY(RenderLayer, cocos2d::CCRenderTexture);
        
        TVP_PROPERTY_GETTER_SETTER(ClipHeight, int32, mClippingRect.size.height);
        TVP_PROPERTY_GETTER_SETTER(ClipWidth, int32, mClippingRect.size.width);
        TVP_PROPERTY_GETTER_SETTER(ClipLeft, int32, mClippingRect.origin.x);
        TVP_PROPERTY_GETTER_SETTER(ClipTop, int32, mClippingRect.origin.y);
        
        
    public:
        /* CCTouchDelegate */
        virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
        virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
        virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) ;
        virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
        
        /* CCNode */
        virtual void visit();
        virtual void draw();
        virtual void onEnter();
        virtual void onExit();
        
        /* CCInputListener */
        virtual bool onMouseEvent(const cocos2d::CCMouseEvent& evt);
        virtual bool onKeyEvent(const cocos2d::CCKeyEvent& evt);
        virtual bool onJoystickEvent(const cocos2d::CCJoyStickEvent& evt);
        
    public:
        /* krkr2 Layer methods */
        void adjustGamma(float rgamma = 1.0, // 0.0 ~ 1.0 ~ 9.0
                         int32 rfloor = 0,     // 0 - 255
                         int32 rceil = 255,    // 0 - 255
                         float ggamma = 1.0, // 0.0 ~ 1.0 ~ 9.0
                         int32 gfloow = 0,     // 0 - 255
                         int32 gceil = 255,    // 0 - 255
                         float bgamma = 1.0, // 0.0 ~ 1.0 ~ 9.0
                         int32 bfloor = 0,     // 0 - 255
                         int32 bceil = 255);   // 0 - 255
        
        void affineBlend(Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight, bool affine, float A, float B, float C, float D, float E, float F, int32 opa=255, int32 type = stNearest);
        void affineCopy(Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight, bool affine, float A, float B, float C, float D, float E, float F, int32 type = stNearest, bool clear = false);
        void affinePile(Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight, bool affine, float A, float B, float C, float D, float E, float F, int32 opa=255, int32 type = stNearest);
        
        void assignImages(Layer* src);
        
        void beginTransition(const char* name, bool withchildren=true, Layer* transsrc = 0, const char* options="%[]");
        
        void blendRect(int32 dleft, int32 dtop, Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight, int32 opa=255);
        
        
        void bringToFront();
        void bringToBack();
        
        void colorRect(float left, float right, float width, float height, uint32 color, int32 opacity = 255);
        
        void doBoxBlur(float xblur = 1, float yblur = 1);
        void doGrayScale();
        
        void drawText(float x, float y, const char* text, uint32 color, int32 opacity = 255, bool aa = true, int32 shadowlevel = 0, uint32 shadowColor=0x00000000, float shadowWidth = 0, float shadowfsx = 0, float shadowfsy = 0);
        
        void fillRect(float left, float top, float width, float height, uint32 color);
        
        void flipLR();
        void flipUD();
        
        void focus(bool direction = true);
        void focusNext();
        void focusPrev();
        
        Layer* getLayerAt(float x, float y, bool exclude_self = false, bool get_disabled = false);
        uint32 getMainPixel(int32 x, int32 y);
        uint32 getMaskPixel(int32 x, int32 y);
        uint32 getProvincePixel(int32 x, int32 y);
        
        void independMainImage(bool copy = true);
        void independProvinceImage(bool copy = true);
        
        void loadImages(const char* image, uint32 color);
        void loadProvinceImage(const char* image);
        
        
        void moveBefore(Layer* layer);
        void moveBehind(Layer* layer);
        
        void operateAffine(Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight, bool affine, float A, float B, float C, float D, float E, float F, int32 mode = omAuto, int32 opa = 255, int32 type = stNearest);
        void operateRect(int32 dleft, int32 dtop, Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight, int32 mode = omAuto, int32 opa = 255);
        void operateStretch(int32 dleft, int32 dtop, int32 dwidth, int32 dheight, Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight, int32 mode = omAuto, int32 opa = 255, int32 type = stNearest);
        
        void pileRect(int32 dleft, int32 dtop, Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight, int32 opa = 255);
        void piledCopy(int32 dleft, int32 dtop, Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight);
        
        
        void releaseCapture();
        void removeMode();
        
        
        void saveLayerImage(const char* name, const char* type = "bmp");
        void setAttentionPos(int32 left, int32 top);
        
        void setClip(int32 x, int32 y, int32 width, int32 height);
        void setCursorPos(int32 x, int32 y);
        
        void setImagePos(int32 left, int32 top);
        void setImageSize(int32 width, int32 height);
        void setMainPixel(int32 x, int32 y, uint32 color);
        void setMaskPixel(int32 x, int32 y, uint32 value);
        
        void setMode();
        void setPos(int32 left, int32 top, int32 width, int32 height);
        void setProvincePixel(int32 x, int32 y, uint32 value);
        
        void setSize(int32 width, int32 height);
        void setSizeToImageSize();
        
        void stopTransition();
        
        void stretchBlend(int32 dleft, int32 dtop, int32 dwidth, int32 dheight, Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight, int32 opa = 255, int32 type = stNearest);
        void stretchPile(int32 dleft, int32 dtop, int32 dwidth, int32 dheight, Layer* src, int32 sleft, int32 stop, int32 swidth, int32 sheight, int32 opa = 255, int32 type = stNearest);
        
        void update(int32 left, int32 top, int32 width, int32 height);
        
        
    public:
        /* krkr2 Layer events */
        void onBeforeFocus(Layer* layer, bool blurred, bool direction);
        void onBlur(bool focused);
        void onClick(float x, float y);
        void onDoubleClick(float x, float y);
        void onFocus(bool focused, bool direction);
        void onHitTest(float x, float y, bool hit);
        void onKeyDown(uint8 key, int32 shift, bool process = true);
        void onKeyPress(uint8 key, bool process = true);
        void onKeyUp(uint8 key, int32 shift, bool process = true);
        void onMouseDown(float x, float y, int32 btn, int32 shift);
        void onMouseEnter();
        void onMouseLeave();
        void onMouseMove(float x, float y, int32 shift);
        void onMouseUp(float x, float y, int32 button, int32 shift);
        void onMouseWheel(int32 shift, int32 delta, float x, float y);
        void onNodeDisabled();
        void onNodeEnabled();
        void onPaint();
        void onSearchNextFocusable(Layer* layer);
        void onSearchPrevFocusable(Layer* layer);
        void onTransitionCompleted(Layer* dest, Layer* src);
        
    public:
        static Layer* LayerWithClippingRect(const cocos2d::CCRect& rc);
        
    };

TVP_NS_VISUAL_END

#endif
