//
//  Predefine.h
//  TVPortable
//
//  Created by Ruiwei Bu on 6/17/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#ifndef TVPortable_Visual_Prerequisites_h
#define TVPortable_Visual_Prerequisites_h

#include "../Preprocessor.h"

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

TVP_NS_VISUAL_END

#endif
