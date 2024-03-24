#include <string>

#include "kws/kws.hpp"

auto main() -> int
{
  auto const exported = exported_class {};

  return std::string("kws") == exported.name() ? 0 : 1;
}
