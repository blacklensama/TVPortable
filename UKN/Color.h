//
//  Color.h
//  Project Unknown
//
//  Created by Robert Bu on 11/23/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_Color_h
#define Project_Unknown_Color_h

#include "UKN/Platform.h"
#include "UKN/MathUtil.h"

namespace ukn {
    
#define COLOR_ARGB(a,r,g,b)      ((uint32(a)<<24) + (uint32(r)<<16) + (uint32(g)<<8) + uint32(b))
#define COLOR_GETA(col)          (((col)>>24) & 0xFF)
#define COLOR_GETR(col)          (((col)>>16) & 0xFF)
#define COLOR_GETG(col)          (((col)>>8) & 0xFF)
#define COLOR_GETB(col)          ((col) & 0xFF)
#define COLOR_SETA(col,a)	 	 (((col) & 0x00FFFFFF) + (uint32(a)<<24))
#define COLOR_SETR(col,r)	  	 (((col) & 0xFF00FFFF) + (uint32(r)<<16))
#define COLOR_SETG(col,g)		 (((col) & 0xFFFF00FF) + (uint32(g)<<8))
#define COLOR_SETB(col,b)		 (((col) & 0xFFFFFF00) + uint32(b))
    
    class Color {
    public:
        Color():
        r(1.f),
        g(1.f),
        b(1.f),
        a(1.f) {}
        
        Color(int32 _r, int32 _g, int32 _b):
        r(_r / 255.0f),
        g(_g / 255.0f),
        b(_b / 255.0f),
        a(1.0f) {
            normalize();
        }
        
        Color(float _r, float _g, float _b, float _a):
        r(_r), 
        g(_g), 
        b(_b), 
        a(_a) {
            normalize();
        }
        
        Color(uint32 col) { 
            *this = col;
        }
        
        void set(float _r, float _g, float _b, float _a) { 
            this->r = _r; 
            this->g = _g; 
            this->b = _b; 
            this->a = _a;
        }
        
        Color operator = (const Color& rhs) {
            this->r = rhs.r; 
            this->g = rhs.g; 
            this->b = rhs.b; 
            this->a = rhs.a; 
            return *this;
        }
        
        Color operator = (uint32 col) {
            this->a = (col>>24)/255.0f; 
            this->r = ((col>>16) & 0xFF)/255.0f; 
            this->g = ((col>>8) & 0xFF)/255.0f; 
            this->b = (col & 0xFF)/255.0f; 
            return *this;
        }
        
        Color& operator += (const Color& rhs) {
            this->r += rhs.r; 
            this->g += rhs.g; 
            this->b += rhs.b; 
            this->a += rhs.a; 
            return *this;
        }
        
        Color& operator -= (const Color& rhs) {
            this->r -= rhs.r; 
            this->g -= rhs.g; 
            this->b -= rhs.b; 
            this->a -= rhs.a; 
            return *this;
        }
        Color& operator *= (const Color& rhs) {
            this->r *= rhs.r; 
            this->g *= rhs.g; 
            this->b *= rhs.b; 
            this->a *= rhs.a; 
            return *this;
        }
        Color& operator /= (float s) {
            this->r /= s; 
            this->g /= s; 
            this->b /= s; 
            this->a /= s; 
            return *this;
        }
        Color& operator *= (float s) {
            this->r *= s; 
            this->g *= s; 
            this->b *= s; 
            this->a *= s; 
            return *this; 
        }
        
        Color operator - (const Color& rhs)	const {
            return Color(this->r-rhs.r, this->g-rhs.g, this->b-rhs.b, this->a-rhs.a);
        }
        
        Color operator + (float s)	const {
            return Color(this->r+s, this->g+s, this->b+s, this->a+s);
        }
        Color operator - (float s)	const {
            return Color(this->r-s, this->g-s, this->b-s, this->a-s);
        }
        Color operator * (float s)	const {
            return Color(this->r*s, this->g*s, this->b*s, this->a*s);
        }
        Color operator / (float s)	const {
            ukn_assert(s != 0.f);
            return Color(this->r/s, this->g/s, this->b/s, this->a/s);
        }
        Color operator * (const Color& rhs) const {
            return Color(this->r * rhs.r, 
                                 this->g * rhs.g, 
                                 this->b * rhs.b, 
                                 this->a * rhs.a);
        }
        
        
        Color operator + (const Color& rhs) const {
            return Color(this->r+rhs.r, 
                                 this->g+rhs.g, 
                                 this->b+rhs.b, 
                                 this->a+rhs.a);
        }
        
        bool operator == (const Color& rhs)	{
            return (this->r==rhs.r && this->g==rhs.g && this->b==rhs.b && this->a==rhs.a);
        }
        bool operator != (const Color& rhs)	{
            return !(*this == rhs);
        }
        
        uint32 toHWColor() const { 
            return ((unsigned int)(this->a*255.0f)<<24) + 
                    ((unsigned int)(this->r*255.0f)<<16) +
                    ((unsigned int)(this->g*255.0f)<<8) + 
                    (unsigned int)(this->b*255.0f);
        }
        
        static uint32 ToHWColor(float oR, float og, float ob, float oa)  { 
            return ((unsigned int)(oa*255.0f)<<24) + 
                    ((unsigned int)(oR*255.0f)<<16) +
                    ((unsigned int)(og*255.0f)<<8) + 
                    (unsigned int)(ob*255.0f);
        }
        
        static Color FromHWColor(uint32 col) {
            return Color(col);
        }
        
        operator uint32() const {
            return toHWColor();
        }
        
        Color& normalize() {
            this->r = clamp(0.f, 1.f, r);
            this->g = clamp(0.f, 1.f, g);
            this->b = clamp(0.f, 1.f, b);
            this->a = clamp(0.f, 1.f, a);
            return *this;
        }
        
        float r;
        float g;
        float b;
        float a;

    };

    namespace color {
        static Color White          = Color(1.f, 1.f, 1.f, 1.f);
        static Color Black          = Color(0.f, 0.f, 0.f, 1.f);
        static Color Red            = Color(1.f, 0.f, 0.f, 1.f);
        static Color Blue           = Color(0.f, 0.f, 1.f, 1.f);
        static Color Lime           = Color(0.f, 1.f, 0.f, 1.f);
        static Color Marron         = Color(0xFF800000);
        static Color Darkred        = Color(0xFF8B0000);
        static Color Crimson        = Color(0xFFDC143C);
        static Color Darkmagenta    = Color(0xFF8B008B);
        static Color Orchid         = Color(0xFFDA70D6);
        static Color Thistle        = Color(0xFFD8BFD8);
        static Color Plum           = Color(0xFFDDA0DD);
        static Color Violet         = Color(0xFFEE82EE);
        static Color Slateblue      = Color(0xFF6A5ACD);
        static Color Mediumslateblue= Color(0xFF7B68EE);
        static Color Darkblue       = Color(0xFF00008B);
        static Color Mediumblue     = Color(0xFF0000CD);
        static Color Navy           = Color(0xFF000080);
        static Color Royalblue      = Color(0xFF4169E1);
        static Color Lightsteelblue = Color(0xFFB0C4DE);
        static Color Aliceblue      = Color(0xFFF0F8FF);
        static Color Ghostblue      = Color(0xFFF8F8FF);
        static Color Lavender       = Color(0xFFE6E6FA);
        static Color Dodgerblue     = Color(0xFF1E90FF);
        static Color Steelblue      = Color(0xFF00BFFF);
        static Color Deepskyblue    = Color(0xFF4169E1);
        static Color Slategray      = Color(0xFF808090);
        static Color Lightskyblue   = Color(0xFF87CEFA);
        static Color Skyblue        = Color(0xFF87CEEB);
        static Color Lightblue      = Color(0xFFADD8E6);
        static Color Teal           = Color(0xFF008080);
        static Color Darkturquoise  = Color(0xFF00CED1);
        static Color Aqua           = Color(0x0000FFFF);
        static Color Mediumturquoise= Color(0xFF48D1CC);
        static Color Cadetblue      = Color(0xFF5F9EA0);
        static Color Paleturquoise  = Color(0xFFAFEEEE);
        static Color Lightcyan      = Color(0xFFE0FFFF);
        static Color Azure          = Color(0xFFF0FFFF);
        static Color Lightseagreen  = Color(0xFF20B2AA);
        static Color Turquoise      = Color(0xFFAFEEEE);
        static Color Powderblue     = Color(0xFF40E0D0);
        static Color Darkslategray  = Color(0xFF2F4F4F);
        static Color Aquamarine     = Color(0xFF7FFFD4);
        static Color Springgreen    = Color(0xFF00FF7F);
        static Color Mediumseagreen = Color(0xFF3CB371);
        static Color Seagreen       = Color(0xFF2E8B57);
        static Color Limegreen      = Color(0xFF32CD32);
        static Color Darkgreen      = Color(0xFF006400);
        static Color Green          = Color(0xFF008000);
        static Color Forestgreen    = Color(0xFF228B22);
        static Color Darkseagreen   = Color(0xFF8FBC8F);
        static Color Lightgreen     = Color(0xFF90EE90);
        static Color Palegreen      = Color(0xFF98FB98);
        static Color Mintcream      = Color(0xFFF5FFFA);
        static Color Honeydew       = Color(0xFFF0FFF0);
        static Color Chartreuse     = Color(0xFF7FFF00);
        static Color Olivedrab      = Color(0xFF6B8E23);
        static Color Darkolivegreen = Color(0xFF556B2F);
        static Color Yellowgreen    = Color(0xFF9ACD32);
        static Color Greenyellow    = Color(0xFFADFF2F);
        static Color Beige          = Color(0xFFF5F5DC);
        static Color Linen          = Color(0xFFFAF0E6);
        static Color Olive          = Color(0xFF808000);
        static Color Yellow         = Color(0xFFFFFF00);
        static Color Lightyellow    = Color(0xFFFFFFE0);
        static Color Ivory          = Color(0xFFFFFFF0);
        static Color Darkkhaki      = Color(0xFFBDB76B);
        static Color Khaki          = Color(0xFFF0E68C);
        static Color Palegold       = Color(0xFFEEE8AA);
        static Color Wheat          = Color(0xFFF6DEB3);
        static Color Gold           = Color(0xFFFFD700);
        static Color Lemonchiffon   = Color(0xFFFFFACD);
        static Color Papayawhip     = Color(0xFFFFEFD5);
        static Color Darkgold       = Color(0xFFB8860B);
        static Color Goldenrod      = Color(0xFFDAA520);
        static Color Antiquewhite   = Color(0xFFFAEBD7);
        static Color Cornsilk       = Color(0xFFFFF8DC);
        static Color Oldlace        = Color(0xFFFDF5E6);
        static Color Moccasin       = Color(0xFFFFE4B5);
        static Color Navajowhite    = Color(0xFFFFDEAD);
        static Color Orange         = Color(0xFFFFA500);
        static Color Bisque         = Color(0xFFFFE4CE);
        static Color Tan            = Color(0xFFD2B48C);
        static Color Darkorange     = Color(0xFFFF8C00);
        static Color Burlywood      = Color(0xFFDEB887);
        static Color Saddlebrown    = Color(0xFF8B4513);
        static Color Sandybrown     = Color(0xFFF4A460);
        static Color Blanchedalmond = Color(0xFFFFEBCD);
        static Color Lavenderblush  = Color(0xFFFFF0F5);
        static Color Seashell       = Color(0xFFFFF5EE);
        static Color Floralwhite    = Color(0xFFFFFAF0);
        static Color Snow           = Color(0xFFFFFAFA);
        static Color Peru           = Color(0xFFCD853F);
        static Color Peachpuff      = Color(0xFFFFDAB9);
        static Color Chocolate      = Color(0xFFD2691E);
        static Color Sienna         = Color(0xFFA0522D);
        static Color Lightsalmon    = Color(0xFFFFA07A);
        static Color Coral          = Color(0xFFFF7F50);
        static Color Darksalm       = Color(0xFFE9967A);
        static Color Mistyrose      = Color(0xFFFFE4E1);
        static Color Orangered      = Color(0xFFFF4500);
        static Color Salmon         = Color(0xFFFA8072);
        static Color Tomato         = Color(0xFFFF6347);
        static Color Rosybrow       = Color(0xFFBC8F8F);
        static Color Pink           = Color(0xFFFFC0CB);
        static Color Indianred      = Color(0xFFCD5C5C);
        static Color Lightcoral     = Color(0xFFF08080);
        static Color Brown          = Color(0xFFA52A2A);
        static Color Firebrik       = Color(0xFFB22222);
        static Color Dimgray        = Color(0xFF696969);
        static Color Gray           = Color(0xFF808080);
        static Color Darkgray       = Color(0xFFA9A9A9);
        static Color Silver         = Color(0xFFC0C0C0);
        static Color Lightgrey      = Color(0xFFD3D3D3);
        static Color Gainsboro      = Color(0xFFDCDCDC);
        static Color Whitesmoke     = Color(0xFFF5F5F5); 
    } // namespace color

    class ColorHSV {
    public:
        float h, s, v, a;
        
        ColorHSV(float _h, float _s, float _v, float _a):
        h(_h), 
        s(_s), 
        v(_v), 
        a(_a) {}
        
        ColorHSV(uint32 col) {
            fromHWColor(col);
        }
        
        bool operator ==(const ColorHSV& c) const;
        bool operator !=(const ColorHSV& c) const;
        
        void fromHWColor(uint32 col);
        uint32 toHWColor() const;
    };
    
    inline void ColorHSV::fromHWColor(uint32 col) {
        float r, g, b;
        float minv, maxv, delta;
        float del_R, del_G, del_B;
        
        this->a = (col>>24) / 255.0f;
        r = ((col>>16) & 0xFF) / 255.0f;
        g = ((col>>8)  & 0xFF) / 255.0f;
        b = (col       & 0xFF) / 255.0f;
        
        minv = std::min(std::min(r, g), b);
        maxv = std::max(std::max(r, g), b);
        delta = maxv - minv;
        
        this->v = maxv;
        
        if (delta == 0) {
            this->h = 0;
            this->s = 0;
        }
        else {
            this->s = delta / maxv;
            del_R = (((maxv - r) / 6) + (delta / 2)) / delta;
            del_G = (((maxv - g) / 6) + (delta / 2)) / delta;
            del_B = (((maxv - b) / 6) + (delta / 2)) / delta;
            
            if      (r == maxv) {this->h = del_B - del_G;}
            else if (g == maxv) {this->h = (1 / 3) + del_R - del_B;}
            else if (b == maxv) {this->h = (2 / 3) + del_G - del_R;}
            
            if (this->h < 0) this->h += 1;
            if (this->h > 1) this->h -= 1;
        }
    }
    
    inline uint32 ColorHSV::toHWColor() const {
        float r, g, b;
        float xh, i, p1, p2, p3;
        
        if (this->s == 0) {
            r = v;
            g = v;
            b = v;
        }
        else {
            xh = h * 6;
            if(xh == 6) xh=0;
            i = floorf(xh);
            p1 = this->v * (1 - this->s);
            p2 = this->v * (1 - this->s * (xh - i));
            p3 = this->v * (1 - this->s * (1 - (xh - i)));
            
            if      (i == 0) {r = v;  g = p3; b = p1;}
            else if (i == 1) {r = p2; g = v;  b = p1;}
            else if (i == 2) {r = p1; g = v;  b = p3;}
            else if (i == 3) {r = p1; g = p2; b = v; }
            else if (i == 4) {r = p3; g = p1; b = v; }
            else			 {r = v;  g = p1; b = p2;}
        }
        
        return (uint32(a*255.0f)<<24) + (uint32(r*255.0f)<<16) + (uint32(g*255.0f)<<8) + uint32(b*255.0f);
    }
    
    
} // namespace ukn


#endif
