/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    Sound.hpp                                      oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#ifndef LUMS_SOUND_HPP
#define LUMS_SOUND_HPP

#include <string>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <vorbis/vorbisfile.h>
#include <LumsInclude/Math/Vector.hpp>
#include <LumsInclude/Provider_internal.hpp>

#define BUFFER_SIZE 40960

namespace lm
{
    /**
     * @cond
     */
    namespace Audio
    {
        LUMS_EXPORTED void    init();
    }
    /**
     * @endcond
     */

    /**
     * @brief An abstract class for sounds
     */
    class Sound
    {
    public:
        LUMS_EXPORTED Sound();
        
        LUMS_EXPORTED Sound(Sound&& rhs);

        LUMS_EXPORTED Sound& operator=(Sound&& rhs);
        /**
         * Set the loading path for the sound
         * @param name The file path
         * @param resource If true, path is relative to the resource path
         */
        LUMS_EXPORTED void  setPath(const std::string name, bool resource = true);

        LUMS_EXPORTED void  loadBinary(const BObject& object);

        /**
         * Load the sound
         */
        LUMS_EXPORTED void  load();

        /**
         * Check wether the sound is loaded
         * @return true if the sound is loaded, false otherwise
         */
        LUMS_EXPORTED bool  loaded();

        /**
         * Unload the sound
         */
        LUMS_EXPORTED void  unload();

        /**
         * Play a sound
         * @param pos The source position
         */
        virtual void         play(Vector3f pos = {0.f, 0.f, 0.f}) = 0;

        /**
         * Pause the sound
         */
        virtual void         pause() = 0;

        /**
         * Stop the sound
         */
        virtual void         stop() = 0;
        
        virtual
        ~Sound()
        {
            unload();
        }

    protected:
        /**
         * Load the ogg file and set appropriate fields.
         */
        void                loadFileOGG();

        /**
         * Set the volume range
         * @param source The source to use
         * @param maxGain The maximum gain
         * @param minGain The minimum gain
         */
        void                setVolumeLimits(ALuint* source, ALfloat* maxGain, ALfloat* minGain);

        /**
         * The sound current volume, between 0 and 1
         */
        float               volume;

        /**
         * The sound file
         */
        FILE*               file;

        /**
         * The current sound stream
         */
        OggVorbis_File      stream;

        /**
         * The sound format
         */
        ALenum              format;

        /**
         * The sound sample rate
         */
        ALsizei             sampleRate;

        /**
         * The fully resolved file path
         */
        std::string         path;
    };
}

#endif