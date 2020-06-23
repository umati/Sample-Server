///
/// \file ConvertSimpleValue.hpp
/// \author Christian von Arnim
/// \date 17.12.2019
///

#pragma once

#include <cstdint>
#include <string>
#include <functional>

#include "NodeValue.hpp"
#include "../OpcUaTypes/DateTime.hpp"
#include <variant>
#include <open62541/types.h>

namespace UmatiServerLib
{

  using primitivTypes_t = std::variant<
      std::int8_t *,
      std::int16_t *,
      std::int32_t *,
      std::int64_t *,
      std::uint8_t *,
      std::uint16_t *,
      std::uint32_t *,
      std::uint64_t *,
      float *,
      double *>;

  // UA_Variant* dst
  typedef std::function<void(UA_Variant *)> copyToVariantFunc;

  class ConvertSimpleValue
  {
  public:
    static copyToVariantFunc asVariantFunc(primitivTypes_t pVariable);
    static copyToVariantFunc asVariantFunc(std::string *variable);
    static copyToVariantFunc asVariantFunc(UmatiServerLib::DateTime_t *variable);
    static copyToVariantFunc asVariantFunc(bool *pVariable);
  };

} // namespace UmatiServerLib
