/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    WinNT/WinNT.cpp                            oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "AtlConv.h"
#include "AtlBase.h"
#include "Shlobj.h"
#include <LumsInclude/OperatingSystem.hpp>

// Store UTF-8 for proper std:: C++ and convert back to UTF-16 on WIN32 calls.
static std::string  res_path;
static std::string  appsupport_path;

namespace lm
{
    template<typename StringT> static void
    normalizePath(StringT& path)
    {
        using AChar = StringT::value_type;
        const AChar slash = static_cast<AChar>('/');
        const AChar bslash = static_cast<AChar>('\\');

        for (;;)
        {
            size_t pos = path.find_first_of(bslash);

            if (pos != StringT::npos)
                path[pos] = slash;
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
            HMODULE hModule = GetModuleHandle(NULL);

            GetModuleFileNameW(hModule, wcPath, MAX_PATH);
			
			wcPathStr = wcPath;
            wcPathStr = wcPathStr.substr(0, wcPathStr.find_last_of(L'\\') + 1);
            normalizePath<std::wstring>(wcPathStr);
			
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
	const std::string
    appSupportPath()
	{
		return appsupport_path;
	}
	
	static BOOL
	directoryExists(LPCWSTR cwPath)
	{
		DWORD dwAttrib = GetFileAttributesW(cwPath);
		
		return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
				(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}
	
	// Returns 0 on success, as proper POSIX calls
	// and contrary to most WIN32 API calls.
	// Returns non-zero on error.
	int
	mkAppDataDir(const char *app)
	{
		USES_CONVERSION;
		std::wstring		dirPath(CA2W(app, CP_UTF8));
		PWSTR            	oPath;
		HRESULT 			res;

		// Nothing to do; success.
		if (directoryExists(dirPath.c_str()))
			return 0;

		res = SHGetKnownFolderPath(
			FOLDERID_RoamingAppData				// Query user's AppData\Roaming folder
			, KF_FLAG_DEFAULT_PATH				// Just get the default path.
			, nullptr							// On the behalf of the current user
			, &oPath							// Output. Needs to be CoTaskMemFree'd.
			);

		if (res != S_OK)
		{
			perror("SHGetKnownFolderPath");
			return -1;
		}

		dirPath = std::wstring(oPath) + std::wstring(L"\\") + dirPath;
		CoTaskMemFree(oPath);

        normalizePath<std::wstring>(dirPath);
		appsupport_path = CW2A(dirPath.c_str(), CP_UTF8);

		// This one returns 0 on error. RLY.
		if (CreateDirectoryW(dirPath.c_str(), nullptr) == 0)
		{
			perror("CreateDirectoryW");
			return -1;
		}

		return 0;
	}
}
