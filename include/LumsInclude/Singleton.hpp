/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    Singleton.hpp                                  oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#ifndef LUMS_SINGLETON_HPP
#define LUMS_SINGLETON_HPP

// Intended for Singleton instantiation by client code only.
// Protecting from accidental inclusion in Lums resident code.
#ifndef EXPORT_DLL

namespace lm
{
	/**
	 * @brief An helper template for creating singletons
	 *
	 * Singletons created using this class are lazy-constructed
	 */
	template <typename T>
	class Singleton
	{
	public:
		/**
		 * Get the single instance
		 * @return The single instance
		 */
        static T& Singleton<T>::instance()
        {
            static T t;

            return t;
        }
		
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

#endif // !defined(EXPORT_DLL)

#endif
