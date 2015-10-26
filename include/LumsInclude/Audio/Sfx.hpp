/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    Sfx.hpp                                        oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#ifndef LUMS_SFX_HPP
#define LUMS_SFX_HPP

#include <vector>
#include <LumsInclude/Audio/Sound.hpp>
#include <LumsInclude/ExportDll.hpp>

namespace lm
{
    /**
     * @brief A class used to represent sound effect
     */
    class Sfx : public Sound
    {
    public:

        static LUMS_EXPORTED void    setGlobalVolume(float volume);

        /**
         * Create an empty sound effect
         */
        LUMS_EXPORTED Sfx();

        /**
         * Deleted ctor
         */
        Sfx(const Sfx&) = delete;

        /**
         * Move-construct ctor
         * @param rhs Source sound effect
         */
        LUMS_EXPORTED Sfx(Sfx&& rhs);

        /**
         * Deleted assignment operator
         */
        Sfx&  operator=(const Sfx&) = delete;

        /**
         * Move-assign a sound effect
         * @param rhs Source sound effect
         */
        LUMS_EXPORTED Sfx&  operator=(Sfx&& rhs);

        /**
         * Play the loaded sound effect
         * @param pos Source position
         */
        LUMS_EXPORTED void    play(Vector3f pos = {0.f, 0.f, 0.f});

        /**
         * Pause the loaded sound effect
         */
        LUMS_EXPORTED void    pause();

        /**
         * Stop the loaded sound effect
         */
        LUMS_EXPORTED void    stop();

        /**
         * Set the volume of the loaded sound effect
         * @param volume The volume, 0 being silent and 1 the loudest
         */
        LUMS_EXPORTED void    setVolume(float volume);

        LUMS_EXPORTED void    clearSources();

        /**
         * Sound dtor
         */
        LUMS_EXPORTED ~Sfx();

    private:
        std::vector<ALuint> _sources;
    };

    using SfxProvider = StreamProvider<Sfx>;
}

#endif