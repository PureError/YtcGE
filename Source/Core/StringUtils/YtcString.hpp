#pragma once
#ifndef YTC_STRING_HPP
#define YTC_STRING_HPP


#include "../Platform/YtcOS.hpp"
#include "../Utility/Localization.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

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
        
        static const MyType & Empty() noexcept
        {
            static MyType empty;
            return empty;
        }

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

        template<class Iter, class = typename std::enable_if_t<std::_Is_iterator<Iter>::value, Iter>>
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

        template<typename...Args>
        static String_T Format(String_T fmt_str, Args&&...args);
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
    using IStream = std::basic_istream<Char>;
    using OStream = std::basic_ostream<Char>;
    using StringBuffer = std::basic_stringbuf<Char>;
    using StringStream = std::basic_stringstream<Char>;

    template<typename C>
    struct SpecialCharset;

    template<>
    struct SpecialCharset<char>
    {
        static constexpr char* Spaces = "\t\r\n ";
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
    inline String ToString(const T & val)
    {
        StringStream ss;
        ss << val;
        return ss.str();
    }

    
    template<typename C>
    struct FormatSymbol
    {
    };

    template<>
    struct FormatSymbol<char>
    {
        static constexpr char ArgStartSymbol = ('{');
        static constexpr char ArgEndSymbol = ('}');
        static constexpr char EscapeSymbol = ('$');
    };

    template<>
    struct FormatSymbol<wchar_t>
    {
        static constexpr wchar_t ArgStartSymbol = L'{';
        static constexpr wchar_t ArgEndSymbol = L'}';
        static constexpr wchar_t EscapeSymbol = L'$';
    };

    template<typename Str>
    class Formatter
    {
        using CharType = typename Str::value_type;
        using FormatSymbol_T = FormatSymbol<CharType>;
    public:
        static constexpr CharType ArgStartSymbol = FormatSymbol_T::ArgStartSymbol;// _T('{');
        static constexpr CharType ArgEndSymbol = FormatSymbol_T::ArgEndSymbol;//_T('}');
        static constexpr CharType EscapeSymbol = FormatSymbol_T::EscapeSymbol;//_T('$');
        struct ArgumentSlot
        {
            size_t beg; // position of '{'
            size_t end; // position of '}'
            size_t index;
        };

        Formatter(Str str_fmt) : str_fmt_(std::move(str_fmt))
        {
            Scan();
        }

        template<typename...Args>
        Str operator()(Args&&...args)
        {
            Str result;
            StringfyArgs(std::forward<Args>(args)...);
            size_t pos = 0;
            for (const auto & slot : slots_)
            {
                uint32_t length = slot.beg - pos;
                result.append(str_fmt_, pos, length);
                result.append(stringfied_args_[slot.index]);
                pos = slot.end + 1;
            }
            if (pos < str_fmt_.length())
            {
                result.append(str_fmt_, pos);
            }

            return result;
        }
    private:
        void StringfyArgs() noexcept
        {
        }

        template<typename First, typename...Args>
        void StringfyArgs(First&& first, Args&&...args)
        {
            StringfyArgImpl(std::forward<First>(first));
            StringfyArgs(std::forward<Args>(args)...);
        }


        template<typename T>
        void StringfyArgImpl(T&& arg)
        {
            stringfied_args_[cnt_str_args_++] = ToString(std::forward<T>(arg));
        }

        enum class State : uint8_t
        {
            NormalCharacter,
            ArgSlotCharacter,
            Escape,
        };

        void Scan()
        {
            State state = State::NormalCharacter;
            Str str_preprocess;
            size_t pos_beg_brace = Str::npos;
            size_t pos_start = 0;
            for (size_t offset = 0; offset < str_fmt_.length(); ++offset)
            {
                auto ch_cur = str_fmt_[offset];
                if (State::Escape == state)
                {
                    state = State::NormalCharacter;
                    str_preprocess.push_back(ch_cur);
                    pos_start = offset + 1;
                }
                else
                {
                    switch (ch_cur)
                    {
                        case EscapeSymbol:
                            YTC_RT_ASSERT(State::NormalCharacter == state);
                            state = State::Escape;
                            str_preprocess.append(str_fmt_.substr(pos_start, offset - pos_start));
                            break;
                        case ArgStartSymbol:
                            if (state != State::ArgSlotCharacter)
                            {
                                state = State::ArgSlotCharacter;
                                pos_beg_brace = offset;
                                str_preprocess.append(str_fmt_.substr(pos_start, offset - pos_start));
                            }
                            else
                            {
                                YTC_RT_ASSERT(false);
                            }
                            break;
                        case ArgEndSymbol:
                            if (state == State::ArgSlotCharacter && pos_beg_brace < str_fmt_.length())
                            {
                                auto beg = str_preprocess.length();
                                str_preprocess.append(str_fmt_.substr(pos_beg_brace, offset + 1));
                                auto end = str_preprocess.length() - 1;
                                auto idx = GetArgIndex(str_preprocess, beg, end);
                                if (stringfied_args_.size() <= idx)
                                {
                                    stringfied_args_.resize(idx + 1);
                                }
                                slots_.push_back({ beg, end, idx });
                                pos_start = offset + 1;
                                state = State::NormalCharacter;
                                pos_beg_brace = Str::npos;
                            }
                            else
                            {
                                YTC_RT_ASSERT(false);
                            }
                        default:
                            break;
                    }
                }
            }
            if (pos_start < str_fmt_.length())
            {
                str_preprocess.append(str_fmt_, pos_start);
            }
            str_fmt_.swap(str_preprocess);
        }

        static size_t GetArgIndex(const Str & str_fmt, size_t beg, size_t off)
        {
            return std::stoul(str_fmt.substr(beg + 1, off - beg));
        }

        Str str_fmt_;
        std::vector<Str> stringfied_args_;
        std::vector<ArgumentSlot> slots_;
        int cnt_str_args_ = 0;
    };

    template<typename C, typename T, typename A>
    template<typename...Args>
    String_T<C, T, A> String_T<C, T, A>::Format(String_T<C, T, A> fmt_str, Args&&...args)
    {
        using Fmtter = Formatter<String_T<C, T, A>>;
        return Fmtter(std::move(fmt_str))(std::forward<Args>(args)...);
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