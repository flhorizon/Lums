/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    OperatingSystem.hpp                            oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#ifndef LUMS_OPERATINGSYSTEM_HPP
#define LUMS_OPERATINGSYSTEM_HPP

#include <string>
#include <LumsInclude/ExportDll.hpp>

namespace lm
{
    /**
     * Get the program resource path.
     * On Mac OS X, this is the Resource folder if the program is a bundle.
     * Else, it's the folder containing the executable.
     * @return A const reference to a resource path string.
     */
    LUMS_EXPORTED const std::string&  resourcePath();

    /**
     * Get the user's application data path.
     * On Mac OS X, this is $HOME/Applications/Application Support/Forsaken Souls.
     * @return A const reference to the application data path string.
     */
    LUMS_EXPORTED const std::string&  userDataPath();

    /**
     * Try to create a directory corresponding to the given `dirPath'.
     * @return 0 on success, -1 on error.
     */
    LUMS_EXPORTED int   mkDir(
                                const char* dirPath //!< Path to create the directory at.
                            );
}

#endif
