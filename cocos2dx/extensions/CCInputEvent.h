//
//  CCKeyboardEvent.h
//  TVPortable
//
//  Created by Ruiwei Bu on 6/15/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#ifndef TVPortable_CCKeyboardEvent_h
#define TVPortable_CCKeyboardEvent_h

#include "cocos2d.h"

namespace cocos2d {
    
    /**
     Original Input classes from Project Unknown
     **/
    // mouse
    struct CC_DLL CCMouse {
        enum MouseButton {
            Null = -1,
            
            LeftButton = 0x01,
            RightButton = 0x02,
            MiddleButton = 0x04,
        };
        
        enum MouseState {
            Press      = 0,
            Release    = 1,
            Move       = 2,
            Wheel      = 3,
            Drag       = 4,
            Enter      = 5,
            Leave      = 6,
        };
    };
    
    // keyboard
    struct CC_DLL CCKey {
        enum KeyCode {
            Null = -1,
            
            Num0 = 0x30,
            Num1,
            Num2,
            Num3,
            Num4,
            Num5,
            Num6,
            Num7,
            Num8,
            Num9,
            
            A = 0x41,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            J,
            K,
            L,
            M,
            N,
            O,
            P,
            Q,
            R,
            S,
            T,
            U,
            V,
            W,
            X,
            Y,
            Z,
            
            F1 = 0x70,
            F2,
            F3,
            F4,
            F5,
            F6,
            F7,
            F8,
            F9,
            F10,
            F11,
            F12,
            
            Numpad0 = 0x60,
            Numpad1,
            Numpad2,
            Numpad3,
            Numpad4,
            Numpad5,
            Numpad6,
            Numpad7,
            Numpad8,
            Numpad9,
            
            Escape		= 0x1B,
            Backspace	= 0x08,
            Tab         = 0x09,
            Enter		= 0x0D,
            Space		= 0x20,
            
            Shift	= 0x10,
            Ctrl	= 0x11,
            Alt     = 0x12,
            
            Pause		= 0x13,
            CapsLock	= 0x14,
            NumLock     = 0x90,
            ScrollLock  = 0x91,
            
            PageUp		= 0x21,
            PageDown	= 0x22,
            Home		= 0x24,
            End         = 0x23,
            Insert		= 0x2D,
            Delete		= 0x2E,
            
            Left	= 0x25,
            Up		= 0x26,
            Right	= 0x27,
            Down	= 0x28,
            
            Grave		= 0xC0,
            Minus		= 0xBD,
            Equals		= 0xBB,
            Backslash	= 0xDC,
            LeftBracket	= 0xDB,
            RightBracket= 0xDD,
            Semicolon	= 0xBA,
            Apostrophe  = 0xDE,
            Comma		= 0xBC,
            Period		= 0xBE,
            Slash		= 0xBF,
            
            LeftWin     = 0x5B,
            RightWin    = 0x5C,
            Apps        = 0x5D,
        };
        
        enum KeyState {
            Press    = 0,
            Release  = 1,
        };
        
        enum KeyFlag {
            FlagShift		= 1 << 0,
            FlagCtrl		= 1 << 1,
            FlagAlt			= 1 << 2,
            FlagCapslock	= 1 << 3,
            FlagScrolllock	= 1 << 4,
            FlagNumlock		= 1 << 5,
            FlagRepeat		= 1 << 6,
        };
    };
    
    // joystick
    struct CC_DLL CCJoyStick {
        enum JoyStickButton {
            Joystick1 = 0,
            Joystick2,
            Joystick3,
            Joystick4,
            Joystick5,
            Joystick6,
            Joystick7,
            Joystick8,
            Joystick9,
            Joystick10,
            Joystick11,
            Joystick12,
            Joystick13,
            Joystick14,
            Joystick15,
            Joystick16,
        };
        
        enum JoyStickState {
            Press       = 0,
            Release     = 1
        };
    };

	struct CC_DLL CCInputEvent {
		enum Type {
			Mouse,
			Keyboard,
			Joystick,
			Touch,
		};

		CCInputEvent::Type type;
	};
    
    struct CC_DLL CCKeyEvent: public CCInputEvent {
		CCKeyEvent() { type = CCInputEvent::Keyboard; }

        CCKey::KeyState    state;		// state
        CCKey::KeyCode     key;			// key code
        int                flag;
        
        // experimental
        bool    isIME;                  // is ime key
        
        bool isKeyDown(CCKey::KeyCode k) const;
        bool isKeyUp(CCKey::KeyCode k) const;
        bool isDigit() const;
        bool isLetter() const;
        bool isPrintable() const;
        bool isShiftFlag() const;
        bool isCtrlFlag() const;
        bool isAltFlag() const;
        
        char toCharCode() const;
    };
    
    struct CC_DLL CCMouseEvent: public CCInputEvent {
		CCMouseEvent() { type = CCInputEvent::Mouse; }

        CCMouse::MouseState   state;
        CCMouse::MouseButton  button;
        
        int   flag;            
        int   x;
        int   y;
        int   wheel;
        
        int  deltaX;
        int  deltaY;
    };
    
    struct CC_DLL CCJoyStickEvent: public CCInputEvent {
		CCJoyStickEvent() { type = CCInputEvent::Joystick; }

        CCJoyStick::JoyStickState     flag;
        CCJoyStick::JoyStickButton    button;
    };
    
}


#endif
