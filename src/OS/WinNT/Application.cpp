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

#define OGL_LOADER_REQUIRE
#include <gl/gl_core_3_2.h>	
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <LumsInclude/Application.hpp>
#include <LumsInclude/Graphics/Window.hpp>

namespace lm {
	namespace Application {

        static void
        debugShowStatus(const std::string&& msg)
        {
            MessageBoxA(NULL, msg.c_str(), "Lums Debug", MB_OK);
        }

		void
		init()
		{
            lm::Window dummyWindow(128, 32, "Initializing Lums...", false);
			int res = ogl_LoadFunctions();

			if (res == ogl_LOAD_FAILED)
			{
				fprintf(stderr, "[Lums][Critical] Failed to load OpenGL functions.\r\n");
				debugShowStatus("glLoader failed entierly !");
				exit(EXIT_FAILURE);
			}
			else if (res > ogl_LOAD_SUCCEEDED)
			{
                char msg[1024];

                snprintf(msg, sizeof(msg), "[Lums] Failed to load %i OpenGL functions.\r\n", res - ogl_LOAD_SUCCEEDED);
				fprintf(stderr, "%s", msg);
                std::thread(&debugShowStatus, msg).detach();
			}
		}
	}
}