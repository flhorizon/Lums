/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    Component.cpp                                  oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#include <unordered_map>
#include <LumsInclude/GameObject/Component.hpp>
#include <LumsInclude/Binary/BObject.hpp>

#include <LumsInclude/Singleton_internal_inline.hpp>

LM_I_FORCE_SINGLETON_INSTANCE(ComponentFactory, lm::ComponentFactory)
LM_I_FORCE_SINGLETON_INSTANCE(ComponentBindings, lm::ComponentBindings)


using namespace lm;

Component::Component()
{

}

void
Component::loadBinary(const BObject& object)
{
    for (auto& p : object)
    {
        void (Component::*setter)(const BValue&);

        setter = ComponentBindings::instance().getSetter(id(), p.first);
        (this->*setter)(p.second);
    }
}

Component::~Component()
{

}
