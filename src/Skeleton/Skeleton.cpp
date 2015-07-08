/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    Skeleton.cpp                                   oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#include <cstdio>
#include <cstdint>
#include <fstream>
#include <LumsInclude/Skeleton/Skeleton.hpp>
#include <LumsInclude/OperatingSystem.hpp>

using namespace lm;

Skeleton::Skeleton()
{

}

void
Skeleton::load(const std::string& path, bool resource)
{
    std::string fpath = resource ? resourcePath() + path : path;

    std::ifstream file(fpath);
    uint32_t boneCount;
    file.read((char*)&boneCount, 4);
    for (unsigned i = 0; i < boneCount; ++i)
    {
        int32_t nameLen;
        char* name;
        int32_t parent;
        lm::Vector3f position;
        lm::Vector2f scale;
        float rotation;

        file.read((char*)&nameLen, 4);
        name = new char[nameLen + 1];
        name[nameLen] = 0;
        delete [] name;
        file.read(name, nameLen);
        file.read((char*)&parent, 4);
        file.read((char*)&position.x, sizeof(float));
        file.read((char*)&position.y, sizeof(float));
        file.read((char*)&scale.x, sizeof(float));
        file.read((char*)&scale.y, sizeof(float));
        file.read((char*)&rotation, sizeof(float));

        position.y *= -1;

        Bone b(parent);
        b.rotate(rotation);
        b.translate(position);
        _bones.push_back(b);
        if (parent != -1)
            _bones[parent].addChild(i);
    }

    uint32_t skinCount;
    file.read((char*)&skinCount, 4);
    for (unsigned i = 0; i < skinCount; ++i)
    {
        int bone;
        int texture;
        lm::Vector3f position;
        float rotation;

        file.read((char*)&texture, 4);
        file.read((char*)&bone, 4);
        file.read((char*)&position.x, sizeof(float));
        file.read((char*)&position.y, sizeof(float));
        file.read((char*)&rotation, sizeof(float));

        position.y *= -1;

        Skin s(bone, texture);
        s.rotate(rotation);
        s.translate(position);
        _skins.push_back(s);
    }
}

void
Skeleton::loadBinary(const BObject& object)
{
    load(object["path"].asString());
}
