///
/// \file EUInformation.hpp
/// \author Christian von Arnim
/// \date 12.02.2020
///

#pragma once
#include "LocalizedText.hpp"
#include <open62541/types_generated.h>

namespace UmatiServerLib
{
  struct EUInformation_t
  {
    std::string NamespaceUri;
    UA_Int32 UnitId;
    LocalizedText_t DisplayName;
    LocalizedText_t Description;

    static EUInformation_t fromUa(const ::UA_EUInformation &other);
    bool operator==(const EUInformation_t &other) const;
  };

} // namespace UmatiServerLib

REFL_TYPE(UmatiServerLib::EUInformation_t, UmatiServerLib::attribute::UaDataType(&UA_TYPES[UA_TYPES_EUINFORMATION]))
REFL_FIELD(NamespaceUri)
REFL_FIELD(UnitId)
REFL_FIELD(DisplayName)
REFL_FIELD(Description)
REFL_END
