#ifndef STRING_H
#define STRING_H

#include <string>
#include <string_view>
#include <vector>

namespace KWS ::Utility {

std::string LStrip(std::string_view str, char chr = ' ');
std::string RStrip(std::string_view str, char chr = ' ');
std::string Strip(std::string_view str, char chr = ' ');
std::vector<std::string> Split(std::string_view url, char delim = ' ');

}  // namespace KWS::Utility

#endif  //STRING_H
