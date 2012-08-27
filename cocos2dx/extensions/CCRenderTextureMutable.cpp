/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org
Copyright (c) 2009      Jason Booth

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/


/*
	A CCRenderTexture extension that supports modifying the rendered texture with CCTexture2DMutable
	Copyright (c) 2012 Robert Bu(Studio Symphonie). All rights reserved.
*/

#include "CCConfiguration.h"
#include "CCRenderTextureMutable.h"
#include "CCDirector.h"
#include "platform/platform.h"
#include "CCImage.h"
#include "support/ccUtils.h"
#include "CCTextureCache.h"
#include "CCFileUtils.h"
#include "CCGL.h"
#include "CCRenderTexture.h"

#include "kazmath/kazmath.h"
#include "kazmath/GL/matrix.h"

namespace cocos2d { 

// implementation CCRenderTextureMutable
CCRenderTextureMutable::CCRenderTextureMutable()
: m_pSprite(NULL)
, m_uFBO(0)
, m_nOldFBO(0)
, m_pTexture(0)
, m_pUITextureImage(NULL)
, m_uDepthRenderBufffer(0)
, m_ePixelFormat(kCCTexture2DPixelFormat_RGBA8888)
{
}

CCRenderTextureMutable::~CCRenderTextureMutable()
{
    removeAllChildrenWithCleanup(true);
    glDeleteFramebuffers(1, &m_uFBO);

	CC_SAFE_DELETE(m_pUITextureImage);
}

CCSprite * CCRenderTextureMutable::getSprite()
{
	return m_pSprite;
}
void CCRenderTextureMutable::setSprite(CCSprite* var)
{
	m_pSprite = var;
}

CCRenderTextureMutable * CCRenderTextureMutable::renderTextureWithWidthAndHeight(int w, int h, CCTexture2DPixelFormat eFormat)
{
    CCRenderTextureMutable *pRet = new CCRenderTextureMutable();

    if(pRet && pRet->initWithWidthAndHeight(w, h, eFormat))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CCRenderTextureMutable * CCRenderTextureMutable::renderTextureWithWidthAndHeight(int w, int h)
{
	CCRenderTextureMutable *pRet = new CCRenderTextureMutable();

    if(pRet && pRet->initWithWidthAndHeight(w, h, kCCTexture2DPixelFormat_RGBA8888))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool CCRenderTextureMutable::initWithWidthAndHeight(int w, int h, CCTexture2DPixelFormat eFormat)
{
	// If the gles version is lower than GLES_VER_1_0, 
	// some extended gles functions can't be implemented, so return false directly.
/*	if (CCConfiguration::sharedConfiguration()->getGlesVersion() <= GLES_VER_1_0)
	{
		return false;
	}*/
    CCAssert(m_ePixelFormat != kCCTexture2DPixelFormat_A8, "only RGB and RGBA formats are valid for a render texture");
    
    bool bRet = false;
    do
    {
        w *= (int)CC_CONTENT_SCALE_FACTOR();
        h *= (int)CC_CONTENT_SCALE_FACTOR();
        
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_nOldFBO);
        
        // textures must be power of two squared
        unsigned int powW = 0;
        unsigned int powH = 0;
        
        if( CCConfiguration::sharedConfiguration()->supportsNPOT() ) {
            powW = w;
            powH = h;
        } else {
            powW = ccNextPOT(w);
            powH = ccNextPOT(h);
        }
        
        void *data = malloc((int)(powW * powH * 4));
        CC_BREAK_IF(! data);
        
        memset(data, 0, (int)(powW * powH * 4));
        m_ePixelFormat = eFormat;
        
        m_pTexture = new CCTexture2DMutable();
        CC_BREAK_IF(! m_pTexture);
        
        m_pTexture->initWithData(data, (CCTexture2DPixelFormat)m_ePixelFormat, powW, powH, CCSizeMake((float)w, (float)h));
        free( data );
        
        GLint oldRBO;
        glGetIntegerv(GL_RENDERBUFFER_BINDING, &oldRBO);
        
        // generate FBO
        glGenFramebuffers(1, &m_uFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_uFBO);
        
        // associate texture with FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTexture->getName(), 0);
        
        if (m_uDepthRenderBufffer != 0)
        {
            GLuint uDepthStencilFormat = CC_GL_DEPTH24_STENCIL8;
            
            //create and attach depth buffer
            glGenRenderbuffers(1, &m_uDepthRenderBufffer);
            glBindRenderbuffer(GL_RENDERBUFFER, m_uDepthRenderBufffer);
            glRenderbufferStorage(GL_RENDERBUFFER, uDepthStencilFormat, powW, powH);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_uDepthRenderBufffer);
            
            // if depth format is the one with stencil part, bind same render buffer as stencil attachment
            if (uDepthStencilFormat == CC_GL_DEPTH24_STENCIL8)
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_uDepthRenderBufffer);
        }
        
        
        // check if it worked (probably worth doing :) )
        CCAssert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Could not attach texture to framebuffer");
        
        m_pTexture->setAliasTexParameters();
        
        m_pSprite = CCSprite::create(m_pTexture);
        
        m_pTexture->release();
        m_pSprite->setScaleY(-1);
        this->addChild(m_pSprite);
        
        ccBlendFunc tBlendFunc = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA };
        m_pSprite->setBlendFunc(tBlendFunc);
        
        glBindRenderbuffer(GL_RENDERBUFFER, oldRBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_nOldFBO);
        bRet = true;
    } while (0);
    
    return bRet;
}

void CCRenderTextureMutable::begin()
{
	// Save the current matrix
    kmGLPushMatrix();
    
    const CCSize& texSize = m_pTexture->getContentSizeInPixels();
    
    // Calculate the adjustment ratios based on the old and new projections
    CCDirector *director = CCDirector::sharedDirector();
    CCSize size = director->getWinSizeInPixels();
    float widthRatio = size.width / texSize.width;
    float heightRatio = size.height / texSize.height;
    
    // Adjust the orthographic projection and viewport
    glViewport(0, 0, (GLsizei)texSize.width, (GLsizei)texSize.height);
    
    
    kmMat4 orthoMatrix;
    kmMat4OrthographicProjection(&orthoMatrix, (float)-1.0 / widthRatio,  (float)1.0 / widthRatio,
                                 (float)-1.0 / heightRatio, (float)1.0 / heightRatio, -1,1 );
    kmGLMultMatrix(&orthoMatrix);
    
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_nOldFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_uFBO);
}

void CCRenderTextureMutable::beginWithClear(float r, float g, float b, float a)
{
    this->begin();

	// save clear color
	GLfloat	clearColor[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE,clearColor); 

	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// restore clear color
	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);     
}

void CCRenderTextureMutable::end(bool bIsTOCacheTexture)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_nOldFBO);
    kmGLPopMatrix();
    
    CCDirector *director = CCDirector::sharedDirector();
    
    CCSize size = director->getWinSizeInPixels();
    
    // restore viewport
    glViewport(0, 0, GLsizei(size.width * CC_CONTENT_SCALE_FACTOR()), GLsizei(size.height * CC_CONTENT_SCALE_FACTOR()));
    
    // special viewport for 3d projection + retina display
    if ( director->getProjection() == kCCDirectorProjection3D && CC_CONTENT_SCALE_FACTOR() != 1 )
    {
        glViewport((GLsizei)(-size.width/2), (GLsizei)(-size.height/2), (GLsizei)(size.width * CC_CONTENT_SCALE_FACTOR()), (GLsizei)(size.height * CC_CONTENT_SCALE_FACTOR()));
    }
    
    director->setProjection(director->getProjection());
    
#if CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
    m_pTexture->updateData();
#else
    // on ios & android, we need to read the fbo directly cause there's no glGetTexImage in opengles
    m_pTexture->updateData(m_uFBO, m_nOldFBO);
#endif
}

void CCRenderTextureMutable::clear(float r, float g, float b, float a)
{
	this->beginWithClear(r, g, b, a);
	this->end();
}

bool CCRenderTextureMutable::saveBuffer(const char *szFilePath, int x, int y, int nWidth, int nHeight)
{
	bool bRet = false;

	CCImage *pImage = new CCImage();
	if (pImage != NULL && getUIImageFromBuffer(pImage, x, y, nWidth, nHeight))
	{
		bRet = pImage->saveToFile(szFilePath);
	}

	CC_SAFE_DELETE(pImage);
	return bRet;
}
bool CCRenderTextureMutable::saveBuffer(int format, const char *fileName, int x, int y, int nWidth, int nHeight)
{
	bool bRet = false;
	CCAssert(format == kCCImageFormatJPEG || format == kCCImageFormatPNG,
			 "the image can only be saved as JPG or PNG format");

	CCImage *pImage = new CCImage();
	if (pImage != NULL && getUIImageFromBuffer(pImage, x, y, nWidth, nHeight))
	{
		std::string fullpath = CCFileUtils::sharedFileUtils()->getWriteablePath() + fileName;
		
		bRet = pImage->saveToFile(fullpath.c_str());
	}

	CC_SAFE_DELETE(pImage);

	return bRet;
}

/* get buffer as UIImage */
bool CCRenderTextureMutable::getUIImageFromBuffer(CCImage *pImage, int x, int y, int nWidth, int nHeight)
{
	if (NULL == pImage || NULL == m_pTexture)
	{
		return false;
	}

	const CCSize& s = m_pTexture->getContentSizeInPixels();
	int tx = (int)s.width;
	int ty = (int)s.height;

	if (x < 0 || x >= tx || y < 0 || y >= ty)
	{
		return false;
	}

	if (nWidth < 0 
		|| nHeight < 0
		|| (0 == nWidth && 0 != nHeight)
		|| (0 == nHeight && 0 != nWidth))
	{
		return false;
	}
	
	// to get the image size to save
	//		if the saving image domain exeeds the buffer texture domain,
	//		it should be cut
	int nSavedBufferWidth = nWidth;
	int nSavedBufferHeight = nHeight;
	if (0 == nWidth)
	{
		nSavedBufferWidth = tx;
	}
	if (0 == nHeight)
	{
		nSavedBufferHeight = ty;
	}
	nSavedBufferWidth = x + nSavedBufferWidth > tx ? (tx - x): nSavedBufferWidth;
	nSavedBufferHeight = y + nSavedBufferHeight > ty ? (ty - y): nSavedBufferHeight;

	GLubyte *pBuffer = NULL;
	GLubyte *pTempData = NULL;
	bool bRet = false;

	do
	{
		CCAssert(m_ePixelFormat == kCCTexture2DPixelFormat_RGBA8888, "only RGBA8888 can be saved as image");

		CC_BREAK_IF(! (pBuffer = new GLubyte[nSavedBufferWidth * nSavedBufferHeight * 4]));

		// On some machines, like Samsung i9000, Motorola Defy,
		// the dimension need to be a power of 2
		int nReadBufferWidth = 0;
		int nReadBufferHeight = 0;
		int nMaxTextureSize = 0;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &nMaxTextureSize);

		nReadBufferWidth = ccNextPOT(tx);
		nReadBufferHeight = ccNextPOT(ty);

		CC_BREAK_IF(0 == nReadBufferWidth || 0 == nReadBufferHeight);
		CC_BREAK_IF(nReadBufferWidth > nMaxTextureSize || nReadBufferHeight > nMaxTextureSize);

		CC_BREAK_IF(! (pTempData = new GLubyte[nReadBufferWidth * nReadBufferHeight * 4]));

		this->begin();
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(0,0,nReadBufferWidth,nReadBufferHeight,GL_RGBA,GL_UNSIGNED_BYTE, pTempData);
		this->end(false);

		// to get the actual texture data 
		// #640 the image read from rendertexture is upseted
		for (int i = 0; i < nSavedBufferHeight; ++i)
		{
			memcpy(&pBuffer[i * nSavedBufferWidth * 4], 
				&pTempData[(y + nSavedBufferHeight - i - 1) * nReadBufferWidth * 4 + x * 4], 
				nSavedBufferWidth * 4);
		}

		bRet = pImage->initWithImageData(pBuffer, nSavedBufferWidth * nSavedBufferHeight * 4, CCImage::kFmtRawData, nSavedBufferWidth, nSavedBufferHeight, 8);
	} while (0);

	CC_SAFE_DELETE_ARRAY(pBuffer);
	CC_SAFE_DELETE_ARRAY(pTempData);

	return bRet;
}

    CCTexture2DMutable* CCRenderTextureMutable::getTexture() const {
        return m_pTexture;
    }

} // namespace cocos2d
