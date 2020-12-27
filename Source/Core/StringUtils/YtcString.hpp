#pragma once
#ifndef YTC_STRING_HPP
#define YTC_STRING_HPP


#include "Core/Fundation.hpp"

#include <string>
#include <sstream>

#ifdef YTC_OS_WINDOWS
#if defined(_UNICODE) || defined(UNICODE)
#define YTC_UNICODE
#endif
#else

#endif

namespace YtcGE
{
    template<typename C, typename T, typename A>
    using StringBase = std::basic_string<C, T, A>;

    template<typename C, typename T, typename A>
    class String_T final : public ::std::basic_string<C, T, A>
    {
    public:
        using Base = StringBase<C, T, A>;
        using MyType = String_T<C, T, A>;
        String_T() noexcept = default;
        String_T(const A& _allocator) noexcept : Base(_allocator)
        {
        }

        String_T(const Base& right) : Base(right)
        {
        }
        
        String_T(const Base& right, const A& _allocator) : Base(right, _allocator)
        {
        }

        String_T(Base&& right) : Base(std::move(right))
        {
        }

        String_T(const value_type * ptr, size_type count) : Base(ptr, count)
        {
        }

        String_T(const value_type * ptr, size_type count, const A& al) : Base(ptr, count, al)
        {
        }

        String_T(const value_type * ptr) : Base(ptr)
        {
        }

        String_T(const size_type count, value_type ch) : Base(count, ch)
        {
        }

        String_T(const Base& right, size_type offset, size_type count = Base::npos) : Base(right, offset, count)
        {
        }

        String_T(const Base& right, size_type offset, size_type count, const A& al) : Base(right, offset, count, al)
        {
        }

        template<class Iter, class = typename std::enable_if_t<typename std::_Is_iterator<Iter>::value, Iter>>
        String_T(Iter begin, Iter end) : Base(begin, end)
        {
        }

        bool EndWith(value_type ch) const noexcept
        {
            return !empty() && (back() == ch);
        }

        bool EndWith(const value_type* str) const
        {
            return rfind(str) == (length() - traits_type::length(str));
        }

        bool EndWith(const Base& str) const noexcept
        {
            return EndWith(str.c_str());
        }

        size_type rfind_first_not_of(const value_type* ptr)
        {
            MyType charset(ptr);
            auto iter = std::find_if(rbegin(), rend(), [&](value_type ch)
            {
                return charset.find(ch) == Base::npos;
            });
            return iter == rend() ? Base::npos : length() - (iter - rbegin());
        }
    };

    using WString = String_T<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>;
    using AString = String_T<char, std::char_traits<char>, std::allocator<char>>;
#ifdef YTC_UNICODE
    using Char = wchar_t;
#ifndef _T
#define _T(text) L##text 
#endif
#else
#ifndef _T
#define _T(text) text 
#endif
    using Char = char;
#endif
    using String = String_T<Char, std::char_traits<Char>, std::allocator<Char>>;
    using StringBuffer = std::basic_stringbuf<Char>;
    using StringStream = std::basic_stringstream<Char>;

    template<typename C>
    struct SpecialCharset;

    template<>
    struct SpecialCharset<char>
    {
        constexpr static char* Spaces = "\t\r\n ";
    };

    template<>
    struct SpecialCharset<wchar_t>
    {
        constexpr static wchar_t* Spaces = L"\t\r\n ";
    };

    template<typename C, typename T, typename A>
    inline String_T<C, T, A> TrimLeft(const StringBase<C, T, A>& str) noexcept
    {
        using RegardAs = String_T<C, T, A>;
        auto start = str.find_first_not_of(SpecialCharset<RegardAs::value_type>::Spaces);
        return start < str.length() ? str.substr(start) : String();
    }

    template<typename C, typename T, typename A>
    inline String_T<C, T, A> TrimRight(const StringBase<C, T, A>& str) noexcept
    {
        using RegardAs = String_T<C, T, A>;
        auto end = static_cast<RegardAs>(str).rfind_first_not_of(SpecialCharset<RegardAs::value_type>::Spaces);
        return str.substr(0, end);
    }
 
    template<typename C, typename T, typename A>
    inline String_T<C, T, A> Trim(const StringBase<C, T, A>& str) noexcept
    {
        using RegardAs = String_T<C, T, A>;
        auto spaces = SpecialCharset<RegardAs::value_type>::Spaces;
        auto start = str.find_first_not_of(spaces);
        auto end = static_cast<RegardAs>(str).rfind_first_not_of(spaces);
        return start < str.length() ? str.substr(start, end - start) : RegardAs();
    }

    template<typename T>
    inline String ToString(T val)
    {
#ifdef YTC_UNICODE
        return std::to_wstring(val);
#else
        return std::to_string(val);
#endif
    }


}

namespace std
{
    template<>
    struct hash<::YtcGE::AString> : hash<string>
    {
    };

    template<>
    struct hash<::YtcGE::WString> : hash<wstring>
    {
    };

}
#endif