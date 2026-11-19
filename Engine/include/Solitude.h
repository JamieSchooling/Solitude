#pragma once

#ifdef _WIN32
	#ifdef BUILDING_CORE_LIBRARY
		#define SLTD_API __declspec(dllexport)
	#else
		#define SLTD_API __declspec(dllimport)
	#endif
#else
	#define SLTD_API __attribute__((visibility("default")))
#endif