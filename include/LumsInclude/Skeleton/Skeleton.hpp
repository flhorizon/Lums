/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    Skeleton.hpp                                   oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#ifndef LUMS_SKELETON_HPP
#define LUMS_SKELETON_HPP

#include <LumsInclude/Skeleton/SkeletonPose.hpp>
#include <LumsInclude/Skeleton/SkeletonData.hpp>
#include <LumsInclude/ExportDll.hpp>

namespace lm
{
    class Skeleton : public SkeletonPose
    {
    public:
        LUMS_EXPORTED Skeleton();
        LUMS_EXPORTED explicit Skeleton(const SkeletonData& data);

        LUMS_EXPORTED void    setToPose();
        void    setFlip(bool flip) { _flipX = flip; };
        bool    flip() const { return _flipX; }
        int     frame() const { return _frame; }
        LUMS_EXPORTED void    setAnimation(const char* animation, bool loop = false, bool repeat = false, int interpolation = 0);
        LUMS_EXPORTED void    setAnimation(size_t animation, bool loop = false, bool repeat = false, int interpolation = 0);
        size_t  animation() const { return _animationHash; }
        void    transformBone(Matrix4f& matrix, int bone) const;
        void    transformSkin(Matrix4f& matrix, int skin) const;
        LUMS_EXPORTED void    update();
        LUMS_EXPORTED void    updateAnimation();
        LUMS_EXPORTED void    applyIk(int target, int bone);
        LUMS_EXPORTED void    applyIk(int target, int parent, int child, float duration);
        size_t  event() const { return _event; }
        bool    finished() const { return _finished; }

    private:
        const SkeletonData*             _data;
        int                             _frame;
        const SkeletonAnimation*        _animation;
        size_t                          _animationHash;
        size_t                          _event;
        bool                            _flipX;
        bool                            _finished;
        bool                            _loop;
        int                             _interpolationLength;
        bool                            _interpolating;
        int                             _interpolationAcc;
    };
}

#endif
