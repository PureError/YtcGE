#pragma once
#ifndef YTC_COMPILER_HPP
#define YTC_COMPILER_HPP

#ifdef _MSC_VER
    #if _MSC_VER < 1900
        #error "Unsupported compiler version!"
    #else 
        #define YTC_MSC
    #endif
#elif defined(__GNUC__)
    #define YTC_GCC
#endif

#endif