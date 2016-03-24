/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    SkeletonBlueprint.cpp                          oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#include <LumsInclude/OperatingSystem.hpp>
#include <LumsInclude/Skeleton/SkeletonBlueprint.hpp>

#include <LumsInclude/Singleton_internal_inline.hpp>
LM_I_FORCE_SINGLETON_INSTANCE(SkeletonProvider, lm::SkeletonProvider)

using namespace lm;

SkeletonBlueprint::SkeletonBlueprint()
{

}

void
SkeletonBlueprint::loadBinary(const BObject& object)
{
    std::string path = object["path"].asString();
    load(path);
}

void
SkeletonBlueprint::load(const std::string& path, bool resource)
{
    std::string fpath = resource ? resourcePath() + path : path;
    std::ifstream file(fpath, std::ios::binary);
    _data.loadFromFile(file);
}

Skeleton
SkeletonBlueprint::create() const
{
    return Skeleton(_data);
}

SkeletonBlueprint::~SkeletonBlueprint()
{

}
