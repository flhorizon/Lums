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

namespace lm
{
    /**
     * Get the program resource path.
     * On Mac OS X, this is the Resource folder if the program is a bundle.
     * Else, it's the folder containing the executable.
     * @return A const reference to a resource path string.
     */
    const std::string&  resourcePath();

    /**
     * Get the user's application data path.
     * On Mac OS X, this is $HOME/Applications/Application Support/Forsaken Souls.
     * @return A const reference to the application data path string.
     */
    const std::string&  userDataPath();

    int                 mkAppDataDir(const char * dirname);
}

#endif
