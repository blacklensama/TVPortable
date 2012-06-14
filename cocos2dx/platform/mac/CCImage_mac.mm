/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

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
#include <Foundation/Foundation.h>
#include <Cocoa/Cocoa.h>
#include "CCImage.h"
#include "CCFileUtils.h"
#include "CCTexture2D.h"
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include "CCDirectorMac.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#define szFont_kenning 2

#define SHIFT6(num) (num>>6)

using namespace std;

typedef struct
{
    unsigned int height;
    unsigned int width;
    int         bitsPerComponent;
    bool        hasAlpha;
    bool        isPremultipliedAlpha;
    unsigned char*  data;
} tImageInfo;

static unsigned int nextPOT(unsigned int x)
{
    x = x - 1;
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    return x + 1;
}

typedef enum {
    kCCTexture2DPixelFormat_Automatic = 0,
        //! 32-bit texture: RGBA8888
    kCCTexture2DPixelFormat_RGBA8888,
        //! 24-bit texture: RGBA888
    kCCTexture2DPixelFormat_RGB888,
        //! 16-bit texture without Alpha channel
    kCCTexture2DPixelFormat_RGB565,
        //! 8-bit textures used as masks
    kCCTexture2DPixelFormat_A8,
        //! 16-bit textures: RGBA4444
    kCCTexture2DPixelFormat_RGBA4444,
        //! 16-bit textures: RGB5A1
    kCCTexture2DPixelFormat_RGB5A1,    
    
        //! Default texture format: RGBA8888
    kCCTexture2DPixelFormat_Default = kCCTexture2DPixelFormat_RGBA8888,
    
        // backward compatibility stuff
    kTexture2DPixelFormat_Automatic = kCCTexture2DPixelFormat_Automatic,
    kTexture2DPixelFormat_RGBA8888 = kCCTexture2DPixelFormat_RGBA8888,
    kTexture2DPixelFormat_RGB888 = kCCTexture2DPixelFormat_RGB888,
    kTexture2DPixelFormat_RGB565 = kCCTexture2DPixelFormat_RGB565,
    kTexture2DPixelFormat_A8 = kCCTexture2DPixelFormat_A8,
    kTexture2DPixelFormat_RGBA4444 = kCCTexture2DPixelFormat_RGBA4444,
    kTexture2DPixelFormat_RGB5A1 = kCCTexture2DPixelFormat_RGB5A1,
    kTexture2DPixelFormat_Default = kCCTexture2DPixelFormat_Default
    
} CCTexture2DPixelFormat;

// BitmapDC

struct TextLine {
	string sLineStr;
	int iLineWidth;
};



static bool _initPremultipliedATextureWithImage(CGImageRef image, NSUInteger POTWide, NSUInteger POTHigh, tImageInfo *pImageInfo)
{
    NSUInteger            i;
    CGContextRef        context = nil;
    unsigned char*        data = nil;;
    CGColorSpaceRef        colorSpace;
    unsigned char*        tempData;
    unsigned int*        inPixel32;
    unsigned short*        outPixel16;
    bool                hasAlpha;
    CGImageAlphaInfo    info;
    CGSize                imageSize;
    CCTexture2DPixelFormat    pixelFormat;
    
    info = CGImageGetAlphaInfo(image);
    hasAlpha = ((info == kCGImageAlphaPremultipliedLast) || (info == kCGImageAlphaPremultipliedFirst) || (info == kCGImageAlphaLast) || (info == kCGImageAlphaFirst) ? YES : NO);
    
    size_t bpp = CGImageGetBitsPerComponent(image);
    colorSpace = CGImageGetColorSpace(image);
    
    if(colorSpace) 
    {
        if(hasAlpha || bpp >= 8)
        {
            pixelFormat = kCCTexture2DPixelFormat_Default;
        }
        else 
        {
            pixelFormat = kCCTexture2DPixelFormat_RGB565;
        }
    } 
    else  
    {
        // NOTE: No colorspace means a mask image
        pixelFormat = kCCTexture2DPixelFormat_A8;
    }
    
    imageSize.width = CGImageGetWidth(image);
    imageSize.height = CGImageGetHeight(image);
    
    // Create the bitmap graphics context
    
    switch(pixelFormat) 
    {      
        case kCCTexture2DPixelFormat_RGBA8888:
        case kCCTexture2DPixelFormat_RGBA4444:
        case kCCTexture2DPixelFormat_RGB5A1:
            colorSpace = CGColorSpaceCreateDeviceRGB();
            data = new unsigned char[POTHigh * POTWide * 4];
            info = hasAlpha ? kCGImageAlphaPremultipliedLast : kCGImageAlphaNoneSkipLast; 
            context = CGBitmapContextCreate(data, POTWide, POTHigh, 8, 4 * POTWide, colorSpace, info | kCGBitmapByteOrder32Big);                
            CGColorSpaceRelease(colorSpace);
            break;
            
        case kCCTexture2DPixelFormat_RGB565:
            colorSpace = CGColorSpaceCreateDeviceRGB();
            data = new unsigned char[POTHigh * POTWide * 4];
            info = kCGImageAlphaNoneSkipLast;
            context = CGBitmapContextCreate(data, POTWide, POTHigh, 8, 4 * POTWide, colorSpace, info | kCGBitmapByteOrder32Big);
            CGColorSpaceRelease(colorSpace);
            break;
        case kCCTexture2DPixelFormat_A8:
            data = new unsigned char[POTHigh * POTWide];
            info = kCGImageAlphaOnly; 
            context = CGBitmapContextCreate(data, POTWide, POTHigh, 8, POTWide, NULL, info);
            break;            
        default:
            return false;
    }
    
    CGRect rect;
    rect.size.width = POTWide;
    rect.size.height = POTHigh;
    rect.origin.x = 0;
    rect.origin.y = 0;
    
    CGContextClearRect(context, rect);
    CGContextTranslateCTM(context, 0, 0);
    CGContextDrawImage(context, rect, image);
    
    // Repack the pixel data into the right format
    
    if(pixelFormat == kCCTexture2DPixelFormat_RGB565) 
    {
        //Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGGBBBBB"
        tempData = new unsigned char[POTHigh * POTWide * 2];
        inPixel32 = (unsigned int*)data;
        outPixel16 = (unsigned short*)tempData;
        for(i = 0; i < POTWide * POTHigh; ++i, ++inPixel32)
        {
            *outPixel16++ = ((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) | ((((*inPixel32 >> 8) & 0xFF) >> 2) << 5) | ((((*inPixel32 >> 16) & 0xFF) >> 3) << 0);
        }
        
        delete[] data;
        data = tempData;
        
    }
    else if (pixelFormat == kCCTexture2DPixelFormat_RGBA4444) 
    {
        //Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRGGGGBBBBAAAA"
        tempData = new unsigned char[POTHigh * POTWide * 2];
        inPixel32 = (unsigned int*)data;
        outPixel16 = (unsigned short*)tempData;
        for(i = 0; i < POTWide * POTHigh; ++i, ++inPixel32)
        {
            *outPixel16++ = 
            ((((*inPixel32 >> 0) & 0xFF) >> 4) << 12) | // R
            ((((*inPixel32 >> 8) & 0xFF) >> 4) << 8) | // G
            ((((*inPixel32 >> 16) & 0xFF) >> 4) << 4) | // B
            ((((*inPixel32 >> 24) & 0xFF) >> 4) << 0); // A
        }       
        
        delete[] data;
        data = tempData;
        
    }
    else if (pixelFormat == kCCTexture2DPixelFormat_RGB5A1) 
    {
        //Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGBBBBBA"
        tempData = new unsigned char[POTHigh * POTWide * 2];
        inPixel32 = (unsigned int*)data;
        outPixel16 = (unsigned short*)tempData;
        for(i = 0; i < POTWide * POTHigh; ++i, ++inPixel32)
        {
            *outPixel16++ = 
            ((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) | // R
            ((((*inPixel32 >> 8) & 0xFF) >> 3) << 6) | // G
            ((((*inPixel32 >> 16) & 0xFF) >> 3) << 1) | // B
            ((((*inPixel32 >> 24) & 0xFF) >> 7) << 0); // A
        }
        
        delete[] data;
        data = tempData;
    }
    
    // should be after calling super init
    pImageInfo->isPremultipliedAlpha = true;
    pImageInfo->hasAlpha = true;
    pImageInfo->bitsPerComponent = bpp;
    pImageInfo->width = POTWide;
    pImageInfo->height = POTHigh;
    
    if (pImageInfo->data)
    {
        delete [] pImageInfo->data;
    }
    pImageInfo->data = data;
    
    CGContextRelease(context);
    return true;
}

static bool _initWithImage(CGImageRef CGImage, tImageInfo *pImageinfo, double scaleX, double scaleY)
{
    NSUInteger POTWide, POTHigh;
    
    if(CGImage == NULL) 
    {
        return false;
    }
    
	if (cocos2d::CCImage::getIsScaleEnabled())
	{
		POTWide = CGImageGetWidth(CGImage) * scaleX;
		POTHigh = CGImageGetHeight(CGImage) * scaleY;
	}
	else 
	{
		POTWide = CGImageGetWidth(CGImage);
		POTHigh = CGImageGetHeight(CGImage);
	}
    
    
    // always load premultiplied images
    _initPremultipliedATextureWithImage(CGImage, POTWide, POTHigh, pImageinfo);
    
    return true;
}

static bool _initWithFile(const char* path, tImageInfo *pImageinfo)
{
    CGImageRef                CGImage;    
    NSImage                    *jpg;
    //    NSImage                    *png;
    bool            ret;
    
    // convert jpg to png before loading the texture
    
    NSString *fullPath = [NSString stringWithUTF8String:path];
    jpg = [[NSImage alloc] initWithContentsOfFile: fullPath];
    //png = [[NSImage alloc] initWithData:UIImagePNGRepresentation(jpg)];
    CGImageSourceRef source = CGImageSourceCreateWithData((CFDataRef)[jpg TIFFRepresentation], NULL);
	CGImage =  CGImageSourceCreateImageAtIndex(source, 0, NULL);
    
    ret = _initWithImage(CGImage, pImageinfo, 1.0, 1.0);
    
    //    [png release];
    [jpg release];
    
    return ret;
}


static bool _initWithData(void * pBuffer, int length, tImageInfo *pImageinfo, double scaleX, double scaleY)
{
    bool ret = false;
    
    if (pBuffer) 
    {
        CGImageRef CGImage;
        NSData *data;
        
        data = [NSData dataWithBytes:pBuffer length:length];
		CGImageSourceRef source = CGImageSourceCreateWithData((CFDataRef)data, NULL);
        CGImage = CGImageSourceCreateImageAtIndex(source, 0, NULL);
        
        ret = _initWithImage(CGImage, pImageinfo, scaleX, scaleY);
    }
    
    return ret;
}

static bool _isValidFontName(const char *fontName)
{
    bool ret = false;
#if 0 
    NSString *fontNameNS = [NSString stringWithUTF8String:fontName];
    
    for (NSString *familiName in [NSFont familyNames]) 
    {
        if ([familiName isEqualToString:fontNameNS]) 
        {
            ret = true;
            goto out;
        }
        
        for(NSString *font in [NSFont fontNamesForFamilyName: familiName])
        {
            if ([font isEqualToString: fontNameNS])
            {
                ret = true;
                goto out;
            }
        }
    }
#endif
    out:
    return ret;
}


static bool _initWithString(const char * pText, cocos2d::CCImage::ETextAlign eAlign, const char * pFontName, int nSize, tImageInfo* pInfo)
{
    bool bRet = false;
    
	CCAssert( pText, @"Invalid pText");
	CCAssert( pInfo, @"Invalid pInfo");
	
	do {
		NSString * string  = [NSString stringWithUTF8String:pText];
        NSString * fntName = [NSString stringWithUTF8String:pFontName];
        CGSize dim, constrainSize;
        constrainSize.width = pInfo->width;
        constrainSize.height = pInfo->height;
		
		id font;
		font = [[NSFontManager sharedFontManager]
                fontWithFamily:[NSString stringWithUTF8String:pFontName]
                traits:NSUnboldFontMask | NSUnitalicFontMask
                weight:0
                size:nSize];
        
		
		if (font == nil)
            return false;
		
		// color
		NSColor* foregroundColor;
        //		if (pStrokeColor) {
        //			foregroundColor = [NSColor colorWithDeviceRed:pStrokeColor->r/255.0 green:pStrokeColor->g/255.0 blue:pStrokeColor->b/255.0 alpha:1];
        //		} else {
        foregroundColor = [NSColor whiteColor];
        //		}
		
		
		// alignment, linebreak
		unsigned uHoriFlag = eAlign & 0x0f;
		unsigned uVertFlag = (eAlign & 0xf0) >> 4;
		NSTextAlignment align = (2 == uHoriFlag) ? NSRightTextAlignment
        : (3 == uHoriFlag) ? NSCenterTextAlignment
        : NSLeftTextAlignment;
		
		NSMutableParagraphStyle *paragraphStyle = [[[NSMutableParagraphStyle alloc] init] autorelease];
		[paragraphStyle setParagraphStyle:[NSParagraphStyle defaultParagraphStyle]];
		[paragraphStyle setLineBreakMode:NSLineBreakByCharWrapping];
		[paragraphStyle setAlignment:align];
        
		// attribute
		NSDictionary* tokenAttributesDict = [NSDictionary dictionaryWithObjectsAndKeys:
											 foregroundColor,NSForegroundColorAttributeName,
											 font, NSFontAttributeName,
											 paragraphStyle, NSParagraphStyleAttributeName, nil];
        
		// linebreak
		if (pInfo->width > 0) {
			if ([string sizeWithAttributes:tokenAttributesDict].width > pInfo->width) {
				NSMutableString *lineBreak = [[[NSMutableString alloc] init] autorelease];
				NSUInteger length = [string length];
				NSRange range = NSMakeRange(0, 1);
				NSUInteger width = 0;
				for (NSUInteger i = 0; i < length; i++) {
					range.location = i;
					[lineBreak appendString:[string substringWithRange:range]];
					width = [lineBreak sizeWithAttributes:tokenAttributesDict].width;
					if (width > pInfo->width) {
						[lineBreak insertString:@"\r\n" atIndex:[lineBreak length] - 1];
					}
				}
				string = lineBreak;
			}
		}
        
		NSAttributedString *stringWithAttributes =[[[NSAttributedString alloc] initWithString:string
                                                                                   attributes:tokenAttributesDict] autorelease];
        
		NSSize realDimensions = [stringWithAttributes size];
		// Mac crashes if the width or height is 0
		CC_BREAK_IF(realDimensions.width <= 0 || realDimensions.height <= 0);
        
		CGSize dimensions = CGSizeMake(pInfo->width, pInfo->height);
		
        
		if(dimensions.width <= 0 && dimensions.height <= 0) {
			dimensions.width = realDimensions.width;
			dimensions.height = realDimensions.height;
		} else if (dimensions.height <= 0) {
			dimensions.height = realDimensions.height;
		}
        
		NSUInteger POTWide = (NSUInteger)dimensions.width;
		NSUInteger POTHigh = (NSUInteger)(MAX(dimensions.height, realDimensions.height));
		unsigned char*			data;
		//Alignment
        
		CGFloat xPadding = 0;
		switch (align) {
			case NSLeftTextAlignment: xPadding = 0; break;
			case NSCenterTextAlignment: xPadding = (dimensions.width-realDimensions.width)/2.0f; break;
			case NSRightTextAlignment: xPadding = dimensions.width-realDimensions.width; break;
			default: break;
		}
		
		CGFloat yPadding = (1 == uVertFlag || realDimensions.height >= dimensions.height) ? 0	// align to top
		: (2 == uVertFlag) ? dimensions.height - realDimensions.height							// align to bottom
		: (dimensions.height - realDimensions.height) / 2.0f;									// align to center
		
		
		NSRect textRect = NSMakeRect(xPadding, POTHigh - dimensions.height + yPadding, realDimensions.width, realDimensions.height);
		//Disable antialias
		
		[[NSGraphicsContext currentContext] setShouldAntialias:NO];	
		
		NSImage *image = [[NSImage alloc] initWithSize:NSMakeSize(POTWide, POTHigh)];
		[image lockFocus];	
		
		//[stringWithAttributes drawAtPoint:NSMakePoint(xPadding, offsetY)]; // draw at offset position	
		[stringWithAttributes drawInRect:textRect];
		//[stringWithAttributes drawInRect:textRect withAttributes:tokenAttributesDict];
		NSBitmapImageRep *bitmap = [[NSBitmapImageRep alloc] initWithFocusedViewRect:NSMakeRect (0.0f, 0.0f, POTWide, POTHigh)];
		[image unlockFocus];
		
		data = (unsigned char*) [bitmap bitmapData];  //Use the same buffer to improve the performance.
		
		NSUInteger textureSize = POTWide*POTHigh*4;
		
		unsigned char* dataNew = new unsigned char[textureSize];
		CC_BREAK_IF(!dataNew);
		memcpy(dataNew, data, textureSize);
		
		[bitmap release];
		[image release]; 
		
		// output params
		pInfo->width = POTWide;
		pInfo->height = POTHigh;
		pInfo->data = dataNew;
		pInfo->hasAlpha = true;
		pInfo->isPremultipliedAlpha = true;
		pInfo->bitsPerComponent = 8;
		
		bRet = true;
	} while (0);
    return bRet;
}

NS_CC_BEGIN;
class BitmapDC
{
public:
	BitmapDC() {
		libError = FT_Init_FreeType( &library );
		iInterval = szFont_kenning;
		m_pData = NULL;
		reset();
	}
    
	~BitmapDC() {
		FT_Done_FreeType(library);
		//data will be deleted by CCImage
        //		if (m_pData) {
        //			delete m_pData;
        //		}
        
	}
    
	void reset() {
		iMaxLineWidth = 0;
		iMaxLineHeight = 0;
		vLines.clear();
	}
    
	void buildLine(stringstream& ss, FT_Face face, int iCurXCursor, char cLastChar) {
		TextLine oTempLine;
		ss << '\0';
		oTempLine.sLineStr = ss.str();
		//get last glyph
		FT_Load_Glyph(face, FT_Get_Char_Index(face, cLastChar),
                      FT_LOAD_DEFAULT);
        
        oTempLine.iLineWidth = iCurXCursor - SHIFT6((face->glyph->metrics.horiAdvance + face->glyph->metrics.horiBearingX - face->glyph->metrics.width))/*-iInterval*/;//TODO interval
		iMaxLineWidth = MAX(iMaxLineWidth, oTempLine.iLineWidth);
		ss.clear();
		ss.str("");
		vLines.push_back(oTempLine);
	}
    
	bool divideString(FT_Face face, const char* sText, int iMaxWidth, int iMaxHeight) {
		const char* pText = sText;
		int iError = 0;
		int iCurXCursor;
		iError = FT_Load_Glyph(face, FT_Get_Char_Index(face, *pText),
                               FT_LOAD_DEFAULT);
		if (iError) {
			return false;
		}
		iCurXCursor = -SHIFT6(face->glyph->metrics.horiBearingX);
		//init stringstream
        stringstream ss;
        
		int cLastCh = 0;
        
		while (*pText != '\0') {
			if (*pText == '\n') {
				buildLine(ss, face, iCurXCursor, cLastCh);
                
				pText++;
				iError = FT_Load_Glyph(face, FT_Get_Char_Index(face, *pText),
                                       FT_LOAD_DEFAULT);
				if (iError) {
					return false;
				}
				iCurXCursor = -SHIFT6(face->glyph->metrics.horiBearingX);
				continue;
			}
            
			iError = FT_Load_Glyph(face, FT_Get_Char_Index(face, *pText),
                                   FT_LOAD_DEFAULT);
            
			if (iError) {
				return false;
				//break;
			}
			//check its width
			//divide it when exceeding
			if ((iMaxWidth > 0
                 && iCurXCursor + SHIFT6(face->glyph->metrics.width)
                 > iMaxWidth)) {
				buildLine(ss, face , iCurXCursor, cLastCh);
                
				iCurXCursor = -SHIFT6(face->glyph->metrics.horiBearingX);
                
			}
            
			cLastCh = *pText;
			ss << *pText;
			iCurXCursor += SHIFT6(face->glyph->metrics.horiAdvance) + iInterval;
			pText++;
            
		}
		if (iError) {
			return false;
		}
        
		buildLine(ss,face, iCurXCursor, cLastCh);
        
		return true;
	}
    
	/**
	 * compute the start pos of every line
	 *
	 * return >0 represent the start x pos of the line
	 * while -1 means fail
	 *
	 */
	int computeLineStart(FT_Face face, CCImage::ETextAlign eAlignMask, char cText,
                         int iLineIndex) {
		int iRet;
		int iError = FT_Load_Glyph(face, FT_Get_Char_Index(face, cText),
                                   FT_LOAD_DEFAULT);
		if (iError) {
			return -1;
		}
        
		if (eAlignMask == CCImage::kAlignCenter) {
			iRet = (iMaxLineWidth - vLines[iLineIndex].iLineWidth) / 2
			- SHIFT6(face->glyph->metrics.horiBearingX );
            
		} else if (eAlignMask == CCImage::kAlignRight) {
			iRet = (iMaxLineWidth - vLines[iLineIndex].iLineWidth)
			- SHIFT6(face->glyph->metrics.horiBearingX );
		} else {
			// left or other situation
			iRet = -SHIFT6(face->glyph->metrics.horiBearingX );
		}
		return iRet;
	}
    
	bool getBitmap(const char *text, int nWidth, int nHeight, CCImage::ETextAlign eAlignMask, const char * pFontName, float fontSize) {
		FT_Face face;
		FT_Error iError;
        
		const char* pText = text;
		//data will be deleted by CCImage
        //		if (m_pData) {
        //			delete m_pData;
        //		}
        
		int iCurXCursor, iCurYCursor;
		bool bRet = false;
		if (libError) {
			return false;
		}
		do {
			//CCLog("\n\n ---- FT_New_Face with pFontName = %s\n", pFontName);
			iError = FT_New_Face( library, pFontName, 0, &face );
            
			if (iError) {
				//no valid font found use default
				//CCLog("\n\n ---- no valid font, use default %s\n", pFontName);
				iError = FT_New_Face( library, "/System/Library/Fonts/ThonburiBold.ttf", 0, &face );
			}
			CC_BREAK_IF(iError);
            
			//select utf8 charmap
			iError = FT_Select_Charmap(face,FT_ENCODING_UNICODE);
			CC_BREAK_IF(iError);
            
			iError = FT_Set_Pixel_Sizes(face, fontSize,fontSize);
			CC_BREAK_IF(iError);
            
			iError = divideString(face, text, nWidth, nHeight)?0:1;
            
			//compute the final line width
			iMaxLineWidth = MAX(iMaxLineWidth, nWidth);
            
			iMaxLineHeight = (face->size->metrics.ascender >> 6)
			- (face->size->metrics.descender >> 6);
			iMaxLineHeight *= vLines.size();
            
			//compute the final line height
			iMaxLineHeight = MAX(iMaxLineHeight, nHeight);
			m_pData = new unsigned char[iMaxLineWidth * iMaxLineHeight*4];
			iCurYCursor = SHIFT6(face->size->metrics.ascender);
            
			memset(m_pData,0, iMaxLineWidth * iMaxLineHeight*4);
            
            for (size_t i = 0; i < vLines.size(); i++) {
				pText = vLines[i].sLineStr.c_str();
				//initialize the origin cursor
				iCurXCursor = computeLineStart(face, eAlignMask, *pText, i);
                
				while (*pText != 0) {
					int iError = FT_Load_Glyph(face, FT_Get_Char_Index(face, *pText),
                                               FT_LOAD_RENDER);
					if (iError) {
						break;
					}
                    
					//  convert glyph to bitmap with 256 gray
					//  and get the bitmap
					FT_Bitmap & bitmap = face->glyph->bitmap;
                    
					for (int i = 0; i < bitmap.rows; ++i) {
						for (int j = 0; j < bitmap.width; ++j) {
							//  if it has gray>0 we set show it as 1, o otherwise
							int iY = iCurYCursor + i
							- (face->glyph->metrics.horiBearingY
                               >> 6);
							int iX = iCurXCursor
							+ (face->glyph->metrics.horiBearingX
                               >> 6) + j;
                            
							if (iY>=iMaxLineHeight) {
								//exceed the height truncate
								continue;
							}
                            
                            //							m_pData[(iY * iMaxLineWidth + iX) * 4 + 3] =
                            //							bitmap.buffer[i * bitmap.width + j] ?
                            //							0xff : 0;//alpha
                            //							m_pData[(iY * iMaxLineWidth + iX) * 4 + 1] =
                            //							bitmap.buffer[i * bitmap.width + j];//R
                            //							m_pData[(iY * iMaxLineWidth + iX) * 4 + 2] =
                            //							bitmap.buffer[i * bitmap.width + j];//G
                            //							m_pData[(iY * iMaxLineWidth + iX) * 4 + 0] =
                            //							bitmap.buffer[i * bitmap.width + j];//B
							int iTemp = 0;
							unsigned char cTemp = bitmap.buffer[i
                                                                * bitmap.width + j];
							iTemp |= (cTemp ? 0xff : 0)<<24;
							iTemp |= cTemp << 16 | cTemp << 8 | cTemp;
							*(int*) &m_pData[(iY * iMaxLineWidth + iX)
                                             * 4 + 0] = iTemp;
						}
					}
					//step to next glyph
					iCurXCursor += (face->glyph->metrics.horiAdvance >> 6)
					+ iInterval;
                    
					pText++;
				}
				iCurYCursor += (face->size->metrics.ascender >> 6)
				- (face->size->metrics.descender >> 6);
			}
			//print all image bitmap
            //			for (int i = 0; i < iMaxLineHeight; i++) {
            //				for (int j = 0; j < iMaxLineWidth; j++) {
            //					printf("%d",
            //							m_pData[(i * iMaxLineWidth + j) * 4] ? 1 : 0);
            //				}
            //				printf("\n");
            //			}
            
			//  free face
			FT_Done_Face(face);
			face = NULL;
            
			//clear all lines
			vLines.clear();
            
			//success;
			if (iError) {
				bRet = false;
			} else
                bRet = true;
		}while(0);
        
		return bRet;
	}
public:
	FT_Library library;
	unsigned char *m_pData;
	int libError;
	vector<TextLine> vLines;
	int iInterval;
	int iMaxLineWidth;
	int iMaxLineHeight;
};

static BitmapDC& sharedBitmapDC()
{
	static BitmapDC s_BmpDC;
	return s_BmpDC;
}

bool isFileExists(const char* szFilePath)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	//TCHAR dirpath[MAX_PATH];
	//MultiByteToWideChar(936,0,szFilePath,-1,dirpath,sizeof(dirpath));
	DWORD dwFileAttr = GetFileAttributesA(szFilePath);
	if (INVALID_FILE_ATTRIBUTES == dwFileAttr
		|| (dwFileAttr&FILE_ATTRIBUTE_DIRECTORY))	{
		return false;
	}		
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	bool bFind = true;
	do 
	{
		struct stat buf;
		int n = stat(szFilePath, &buf);
		if ((0 != n)
			|| !(buf.st_mode&S_IFMT))	
		{
			bFind = false;
		}
	} while (0);
	if (!bFind)
	{
		//std::string strFilenName = s_strRelativePath + szFilePath;
		unsigned char * pBuffer = NULL;
		unzFile pFile = NULL;
		unsigned long pSize = 0;
		
		do 
		{
			pFile = unzOpen(s_strAndroidPackagePath.c_str());
			if(!pFile)break;
			
			int nRet = unzLocateFile(pFile, szFilePath, 1);
			if(UNZ_OK != nRet)
				bFind = false;
			else
				bFind = true;
		} while (0);
		
		if (pFile)
		{
			unzClose(pFile);
		}
	}
	
	return bFind;
#else
	struct stat buf;
	int n = stat(szFilePath, &buf);
	if ((0 != n)
		|| !(buf.st_mode&S_IFMT))	{
		return false;
	}		
	
#endif
	return true;
}

CCImage::CCImage()
: m_nWidth(0)
, m_nHeight(0)
, m_nBitsPerComponent(0)
, m_pData(0)
, m_bHasAlpha(false)
, m_bPreMulti(false)
, m_dScaleX(1.0)
, m_dScaleY(1.0)

{
    
}

CCImage::~CCImage()
{
    CC_SAFE_DELETE_ARRAY(m_pData);
}

bool cocos2d::CCImage::m_bEnabledScale = true;

bool CCImage::initWithImageFile(const char * strPath, EImageFormat eImgFmt/* = eFmtPng*/)
{
 	std::string strTemp = CCFileUtils::fullPathFromRelativePath(strPath);
	if (m_bEnabledScale)
	{
		if (!isFileExists(strTemp.c_str()))
		{
			if (strTemp.rfind("@2x") == std::string::npos)
			{
				int t = strTemp.rfind(".");
				if (t != std::string::npos)
				{
					strTemp.insert(t, "@2x");
				}
				CCSize size = cocos2d::CCDirectorMac::sharedDirector()->getWinSize();		
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				m_dScaleX = size.width/800.0f;
				m_dScaleY = size.height/480.0f;
	#else
				m_dScaleX = size.width/640.0f;
				m_dScaleY = size.height/960.0f;
				
	#endif
			}
		}    
		else
		{
			m_dScaleX = 1.0;
			m_dScaleY = 1.0;
		}
	}
	CCFileData tempData(strTemp.c_str(), "rb");			
	return initWithImageData(tempData.getBuffer(), tempData.getSize(), eImgFmt);
}
bool CCImage::initWithImageFileThreadSafe(const char *fullpath, EImageFormat imageType)
{
    CC_UNUSED_PARAM(imageType);
    /*
     * CCFileUtils::fullPathFromRelativePath() is not thread-safe, it use autorelease().
     */
    CCFileData data(fullpath, "rb");
    return initWithImageData(data.getBuffer(), data.getSize(), imageType);
}
//bool CCImage::potImageData(unsigned int POTWide, unsigned int POTHigh)
//{
//	unsigned char*			data = NULL;
//	unsigned char*			tempData =NULL;
//	unsigned int*				inPixel32 = NULL;
//	unsigned short*			outPixel16 = NULL;
//	bool					hasAlpha;
//	CCTexture2DPixelFormat	pixelFormat;
//	
//	hasAlpha = this->hasAlpha();
//	
//	size_t bpp = this->getBitsPerComponent();
//	
//    // compute pixel format
//	if(hasAlpha)
//	{
//		pixelFormat = CCTexture2D::defaultAlphaPixelFormat();
//	}
//	else
//	{
//		if (bpp >= 8)
//		{
//			pixelFormat = kCCTexture2DPixelFormat_RGB888;
//		}
//		else
//		{
//			CCLOG("cocos2d: CCTexture2D: Using RGB565 texture since image has no alpha");
//			pixelFormat = kCCTexture2DPixelFormat_RGB565;
//		}
//	}
//	
//	switch(pixelFormat) {          
//		case kCCTexture2DPixelFormat_RGBA8888:
//		case kCCTexture2DPixelFormat_RGBA4444:
//		case kCCTexture2DPixelFormat_RGB5A1:
//		case kCCTexture2DPixelFormat_RGB565:
//		case kCCTexture2DPixelFormat_A8:
//			tempData = (unsigned char*)(this->getData());
//			CCAssert(tempData != NULL, "NULL image data.");
//			
//			if(this->getWidth() == (short)POTWide && this->getHeight() == (short)POTHigh)
//			{
//				data = new unsigned char[POTHigh * POTWide * 4];
//				memcpy(data, tempData, POTHigh * POTWide * 4);
//			}
//			else
//			{
//				data = new unsigned char[POTHigh * POTWide * 4];
//				memset(data, 0, POTHigh * POTWide * 4);
//				
//				unsigned char* pPixelData = (unsigned char*) tempData;
//				unsigned char* pTargetData = (unsigned char*) data;
//				
//				int imageHeight = this->getHeight();
//				for(int y = 0; y < imageHeight; ++y)
//				{
//					memcpy(pTargetData+POTWide*4*y, pPixelData+(this->getWidth())*4*y, (this->getWidth())*4);
//				}
//			}
//			
//			break;    
//		case kCCTexture2DPixelFormat_RGB888:
//			tempData = (unsigned char*)(this->getData());
//			CCAssert(tempData != NULL, "NULL image data.");
//			if(this->getWidth() == (short)POTWide && this->getHeight() == (short)POTHigh)
//			{
//				data = new unsigned char[POTHigh * POTWide * 3];
//				memcpy(data, tempData, POTHigh * POTWide * 3);
//			}
//			else
//			{
//				data = new unsigned char[POTHigh * POTWide * 3];
//				memset(data, 0, POTHigh * POTWide * 3);
//				
//				unsigned char* pPixelData = (unsigned char*) tempData;
//				unsigned char* pTargetData = (unsigned char*) data;
//				
//				int imageHeight = this->getHeight();
//				for(int y = 0; y < imageHeight; ++y)
//				{
//					memcpy(pTargetData+POTWide*3*y, pPixelData+(this->getWidth())*3*y, (this->getWidth())*3);
//				}
//			}
//			break;   
//		default:
//			CCAssert(0, "Invalid pixel format");
//	}
//	
//	// Repack the pixel data into the right format
//	
//	if(pixelFormat == kCCTexture2DPixelFormat_RGB565) {
//		//Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGGBBBBB"
//		tempData = new unsigned char[POTHigh * POTWide * 2];
//		inPixel32 = (unsigned int*)data;
//		outPixel16 = (unsigned short*)tempData;
//		
//		unsigned int length = POTWide * POTHigh;
//		for(unsigned int i = 0; i < length; ++i, ++inPixel32)
//		{
//			*outPixel16++ = 
//			((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) |  // R
//			((((*inPixel32 >> 8) & 0xFF) >> 2) << 5) |   // G
//			((((*inPixel32 >> 16) & 0xFF) >> 3) << 0);   // B
//		}
//		
//		delete [] data;
//		data = tempData;
//	}
//	else if (pixelFormat == kCCTexture2DPixelFormat_RGBA4444) {
//		//Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRGGGGBBBBAAAA"
//		tempData = new unsigned char[POTHigh * POTWide * 2];
//		inPixel32 = (unsigned int*)data;
//		outPixel16 = (unsigned short*)tempData;
//		
//		unsigned int length = POTWide * POTHigh;
//		for(unsigned int i = 0; i < length; ++i, ++inPixel32)
//		{
//			*outPixel16++ = 
//			((((*inPixel32 >> 0) & 0xFF) >> 4) << 12) | // R
//			((((*inPixel32 >> 8) & 0xFF) >> 4) << 8) | // G
//			((((*inPixel32 >> 16) & 0xFF) >> 4) << 4) | // B
//			((((*inPixel32 >> 24) & 0xFF) >> 4) << 0); // A
//		}
//		
//		delete [] data;
//		data = tempData;
//	}
//	else if (pixelFormat == kCCTexture2DPixelFormat_RGB5A1) {
//		//Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGBBBBBA"
//		tempData = new unsigned char[POTHigh * POTWide * 2];
//		inPixel32 = (unsigned int*)data;
//		outPixel16 = (unsigned short*)tempData;
//		
//		unsigned int length = POTWide * POTHigh;
//		for(unsigned int i = 0; i < length; ++i, ++inPixel32)
//		{
//			*outPixel16++ = 
//			((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) | // R
//			((((*inPixel32 >> 8) & 0xFF) >> 3) << 6) | // G
//			((((*inPixel32 >> 16) & 0xFF) >> 3) << 1) | // B
//			((((*inPixel32 >> 24) & 0xFF) >> 7) << 0); // A
//		}
//		
//		delete []data;
//		data = tempData;
//	}
//	else if (pixelFormat == kCCTexture2DPixelFormat_A8)
//	{
//		// fix me, how to convert to A8
//		pixelFormat = kCCTexture2DPixelFormat_RGBA8888;
//		
//		/*
//		 * The code can not work, how to convert to A8?
//		 *
//		 tempData = new unsigned char[POTHigh * POTWide];
//		 inPixel32 = (unsigned int*)data;
//		 outPixel8 = tempData;
//		 
//		 unsigned int length = POTWide * POTHigh;
//		 for(unsigned int i = 0; i < length; ++i, ++inPixel32)
//		 {
//		 *outPixel8++ = (*inPixel32 >> 24) & 0xFF;
//		 }
//		 
//		 delete []data;
//		 data = tempData;
//		 */
//	}
//	
//	if (data)
//	{
//		CC_SAFE_DELETE_ARRAY(m_pData);
//		m_pData = data;
//	}
//	return true;	
//}

bool CCImage::_initWithRawData(void *pData, int nDatalen, int nWidth, int nHeight, int nBitsPerComponent)
{
    bool bRet = false;
	do 
	{
		CC_BREAK_IF(0 == nWidth || 0 == nHeight);
        
		m_nBitsPerComponent = nBitsPerComponent;
		m_nHeight   = (short)nHeight;
		m_nWidth    = (short)nWidth;
		m_bHasAlpha = true;
        
		// only RGBA8888 surported
		int nBytesPerComponent = 4;
		int nSize = nHeight * nWidth * nBytesPerComponent;
		m_pData = new unsigned char[nSize];
		CC_BREAK_IF(! m_pData);
		memcpy(m_pData, pData, nSize);
        
		bRet = true;
	} while (0);
	return bRet;
}


bool CCImage::initWithImageData(void * pData, 
                                int nDataLen, 
                                EImageFormat eFmt,
                                int nWidth,
                                int nHeight,
                                int nBitsPerComponent)
{
    bool bRet = false;
    tImageInfo info = {0};
    do 
    {
        CC_BREAK_IF(! pData || nDataLen <= 0);
        if (eFmt == kFmtRawData)
        {
            bRet = _initWithRawData(pData, nDataLen, nWidth, nHeight, nBitsPerComponent);
        }
        else // init with png or jpg file data
        {
            bRet = _initWithData(pData, nDataLen, &info, m_dScaleX, m_dScaleY);
            if (bRet)
            {
                m_nHeight = (short)info.height;
                m_nWidth = (short)info.width;
                m_nBitsPerComponent = info.bitsPerComponent;
                m_bHasAlpha = info.hasAlpha;
                m_bPreMulti = info.isPremultipliedAlpha;
                m_pData = info.data;
            }
        }
    } while (0);
    
    return bRet;
}

bool CCImage::initWithString(
                             const char * pText, 
                             int         nWidth /* = 0 */, 
                             int         nHeight /* = 0 */,
                             ETextAlign eAlignMask /* = kAlignCenter */,
                             const char * pFontName /* = nil */,
                             int         nSize /* = 0 */)
{
    tImageInfo info = {0};
    info.width = nWidth;
    info.height = nHeight;
    
    bool bRet = false;
    if (!_initWithString(pText, eAlignMask, pFontName, nSize, &info))
    {
        do
        {
            CC_BREAK_IF(! pText);
            
            BitmapDC &dc = sharedBitmapDC();
            
            const char* pFullFontName = CCFileUtils::fullPathFromRelativePath(pFontName);
            //CCLog("-----pText=%s and Font File is %s nWidth= %d,nHeight=%d",pText,pFullFontName,nWidth,nHeight);
            
            CC_BREAK_IF(! dc.getBitmap(pText, nWidth, nHeight, eAlignMask, pFullFontName, nSize));
            //CCLog("---- dc.getBitmap is Succesfull... \n");
            
            // assign the dc.m_pData to m_pData in order to save time
            m_pData = dc.m_pData;
            CC_BREAK_IF(! m_pData);
            
            m_nWidth = (short)dc.iMaxLineWidth;
            m_nHeight = (short)dc.iMaxLineHeight;
            m_bHasAlpha = true;
            m_bPreMulti = true;
            m_nBitsPerComponent = 8;
            
            bRet = true;
            
            dc.reset();
        }while (0);
        
        //do nothing
        return bRet;
    }
    
    m_nHeight = (short)info.height;
    m_nWidth = (short)info.width;
    m_nBitsPerComponent = info.bitsPerComponent;
    m_bHasAlpha = info.hasAlpha;
    m_bPreMulti = info.isPremultipliedAlpha;
    m_pData = info.data;
    
    return true;
}

bool CCImage::saveToFile(const char *pszFilePath, bool bIsToRGB)
{
		
	return true;
}
NS_CC_END;

