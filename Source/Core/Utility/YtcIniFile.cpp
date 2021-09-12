#include "YtcIniFile.hpp"
#include "Core/Utility/YtcException.hpp"
#include "Core/Utility/YtcFileStream.hpp"

YtcGE::IniObject::IniObject(const String& path)
{
    IFileStream file_stream(path);
    Parse(file_stream);
}

void YtcGE::IniObject::SaveTo(const String& file_path) const
{
    OFileStream file_stream(file_path, std::ios::out);
    if (!file_stream.is_open()) throw Exception(_T("failed to open file!"));
    Dump(file_stream);
}

void YtcGE::IniObject::Parse(IStream& input_stream)
{
    if (!input_stream.good()) throw Exception(_T("invalid input stream!"));
    auto is_comment = [](const String& line) { return line.front() == _T('#') || line.front() == _T(';'); };
    auto extract_key_value = [](const String& line)
    {
        auto pos_spliter = line.find(_T('='));
        if (pos_spliter == String::npos) return std::make_pair(false, Section::value_type());
        return std::make_pair(true, Section::value_type(line.substr(0, pos_spliter), line.substr(pos_spliter + 1)));
    };
    String section_parsing;
    for (String line_raw_text; !input_stream.eof(); std::getline(input_stream, line_raw_text))
    {
        String line = TrimLeft(line_raw_text);
        if (!line.empty() && !is_comment(line))
        {
            if (line.front() == _T('['))
            {
                auto pos_right_square_bracket = line.rfind(_T(']'));
                if (pos_right_square_bracket == String::npos || pos_right_square_bracket == 1) throw Exception("invalid ini file!");
                section_parsing = line.substr(1, pos_right_square_bracket - 1);
            }
            else if (!section_parsing.empty())
            {
                auto& section = value_map_[section_parsing];
                auto result =  extract_key_value(line);
                if (!result.first) throw Exception("invalid section data in ini file!");
                section.insert(std::move(result.second));
            }
        }
    }
}

void YtcGE::IniObject::Dump(OStream& output_stream) const
{
    for (auto& section : value_map_)
    {
        output_stream << _T("\r\n[");
        output_stream << section.first << _T("]\n");
        for (auto & kv : section.second) output_stream << kv.first << _T("=") << kv.second << _T("\n");
    }
}

const YtcGE::IniObject::Section& YtcGE::IniObject::GetSection(const String& section_name) const
{
    auto iter = value_map_.find(section_name);
    if (iter == value_map_.end()) throw Exception(_T("can not find the section!"));
    return iter->second;
}

void YtcGE::IniObject::Set(const String& section_name, const Key& key, const Value& value)
{
    if (section_name.empty()) throw Exception(_T("invalid section name!"));
    if (key.empty()) throw Exception(_T("invalid key name!"));
    value_map_[section_name][key] = value;
}

std::vector<std::pair<YtcGE::String, YtcGE::IniObject::Section>> YtcGE::IniObject::List() const
{
    std::vector<std::pair<YtcGE::String, YtcGE::IniObject::Section>> ret(value_map_.size());
    std::copy(value_map_.begin(), value_map_.end(), ret.begin());
    return ret;
}

