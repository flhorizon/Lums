/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    SkeletonBlueprint.hpp                          oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#ifndef LUMS_SKELETON_BLUEPRINT_HPP
#define LUMS_SKELETON_BLUEPRINT_HPP

#include <LumsInclude/Provider.hpp>
#include <LumsInclude/Binary/BObject.hpp>
#include <LumsInclude/Skeleton/SkeletonData.hpp>
#include <LumsInclude/Skeleton/Skeleton.hpp>

namespace lm
{
    class SkeletonBlueprint
    {
    public:
        LUMS_EXPORTED SkeletonBlueprint();
        LUMS_EXPORTED void        loadBinary(const BObject& object);
        LUMS_EXPORTED void        load(const std::string& path, bool resource = true);
        LUMS_EXPORTED Skeleton    create() const;
        ~SkeletonBlueprint();

    private:
        SkeletonData    _data;
    };

    using SkeletonProvider = Provider<SkeletonBlueprint>;
}

#endif
