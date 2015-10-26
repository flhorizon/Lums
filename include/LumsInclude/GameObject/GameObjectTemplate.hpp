/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    GameObjectTemplate.hpp                         oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#ifndef LUMS_GAME_OBJECT_TEMPLATE
#define LUMS_GAME_OBJECT_TEMPLATE

#include <string>
#include <unordered_map>
#include <LumsInclude/Binary/BValue.hpp>
#include <LumsInclude/GameObject/GameObject.hpp>
#include <LumsInclude/ExportDll.hpp>

namespace lm
{
    class GameObjectTemplate
    {
    public:
        LUMS_EXPORTED   GameObjectTemplate();
        LUMS_EXPORTED   void        load();
        LUMS_EXPORTED   void        loadBinary(const BObject& object);
        LUMS_EXPORTED   GameObject* operator()();
        LUMS_EXPORTED   ~GameObjectTemplate();

    private:
        std::unordered_map<size_t, BObject>     _components;
        std::vector<size_t>                     _tags;
    };
}

#endif
