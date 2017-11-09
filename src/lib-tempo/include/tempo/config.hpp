////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file config.hpp
/// \author Jamie Terry
/// \date 2017/11/04
/// \brief Contains configuration options for project tempo
////////////////////////////////////////////////////////////////////////////

// Determine OS
#if !defined(TEMPO_OS_WINDOWS) && !defined(TEMPO_OS_UNIX) && !defined(TEMPO_OS_MAC)
	#if defined(_WIN32) || defined(__WIN32__) || defined _MSC_VER
		#define TEMPO_OS_WINDOWS 1
	#elif defined(linux) || defined(__linux) || defined(__unix) || defined(__unix__) || defined(unix)
		#define TEMPO_OS_UNIX 1
	#elif defined(__APPLE__) || defined(__MACH__)
		#define TEMPO_OS_MAC 1
	#else
		//Unsupported system
		//note, it could be that the os is supported but the compiler doesn't #define
		//one of the expected values, so we cant automatically determine the OS
		//try defining the one of the TEMPO_OS_*** #defines manually
		#error This operating system is not supported by Project Tempo
	#endif
#endif
