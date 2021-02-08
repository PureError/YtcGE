#pragma once
#ifndef YTC_EXCEPTION_HPP
#define YTC_EXCEPTION_HPP

#include "../StringUtils/YtcString.hpp"
namespace YtcGE
{
    class Exception
    {
    public:
        Exception() noexcept : message_(_T("Unknown exception!"))
        {
        }

        Exception(const Exception& other) noexcept : message_(other.message_)
        {
        }

        Exception(const String& msg) noexcept : message_(msg)
        {
        }

        Exception(String&& msg) noexcept : message_(std::move(msg))
        {
        }

        Exception(const Char* msg) noexcept: message_(msg)
        {
        }

        virtual const String& Message() noexcept
        {
            return message_;
        }

        virtual ~Exception()
        {
        }

    private:
        const String message_;
    };

    class NoImplementation : public Exception {};

}


#endif