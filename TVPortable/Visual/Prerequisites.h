//
//  Predefine.h
//  TVPortable
//
//  Created by Ruiwei Bu on 6/17/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#ifndef TVPortable_Visual_Prerequisites_h
#define TVPortable_Visual_Prerequisites_h

#include "../Predefine.h"

#define TVP_NS_VISUAL_BEGIN \
namespace TVPortable { \
    namespace Visual { \

#define TVP_NS_VISUAL_END \
    } \
} // namespace TVPortable

TVP_NS_VISUAL_BEGIN

    class Font;
    class MenuItem;
    class Window;
    class Layer;

    /* 
     H** on Windows,
       */
    typedef uintPtr Handle;

    // affine types used in Layer::affineCopy/Pie/Blend
    enum AffineType {
        stNearest,
        stFastLinear,
        stLinear,
        stCubic,
    };

    enum ShiftSigns {
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

    enum FaceMode {
        dfAlpha     = 1U << 0,
        dfAlphaAdd  = 1U << 1,
        dfBoth      = 1U << 2,
        dfOpaque    = 1U << 3,
        dfMask      = 1U << 4,
        dfProvince  = 1U << 5,
        dfAuto      = 1U << 6,
        dfMain      = 1U << 7,
    };

    enum {
        ltOpaque,
        ltAlpha,
        ltAddApha,
        ltAdditive,
        ltSubtractive,
        ltMultiplicative,
        ltDodge,
        ltLighten,
        ltDarken,
        ltScreen,
        ltPsNormal,
        ltPsAdditive,
        ltPsSubtractive,
        ltPsMultiplicative,
        ltPsScreen,
        ltPsOverlay,
        lsPsHardLight,
        ltPsSoftLight,
        lsPsColorDodge,
        ltPsColorDodge5,
        ltPsColorBurn,
        lsPsLighten,
        lsPsDarken,
        lsPsDifference,
        lsPsDifference5,
        ltPsExclusion,
    };

    enum FontFlags {
        fsfFixedPitch,
        fsfSameCharSet,
        fsfNoVertical,
        fsfTrueTypeOnly,
        fsfUseFontFace,
    };

    enum OperateMode {
        omAuto,
        omPsNormal,
        omPsAddictive,
        omPsSubtractive,
        omPsMultiplicative,
        omPsScreen,
        omPsOverlay,
        omPsHardLight,
        omPsSoftLight,
        omPsColorDodge,
        omPsColorDodge5,
        omPsColorBurn,
        omPsLighten,
        omPsDarken,
        omPsDifference,
        omPsExclusion,
        omAddictive,
        omSubtractive,
        omMultiplicative,
        omDodge,
        omDarken,
        omLighten,
        omScreen,
        omAlpha,
        omAddAlpha,
        omOpaque,
    };

    /* flags on windows
     see http://msdn.microsoft.com/en-us/library/windows/desktop/ms648002(v=vs.85).aspx  for details
     may have different behaviours or not available on other platforms*/
    enum PopupMenuFlags {
        tpmCenterAlign      = 0x0004L,
        tpmLeftAlign        = 0x0000L,
        tpmRightAlign       = 0x008L,
        tpmBottomAlign      = 0x0020L,
        tomTopAlign         = 0x0000L,
        tpmVCenterAlign     = 0x0010L,
        tpmNoNotify         = 0x0080L,
        tpmReturnCmd        = 0x0100L,
        tpmLeftButton       = 0x0000L,
        tpmRightButton      = 0x0002L,
        tpmHorNegAnimation  = 0x0800L,
        tpmHorPosAnimation  = 0x0400L,
        tpmNoAnimation      = 0x4000L,
        tpmVerNegAnimation  = 0x2000L,
        tpmVerPosAnimation  = 0x1000L,
    };

TVP_NS_VISUAL_END

#endif
