/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    Singleton_internal.hpp                         oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#ifndef LUMS_SINGLETON_INTERNAL_HPP
#define LUMS_SINGLETON_INTERNAL_HPP

#include <LumsInclude/ExportDll.hpp>

// Intended for Singleton instantiation by Lums code only.
// Why the fuss ? -> Windows platforms.

namespace lm
{
    namespace internal
    {
        /**
         * @brief An helper template for creating singletons
         *
         * Singletons created using this class are lazy-constructed
         */
        template <typename T>
        class LUMS_EXPORTED Singleton
        {
        public:
            /**
             * Get the single instance
             * @return The single instance
             */
            static T& instance();

        protected:
            /**
             * Default ctor
             */
            Singleton() = default;

            /**
             * Deleted copy ctor
             */
            Singleton(const Singleton<T>&) = delete;

            /**
             * Deleted assignment operator
             */
            Singleton<T>& operator=(const Singleton<T>&) = delete;
        };
    }
}

#endif
