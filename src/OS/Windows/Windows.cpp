/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    Windows/Windows.cpp                            oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#include <windows.h>
#include <LumsInclude/OperatingSystem.hpp>
#include <LumsInclude/Path.hpp>


namespace lm
{
    const path_t&
    resourcePath()
    {
        if (res_path.empty())
        {
            char path[MAX_PATH];
            HMODULE hModule = GetModuleHandle(NULL);

            GetModuleFileName(hModule, path, MAX_PATH);
            res_path = path;
            res_path = res_path.substr(0, res_path.find_last_of('\\') + 1);
        }
        return res_path;
    }
	
	const path_t&
	userDataPath()
	{
		if (res_path.empty())
		{
			// -> GetSpecialFolderPath(specialFolder)
			// with   ``specialFolder'' <- FOLDERID_RoamingAppData
		}
		return appdata_path;
	}
}
