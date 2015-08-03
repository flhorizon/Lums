#pragma once
#ifndef PATH_HPP
#define PATH_HPP

#include <LumsInclude/Singleton.hpp>
#include <string>

namespace lm
{

#ifdef _WIN32
	using path_t = std::wstring;
#else // Unix then
	using path_t = std::string;
#endif // ifdef _WIN32


	class Path : public Singleton<Path>
	{
	public:
		Path() = default;
		Path(const Path&) = delete;
		Path&					operator=(const Path&) = delete;

		const path_t&			resourcePath();
		const path_t&			appData();
		bool					mkAppDataDir();
		
	private:
	
	};

}

#endif // PATH_HPP