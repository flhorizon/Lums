/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    TranslationKeyFrame.hpp                        oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#ifndef LUMS_TRANSLATION_KEY_FRAME_HPP
#define LUMS_TRANSLATION_KEY_FRAME_HPP

#include <LumsInclude/Skeleton/KeyFrameCurve.hpp>

namespace lm
{
    struct TranslationKeyFrame
    {
        unsigned        frame;
        Vector2f        translation;
        KeyFrameCurve   curve;
    };
}

#endif
