
#include "LocalizedText.hpp"

namespace UmatiServerLib
{

  LocalizedText_t LocalizedText_t::fromUa(const ::UA_LocalizedText &other)
  {
    LocalizedText_t ret{
        .locale = std::string((const char *)other.locale.data, other.locale.length),
        .text = std::string((const char *)other.text.data, other.text.length)};
    return ret;
  }

  bool LocalizedText_t::operator==(const LocalizedText_t &other) const
  {
    return this->locale == other.locale &&
           this->text == other.text;
  }

} // namespace UmatiServerLib
