/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    Singleton_internal_inline.hpp                  oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#ifndef LUMS_SINGLETON_INTERNAL_INLINE_HPP
#define LUMS_SINGLETON_INTERNAL_INLINE_HPP

// Intended for Singleton instantiation by Lums code only.
#if defined(EXPORT_DLL)

#ifndef LUMS_CONCAT
# define LUMS_CONCAT(a,b) a ## b
#endif

// NSClass stands for namespaced::Class.
#define LM_I_FORCE_SINGLETON_INSTANCE(Class, NSClass)    LUMS_CONCAT(static void instantiate, Class)() \
                                                         { \
                                                             (void)lm::internal::Singleton<NSClass>::instance(); \
                                                         }

namespace lm
{
    namespace internal
    {
        template<typename T> inline
        T& Singleton<T>::instance()
        {
            static T t;

            return t;
        }
    }
}

#else

#define LM_I_FORCE_SINGLETON_INSTANCE

#endif // defined(EXPORT_DLL)
#endif // !defined(LUMS_SINGLETON_INLINE_HPP)