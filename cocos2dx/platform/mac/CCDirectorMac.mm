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
#include "CCEGLView.h"

#include "cocoa/CCNS.h"
#include "CCScene.h"
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
#include "CCEventDispatcher.h"

#include "EAGLView_mac.h"

using namespace cocos2d;


void gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
    GLfloat xmin, xmax, ymin, ymax;
    
    ymax = zNear * (GLfloat)tanf(fovy * (float)M_PI / 360);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;
    
    
    
    glFrustum(xmin, xmax,
                ymin, ymax,
                zNear, zFar);
}

void gluLookAt(float fEyeX, float fEyeY, float fEyeZ,
               float fCenterX, float fCenterY, float fCenterZ,
               float fUpX, float fUpY, float fUpZ)
{
    GLfloat m[16];
    GLfloat x[3], y[3], z[3];
    GLfloat mag;
    
    /* Make rotation matrix */
    
    /* Z vector */
    z[0] = fEyeX - fCenterX;
    z[1] = fEyeY - fCenterY;
    z[2] = fEyeZ - fCenterZ;
    mag = (float)sqrtf(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
    if (mag) {
        z[0] /= mag;
        z[1] /= mag;
        z[2] /= mag;
    }
    
    /* Y vector */
    y[0] = fUpX;
    y[1] = fUpY;
    y[2] = fUpZ;
    
    /* X vector = Y cross Z */
    x[0] = y[1] * z[2] - y[2] * z[1];
    x[1] = -y[0] * z[2] + y[2] * z[0];
    x[2] = y[0] * z[1] - y[1] * z[0];
    
    /* Recompute Y = Z cross X */
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = -z[0] * x[2] + z[2] * x[0];
    y[2] = z[0] * x[1] - z[1] * x[0];
    
    /* cross product gives area of parallelogram, which is < 1.0 for
     * non-perpendicular unit-length vectors; so normalize x, y here
     */
    
    mag = (float)sqrtf(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    if (mag) {
        x[0] /= mag;
        x[1] /= mag;
        x[2] /= mag;
    }
    
    mag = (float)sqrtf(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
    if (mag) {
        y[0] /= mag;
        y[1] /= mag;
        y[2] /= mag;
    }
    
#define M(row,col)  m[col*4+row]
    M(0, 0) = x[0];
    M(0, 1) = x[1];
    M(0, 2) = x[2];
    M(0, 3) = 0.0f;
    M(1, 0) = y[0];
    M(1, 1) = y[1];
    M(1, 2) = y[2];
    M(1, 3) = 0.0f;
    M(2, 0) = z[0];
    M(2, 1) = z[1];
    M(2, 2) = z[2];
    M(2, 3) = 0.0f;
    M(3, 0) = 0.0f;
    M(3, 1) = 0.0f;
    M(3, 2) = 0.0f;
    M(3, 3) = 1.0f;
#undef M
    {
        int a;
        GLfloat fixedM[16];
        for (a = 0; a < 16; ++a)
            fixedM[a] = m[a];
        glMultMatrixf(fixedM);
    }
    
    /* Translate Eye to Origin */
    glTranslatef(-fEyeX, -fEyeY, -fEyeZ);
}


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
			glOrtho(0, size.width, 0, size.height, -1024, 1024);
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
		CCPoolManager::sharedPoolManager()->pop();
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
    
    CCDirector::drawScene();
    
	[[openGLView_ openGLContext] flushBuffer];
	CGLUnlockContext((CGLContextObj)[[openGLView_ openGLContext] CGLContextObj]);	
}

// set the event dispatcher
void CCDisplayLinkDirectorMac::setOpenGLView(CCEGLView *view)
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

