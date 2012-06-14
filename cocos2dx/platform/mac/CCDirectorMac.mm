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
#include <Availability.h>

#include <sys/time.h>
#include "CCDirectorMac.h"
#include "CCEventDispatcher.h"
#include "CCEGLView_mac.h"

#include "cocoa/CCNS.h"
#include "CCScene.h"
#include "CCMutableArray.h"
#include "CCScheduler.h"
#include "ccMacros.h"
#include "CCTouchDispatcher.h"
#include "CCPointExtension.h"
#include "CCTransition.h"
#include "CCTextureCache.h"
#include "CCTransition.h"
#include "CCSpriteFrameCache.h"
#include "CCAutoreleasePool.h"
#include "platform/platform.h"
#include "CCApplication.h"
#include "CCLabelBMFont.h"
#include "CCActionManager.h"
#include "CCLabelTTF.h"
#include "CCConfiguration.h"
#include "CCKeypadDispatcher.h"
#include "CCGL.h"
#include "CCAnimationCache.h"
#include "CCTouch.h"
#include "CCScriptSupport.h"

#include "EAGLView_mac.h"

using namespace cocos2d;



namespace cocos2d {

bool CCDirectorMac::init(void)
{	
	CCDirector::init();
	isFullScreen_ = false;
	resizeMode_ = kCCDirectorResize_AutoScale;
	
	originalWinSize_ = CCSizeZero;
	//fullScreenWindow_ = NULL;
	//windowGLView_ = NULL;
	winOffset_ = CCPointZero;
	
	return true;
}

CCDirectorMac::CCDirectorMac(void)
{
    //superViewGLView_->release();
	//[fullScreenWindow_ release];
	//[windowGLView_ release];
}

//
// setFullScreen code taken from GLFullScreen example by Apple
//
void CCDirectorMac::setFullScreen(bool fullscreen)
{
}

void CCDirectorMac::setOpenGLView(CCEGLView *view)
{
	CCDirector::setOpenGLView(view);
	
	// cache the NSWindow and NSOpenGLView created from the NIB 
	if( !isFullScreen_ && CCSize::CCSizeEqualToSize(originalWinSize_, CCSizeZero))
    {
		originalWinSize_ = m_obWinSizeInPixels;
	}
}

void CCDirectorMac::setResizeMode(int mode)
{
	if( mode != resizeMode_ ) {

		resizeMode_ = mode;

        setProjection(m_eProjection);
        m_pobOpenGLView->setNeedsDisplay(YES);
	}
}

void CCDirectorMac::setProjection(ccDirectorProjection projection)
{
	CCSize size = m_obWinSizeInPixels;
	
	CCPoint offset = CCPointZero;
	float widthAspect = size.width;
	float heightAspect = size.height;
	
	
	if( resizeMode_ == kCCDirectorResize_AutoScale && ! CCSize::CCSizeEqualToSize(originalWinSize_, CCSizeZero ) ) {
		
		size = originalWinSize_;

		float aspect = originalWinSize_.width / originalWinSize_.height;
		widthAspect = m_obWinSizeInPixels.width;
		heightAspect = m_obWinSizeInPixels.width / aspect;
		
		if( heightAspect > m_obWinSizeInPixels.height ) {
			widthAspect = m_obWinSizeInPixels.height * aspect;
			heightAspect = m_obWinSizeInPixels.height;			
		}
		
		winOffset_.x = (m_obWinSizeInPixels.width - widthAspect) / 2;
		winOffset_.y =  (m_obWinSizeInPixels.height - heightAspect) / 2;
		
		offset = winOffset_;

	}

	switch (projection) {
		case kCCDirectorProjection2D:
			glViewport(offset.x, offset.y, widthAspect, heightAspect);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			ccglOrtho(0, size.width, 0, size.height, -1024, 1024);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			break;
			
		case kCCDirectorProjection3D:
		{
			glViewport(offset.x, offset.y, widthAspect, heightAspect);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(60, (GLfloat)widthAspect/heightAspect, 0.1f, 1500.0f);
			
			glMatrixMode(GL_MODELVIEW);	
			glLoadIdentity();
			
			float eyeZ = size.height * getZEye() / m_obWinSizeInPixels.height;

			gluLookAt( size.width/2, size.height/2, eyeZ,
					  size.width/2, size.height/2, 0,
					  0.0f, 1.0f, 0.0f);			
			break;
		}
		case kCCDirectorProjectionCustom:
			if( m_pProjectionDelegate )
				m_pProjectionDelegate->updateProjection();
			break;
			
		default:
			CCLOG("cocos2d: Director: unrecognized projecgtion");
			break;
	}
	
	m_eProjection = projection;
}

// If scaling is supported, then it should always return the original size
// otherwise it should return the "real" size.
CCSize CCDirectorMac::getWinSize(void)
{
	if( resizeMode_ == kCCDirectorResize_AutoScale )
		return originalWinSize_;
    
	return m_obWinSizeInPixels;
}

CCSize CCDirectorMac::getWinSizeInPixels(void)
{
	return getWinSize();
}

CCPoint CCDirectorMac::convertToLogicalCoordinates(CCPoint coords)
{
	CCPoint ret;
	
	if( resizeMode_ == kCCDirectorResize_NoScale )
		ret = coords;
	
	else {
	
		float x_diff = originalWinSize_.width / (m_obWinSizeInPixels.width - winOffset_.x * 2);
		float y_diff = originalWinSize_.height / (m_obWinSizeInPixels.height - winOffset_.y * 2);
		
		float adjust_x = (m_obWinSizeInPixels.width * x_diff - originalWinSize_.width ) / 2;
		float adjust_y = (m_obWinSizeInPixels.height * y_diff - originalWinSize_.height ) / 2;
		
		ret = CCPointMake( (x_diff * coords.x) - adjust_x, ( y_diff * coords.y ) - adjust_y );		
	}
	
	return ret;
}
	
/***************************************************
 * implementation of DisplayLinkDirector
 **************************************************/

// should we afford 4 types of director ??
// I think DisplayLinkDirector is enough
// so we now only support DisplayLinkDirector
void CCDisplayLinkDirectorMac::startAnimation(void)
{
	//TODO:Mark by rock, just for test
	//if (CCTime::gettimeofdayCocos2d(m_pLastUpdate, NULL) != 0)
	//{
	//	CCLOG("cocos2d: DisplayLinkDirector: Error on gettimeofday");
	//}
	
	m_bInvalid = false;
	CCApplication::sharedApplication().setAnimationInterval(m_dAnimationInterval);
}

void CCDisplayLinkDirectorMac::mainLoop(void)
{
	if (m_bPurgeDirecotorInNextLoop)
	{
		purgeDirector();
		m_bPurgeDirecotorInNextLoop = false;
	}
	else if (! m_bInvalid)
	{
		drawScene();
		
		// release the objects
		CCPoolManager::getInstance()->pop();		
	}
}

void CCDisplayLinkDirectorMac::stopAnimation(void)
{
	m_bInvalid = true;
}

void CCDisplayLinkDirectorMac::setAnimationInterval(double dValue)
{
	m_dAnimationInterval = dValue;
	if (! m_bInvalid)
	{
		stopAnimation();
		startAnimation();
	}	
}
	
//
// Draw the Scene
//
void CCDisplayLinkDirectorMac::drawScene(void)
{    
	// We draw on a secondary thread through the display link
	// When resizing the view, -reshape is called automatically on the main thread
	// Add a mutex around to avoid the threads accessing the context simultaneously	when resizing
	EAGLView *openGLView_ = (EAGLView*)[EAGLView sharedEGLView];
	CGLLockContext((CGLContextObj)[[openGLView_ openGLContext] CGLContextObj]);
	[[openGLView_ openGLContext] makeCurrentContext];
	
	/* calculate "global" dt */
	calculateDeltaTime();
	
	/* tick before glClear: issue #533 */
	if( ! m_bPaused ) {
		CCScheduler::sharedScheduler()->tick(m_fDeltaTime);	
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/* to avoid flickr, nextScene MUST be here: after tick and before draw.
	 XXX: Which bug is this one. It seems that it can't be reproduced with v0.9 */
	if( m_pNextScene )
	{
		setNextScene();
	}
	
	glPushMatrix();
	
	
	// By default enable VertexArray, ColorArray, TextureCoordArray and Texture2D
	CC_ENABLE_DEFAULT_GL_STATES();
	
	/* draw the scene */
	if (m_pRunningScene)
    {
        m_pRunningScene->visit();
    }
	
	/* draw the notification node */
	if (m_pNotificationNode)
	{
		m_pNotificationNode->visit();
	}
	
	if (m_bDisplayFPS)
	{
		showFPS();
	}
	
//	if (m_pWatcherFun && m_pWatcherSender)
//	{
//		(*m_pWatcherFun)(m_pWatcherSender);
//	}
	
#if CC_ENABLE_PROFILERS
	showProfilers();
#endif
	
	CC_DISABLE_DEFAULT_GL_STATES();
	
	glPopMatrix();
	
	
	[[openGLView_ openGLContext] flushBuffer];	
	CGLUnlockContext((CGLContextObj)[[openGLView_ openGLContext] CGLContextObj]);	
}

// set the event dispatcher
void CCDisplayLinkDirectorMac::setOpenGLView(CC_GLVIEW *view)
{
	if( view != m_pobOpenGLView ) {
		
		CCDirectorMac::setOpenGLView(view);
		
		EAGLView *openGLView_ = (EAGLView*)[EAGLView sharedEGLView];
		CCEventDispatcher *eventDispatcher = [CCEventDispatcher sharedDispatcher];
		[openGLView_ setEventDelegate: eventDispatcher];
		[eventDispatcher setDispatchEvents: YES];
		
		// Enable Touches. Default no.
		[openGLView_ setAcceptsTouchEvents:YES];		
		
		// Synchronize buffer swaps with vertical refresh rate
		[[openGLView_ openGLContext] makeCurrentContext];
		GLint swapInt = 1;
		[[openGLView_ openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval]; 
	}
}
} // end of namespace cocos2d

