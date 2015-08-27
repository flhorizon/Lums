/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    WinNT/Application.hpp                          oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#include <LumsInclude/Application.hpp>

namespace lm {
	namespace Application {

		// Application::init() is OSX/Cocoa-specific and does nothing on WinNT.
		void
		lm::Application::init()
		{

		}
	}
}