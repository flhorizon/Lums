/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    WinNT/Capabilities.cpp                         oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#include "Capabilities.hpp"
#ifndef LEAN_AND_MEAN
#define LEAN_AND_MEAN 1
#endif
#include <windows.h>
#include <cstring>

namespace lm {
    namespace winnt {


        Capabilities::Capabilities()
        {
            DEVMODE dm;

            dm.dmSize = sizeof(DEVMODE);
            _settings.resize(0);
            for (int iModeNum = 0; TRUE == EnumDisplaySettings(nullptr, iModeNum, &dm); ++iModeNum)
            {
                _settings.push_back({
                    static_cast<short>(dm.dmPelsWidth)
                    , static_cast<short>(dm.dmPelsHeight)
                    , static_cast<short>(dm.dmBitsPerPel)
                    , static_cast<short>(dm.dmDisplayFrequency)
                });
            }
        }

        Capabilities::Capabilities(Capabilities&& src)
            : _settings(std::move(src._settings))
        {

        }

        Capabilities&
        Capabilities::operator=(Capabilities&& src)
        {
            if (&src != this)
                _settings = std::move(src._settings);
            return *this;
        }

        bool
        Capabilities::exists(const DisplaySetting& sg, bool checkFrequency)
        {
            for (auto& s : this->settings())
            {
                if (
                    s.width == sg.width
                    && s.height == sg.height
                    && s.bpp == sg.bpp
                    && (!checkFrequency || s.freq == sg.freq)
                    )
                {
                    return true;
                }
            }
            return false;
        }

        DisplaySetting&
        Capabilities::current(DisplaySetting& sgOut)
        {
            DEVMODE dm;

            dm.dmSize = sizeof(DEVMODE);
            std::memset(&sgOut, 0, sizeof(sgOut));
            if (TRUE == EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dm))
            {
                sgOut.width = static_cast<short>(dm.dmPelsWidth);
                sgOut.height = static_cast<short>(dm.dmPelsHeight);
                sgOut.bpp = static_cast<short>(dm.dmBitsPerPel);
                sgOut.freq = static_cast<short>(dm.dmDisplayFrequency);
            }
            return sgOut;
        }

        Capabilities::~Capabilities()
        {

        }
    }
}