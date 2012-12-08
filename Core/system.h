
#ifndef Rocket_Core_System_H
#define Rocket_Core_System_H

#if defined (__WIN32__) || defined (_WIN32)
#define OS_WINDOWS
// Disable warnings for deprecated functions
#pragma warning(disable:4996)
#else
#define OS_LINUX
#endif

#endif