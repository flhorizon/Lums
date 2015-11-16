/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    WinNT/WinNT.cpp                                oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include "AtlConv.h"
#include "AtlBase.h"
#include "Shlobj.h"
#include <LumsInclude/OperatingSystem.hpp>

namespace lm
{
    // Store UTF-8 for proper std:: C++ and convert back to UTF-16 on WIN32 calls.
    static std::string  res_path;
    static std::string  appsupport_path;

    template<typename StringT> static void
    normalizePath(StringT& path
        , typename StringT::value_type cFind = static_cast<typename StringT::value_type>('\\')
        , typename StringT::value_type cRepl = static_cast<typename StringT::value_type>('/')
    )
    {
        for (;;)
        {
            size_t pos = path.find_first_of(cFind);

            if (pos != StringT::npos)
                path[pos] = cRepl;
            else
                break;
        }
    }

    const std::string&
    resourcePath()
    {
		USES_CONVERSION;
        if (res_path.empty())
        {
            wchar_t 		wcPath[MAX_PATH];
			std::wstring	wcPathStr;

            GetModuleFileNameW(nullptr, wcPath, MAX_PATH);
			
            wcPathStr = wcPath;
			wcPathStr = wcPathStr.substr(0, wcPathStr.find_last_of(L'\\') + 1) + L"resources\\";
			normalizePath(wcPathStr);
			
			// Convenient ATL conversion macro.
			// The API conversion function `WideCharToMultiByte' 
			// has a convoluted behaviour (ambiguous return values...).
			// Converting UTF-16 wchar_t to ANSI char using UTF-8 codepage (producing UTF-8 altogether).
			// Asian-proof.
			res_path = CW2A(wcPathStr.c_str(), CP_UTF8); 
        }
        return res_path;
    }
	
	// Returns nullptr if the application path has not been set.
	const std::string&
    userDataPath()
	{
        USES_CONVERSION;
        if (!appsupport_path.empty())
        {
            return appsupport_path;
        }

        std::wstring		wsAppDir;
        PWSTR            	oPath;
        HRESULT 			res;

        res = SHGetKnownFolderPath(
            FOLDERID_RoamingAppData				// Query user's AppData\Roaming folder
            , KF_FLAG_DEFAULT_PATH				// Just get the default path.
            , nullptr							// On the behalf of the current user
            , &oPath							// Output. Needs to be CoTaskMemFree'd.
            );

        if (res != S_OK)
        {
            MessageBoxA(nullptr, "Failed to locate %APPDATA%\\Roaming", "Lums error", MB_OK|MB_ICONERROR);
        }
        else
        {
            wsAppDir = std::wstring(oPath) + std::wstring(L"\\");
            CoTaskMemFree(oPath);
            appsupport_path = CW2A(wsAppDir.c_str(), CP_UTF8);
            normalizePath(appsupport_path);
        }
		return appsupport_path;
	}
	
	static bool
	directoryExists(LPCWSTR cwPath)
	{
		DWORD dwAttrib = GetFileAttributesW(cwPath);
		
		return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
				(dwAttrib & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
	}
	
	// Returns 0 on success, as proper POSIX calls
	// and contrary to most WIN32 API calls.
	// Returns non-zero on error.
	int
    mkDir(const char* dirPath)
	{
        USES_CONVERSION;
        std::wstring    wsPath(CA2W(dirPath, CP_UTF8));

        normalizePath(wsPath, L'/', L'\\');

        // Nothing to do; success.
        if (directoryExists(wsPath.c_str()))
            return 0;

        // Create the directory
		if (0 == CreateDirectoryW(wsPath.c_str(), nullptr))
		{
            std::wstring msg(L"Failed to create " + wsPath);

            MessageBoxW(nullptr, msg.c_str(), L"Lums error", MB_OK|MB_ICONERROR);
			return -1;
		}

		return 0;
	}
}
