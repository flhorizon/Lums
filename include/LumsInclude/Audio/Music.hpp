/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    Music.hpp                                      oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#ifndef LUMS_MUSIC_HPP
#define LUMS_MUSIC_HPP

#include <mutex>
#include <LumsInclude/Audio/Sound.hpp>
#include <LumsInclude/ExportDll.hpp>

#define NB_BUFFERS  3

namespace lm
{
    /**
     * @brief A class used to represent music
     */
    class Music : public Sound
    {
    public:

        static LUMS_EXPORTED void     setGlobalVolume(float volume);
        /**
         * Create an empty music
         */
        LUMS_EXPORTED Music();

        /**
         * Deleted ctor
         */
        Music(const Music&) = delete;
        
        /**
         * Move-construct a music
         * @param rhs Source music
         */
        LUMS_EXPORTED Music(Music&& rhs);

        /**
         * Deleted assignment operator
         */
        Music&  operator=(const Music&) = delete;

        /**
         * Move-assign a music
         * @param rhs Source music
         */
        LUMS_EXPORTED Music&  operator=(Music&& rhs);

        /**
         * Play the loaded music
         * @param pos Source position
         */
        LUMS_EXPORTED void    play(Vector3f pos = {0.f, 0.f, 0.f});

        /**
         * Pause the loaded music
         */
        LUMS_EXPORTED void    pause();

        /**
         * Stop the loaded music
         */
        LUMS_EXPORTED void    stop();

        /**
         * Set the volume of loaded music
         * @param volume The volume, 0 being silent and 1 the loudest
         */
        LUMS_EXPORTED void    setVolume(float volume);

        LUMS_EXPORTED void   updateVolume();
        
        /**
         * Music dtor
         */
        LUMS_EXPORTED ~Music();

    private:
        enum State
        {
            Playing,
            Paused,
            Stopped
        };
        void    state(Music::State state, std::function<void (void)> func);
        void    streamOGG(Vector3f pos);
        void    bufferizeOGG(ALuint& buffer);

        ALuint          _source;
        Music::State    _state;
        std::mutex      _mtx;
    };

    using MusicProvider = StreamProvider<Music>;
}

#endif