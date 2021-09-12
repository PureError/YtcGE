#include "../Fundation.hpp"
#include "core/StringUtils/YtcString.hpp"
namespace YtcGE
{
    class IniObject
    {
    public:
        using Key = String;
        using Value = String;
        using Section = std::unordered_map<Key, Value>;
        IniObject(const String& path);
        IniObject() = default;
        ~IniObject() = default;
        void SaveTo(const String& file_path) const;
        void Parse(IStream& input_stream);
        void Dump(OStream& output_stream) const;
        bool IsEmpty() const { return value_map_.empty(); }
        bool HasSection(const String& section_name) const { return value_map_.find(section_name) != value_map_.end(); }
        const Section& GetSection(const String& section_name) const;
        void Set(const String& section_name, const Key& key, const Value& value);
        std::vector<std::pair<String, Section>> List() const;
    private:
        std::unordered_map<String, Section> value_map_;
    };
}
