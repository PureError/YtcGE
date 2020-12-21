#pragma once
#ifndef YTC_OS_HPP
#define YTC_OS_HPP

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    #define YTC_OS_WINDOWS
    #ifdef _WIN64
        #define YTC_WIN64
    #else 
        #define YTC_WIN32
    #endif
#elif defined(linux) || defined(__linux) || defined(__linux__)
    #define YTC_OS_LINUX
#elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    #define YTC_OS_IOS
    #else
    #define YTC_MAC_OS
    #endif
#elif defined(__ANDROID__)
    #define YTC_OS_ANDROID

#else 
    #error "Unsupported platform!"
#endif
#endif