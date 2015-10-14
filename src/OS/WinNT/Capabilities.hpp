/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    WinNT/Capabilities.hpp                         oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAPABILITIES_HPP
#define CAPABILITIES_HPP

#include <vector>

namespace lm {
    namespace winnt {
        
        struct DisplaySetting { short width, height, bpp, freq; };
       
        class Capabilities
        {
        public:
            Capabilities();
            Capabilities(const Capabilities&) = delete;
            Capabilities(Capabilities&& src);
            Capabilities& operator=(Capabilities&& src);
			bool exists(const DisplaySetting& sg, bool checkFrequency = false);
            const std::vector<DisplaySetting>& settings() { return _settings; }
            DisplaySetting& current(DisplaySetting& sgOut);
            ~Capabilities();

        private:
            std::vector<DisplaySetting> _settings;
        };
    }
}

#endif // CAPABILITIES_HPP