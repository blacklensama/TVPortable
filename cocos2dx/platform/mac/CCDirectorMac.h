/*
 * cocos2d for iPhone: http://www.cocos2d-iphone.org
 *
 * Copyright (c) 2010 Ricardo Quesada
 * Copyright (c) 2011 Zynga Inc.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


// Only compile this code on Mac. These files should not be included on your iOS project.
// But in case they are included, it won't be compiled.
#ifndef __CC_DIRECTOR_MAC_H__
#define __CC_DIRECTOR_MAC_H__
#include "CCDirector.h"
//@class NSWindow;

/** Base class of Mac directors
 @since v0.99.5
 */
#define kCCDirectorResize_AutoScale 1
#define kCCDirectorResize_NoScale 2
namespace cocos2d {

class CCDirectorMac : public CCDirector
{
public:
	CCDirectorMac();
	
	virtual bool init(void);
	virtual void setOpenGLView(CC_GLVIEW *pobOpenGLView);
	virtual void setProjection(ccDirectorProjection projection);
	virtual CCSize getWinSizeInPixels(void);
	virtual CCSize getWinSize(void);
	
	// whether or not the view is in fullscreen mode
	inline bool isFullScreen(void) { return isFullScreen_; };
	
	// resize mode: with or without scaling
	void setResizeMode(int mode);
	inline int getResizeMode(void) { return resizeMode_; };
	
	inline void setOriginalWinSize(CCSize winSize) { originalWinSize_ = winSize; };
	inline CCSize getOriginalWinSize(void) { return originalWinSize_; };
	
	/** Sets the view in fullscreen or window mode */
	void setFullScreen(bool fullscreen);
	
	/** Converts window size coordiantes to logical coordinates.
	 Useful only if resizeMode is kCCDirectorResize_Scale.
	 If resizeMode is kCCDirectorResize_NoScale, then no conversion will be done.
	 */
	CCPoint convertToLogicalCoordinates(CCPoint coordinates);
	
private:
	bool			isFullScreen_;
	int				resizeMode_;
	CCPoint			winOffset_;
    CCSize			originalWinSize_;
	
	//NSWindow		*fullScreenWindow_;
    
	// cache
	//NSWindow		*windowGLView_;
    CC_GLVIEW       *superViewGLView_;
    CCRect          originalWinRect_; // Original size and position
};
	
class CCDisplayLinkDirectorMac : public CCDirectorMac
{
public:
	CCDisplayLinkDirectorMac(void) 
	: m_bInvalid(false)
	{}
	
	virtual void mainLoop(void);
	virtual void setAnimationInterval(double dValue);
	virtual void startAnimation(void);
	virtual void stopAnimation();
	
	
	virtual void drawScene(void);
	virtual void setOpenGLView(CC_GLVIEW *view);
	
protected:
	bool m_bInvalid;
};
	
} // end of namespace cocos2d
#endif // __CC_DIRECTOR_MAC_H__

