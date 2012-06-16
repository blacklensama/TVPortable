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

#include "../Predefine.h"

#include "extensions/CCInputDispatcher.h"
#include "extensions/CCInputListener.h"

namespace TVPortable {
    
    namespace Visual {
        
        // affine types used in Layer::affineCopy/Pie/Blend
        enum AffineType {
            stNearest,
            stFastLinear,
            stLinear,
            stCubic,
        };
        
        enum ShiftFlags {
            ssAlt   = 1U << 0,
            ssShift = 1U << 1,
            ssCtrl  = 1U << 2,
            ssLeft  = 1U << 3,
            ssMiddle = 1U << 4,
            ssRight = 1U << 5,
            ssRepeat = 1U << 6
        };
        
        enum MouseButton {
            mbLeft,
            mbMiddle,
            mbRight,
        };
        
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
            TVP_PROPERTY(Focused, bool);
            TVP_PROPERTY(CallOnPaint, bool);
            
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
                             int rfloor = 0,     // 0 - 255
                             int rceil = 255,    // 0 - 255
                             float ggamma = 1.0, // 0.0 ~ 1.0 ~ 9.0
                             int gfloow = 0,     // 0 - 255
                             int gceil = 255,    // 0 - 255
                             float bgamma = 1.0, // 0.0 ~ 1.0 ~ 9.0
                             int bfloor = 0,     // 0 - 255
                             int bceil = 255);   // 0 - 255
            
            
            void bringToFront();
            void bringToBack();
            
            void colorRect(float left, float right, float width, float height, uint32 color, int opacity = 255);
            
            void doBoxBlur(float xblur = 1, float yblur = 1);
            void doGrayScale();
            
            void drawText(float x, float y, const char* text, uint32 color, int opacity = 255, bool aa = true, int shadowlevel = 0, uint32 shadowColor=0x00000000, float shadowWidth = 0, float shadowfsx = 0, float shadowfsy = 0);
            
            void fillRect(float left, float top, float width, float height, uint32 color);
            
            void flipLR();
            void flipUD();
            
            void focus(bool direction = true);
            void focusNext();
            void focusPrev();
            
            Layer* getLayerAt(float x, float y, bool exclude_self = false, bool get_disabled = false);
            uint32 getMainPixel(int x, int y);
            uint32 getMaskPixel(int x, int y);
            uint32 getProvincePixel(int x, int y);
            
            void independMainImage(bool copy = true);
            void independProvinceImage(bool copy = true);
            
            void loadImages(const char* image, uint32 color);
            void loadProvinceImage(const char* image);
            
            
            void moveBefore(Layer* layer);
            void moveBehind(Layer* layer);
            
            void releaseCapture();
            void removeMode();
            
            
            void saveImage(const char* name, const char* type = "bmp");
            void setAttentionPos(float left, float top);
            
            void setClip(float x, float y, float width, float height);
            void setCursorPos(float x, float y);
            
            void setImageSize(float width, float height);
            void setMainPixel(int x, int y, uint32 color);
            void setMaskPixel(int x, int y, uint32 value);
            
            void setMode();
            void setPos(float left, float top, float width, float height);
            void setProvincePixel(int x, int y, uint32 value);
            
            void setSize(float width, float height);
            void setSizeToImageSize();
            
            void stopTransition();
            
            void update(float left, float top, float width, float height);
            
            
        public:
            /* krkr2 Layer events */
            void onBeforeFocus(Layer* layer, bool blurred, bool direction);
            void onBlur(bool focused);
            void onClick(float x, float y);
            void onDoubleClick(float x, float y);
            void onFocus(bool focused, bool direction);
            void onHitTest(float x, float y, bool hit);
            void onKeyDown(uint8 key, int shift, bool process = true);
            void onKeyPress(uint8 key, bool process = true);
            void onKeyUp(uint8 key, int shift, bool process = true);
            void onMouseDown(float x, float y, int btn, int shift);
            void onMouseEnter();
            void onMouseLeave();
            void onMouseMove(float x, float y, int shift);
            void onMouseUp(float x, float y, int button, int shift);
            void onMouseWheel(int shift, int delta, float x, float y);
            void onNodeDisabled();
            void onNodeEnabled();
            void onPaint();
            void onSearchNextFocusable(Layer* layer);
            void onSearchPrevFocusable(Layer* layer);
            void onTransitionCompleted(Layer* dest, Layer* src);
            
        public:
            static Layer* LayerWithClippingRect(const cocos2d::CCRect& rc);
            
        };
    }
}

#endif
