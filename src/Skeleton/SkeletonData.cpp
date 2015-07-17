/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    SkeletonData.cpp                               oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#include <cstdint>
#include <LumsInclude/Skeleton/SkeletonData.hpp>

using namespace lm;

void
SkeletonData::loadFromFile(std::ifstream& file)
{
    pose.loadFromFile(file);
    uint32_t animCount;
    file.read((char*)&animCount, 4);
    for (unsigned i = 0; i < animCount; ++i)
    {
        char* name;
        uint32_t nameLen;
        file.read((char*)&nameLen, 4);
        name = new char[nameLen + 1];
        name[nameLen] = 0;
        file.read(name, nameLen);
        //animations[sym(name)].loadFromFile(file);
        delete [] name;
    }
}