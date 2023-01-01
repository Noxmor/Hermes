#ifndef HM_PLATFORM_DETECTION_H
#define HM_PLATFORM_DETECTION_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define HM_PLATFORM_WINDOWS
#ifndef _WIN64
#error "Hermes only supports 64-bit on Windows!"
#endif
#elif defined(__linux__) || defined(__gnu_linux__)
#define HM_PLATFORM_LINUX
#if defined(__ANDROID__)
#define HM_PLATFORM_ANDROID
#endif
#elif defined(__unix__)
#define HM_PLATFORM_UNIX
#elif defined(_POSIX_VERSION)
#define HM_PLATFORM_POSIX
#elif __APPLE__
#define HM_PLATFORM_APPLE
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
#define HM_PLATFORM_IOS
#define HM_PLATFORM_IOS_SIMULATOR
#elif TARGET_OS_IPHONE
#define HM_PLATFORM_IOS
#elif TARGET_OS_MAC
#define HM_PLATFORM_MAC_OS
#else
#error "Unknown Apple platform!"
#endif
#else
#error "Unknown platform!"
#endif

#endif