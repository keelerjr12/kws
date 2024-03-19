#include "str_util.h"

namespace KWS::Utility {

std::string LStrip(std::string_view str, char chr)
{
    const auto loc = str.find_first_not_of(chr);

    if (loc == std::string::npos)
    {
        return "";
    }

    return std::string(str.substr(loc));
}

std::string RStrip(std::string_view str, char chr)
{
    const auto loc = str.find_last_not_of(chr);

    if (loc == std::string::npos)
    {
        return "";
    }

    return std::string(str.substr(0, loc + 1));
}

std::string Strip(std::string_view str, char chr)
{
    auto stripped_str = LStrip(str, chr);
    stripped_str = RStrip(stripped_str, chr);

    return stripped_str;
}

std::vector<std::string> Split(std::string_view url, char delim)
{
    std::vector<std::string> segments;

    auto url_str = Strip(url, delim);
    auto loc = url_str.find(delim);

    while (loc != std::string::npos)
    {
        const auto segment = url_str.substr(0, loc);
        segments.push_back(segment);

        url_str = url_str.substr(loc + 1);
        loc = url_str.find(delim);
    }

    segments.emplace_back(url_str);

    return segments;
}

}  // namespace KWS::Utility
