///
/// \file EUInformation.hpp
/// \author Christian von Arnim
/// \date 12.02.2020
///

#pragma once
#include <open62541/types_generated.h>
#include "LocalizedText.hpp"

namespace open62541Cpp {
struct EUInformation_t{
  std::string NamespaceUri;
  UA_Int32 UnitId;
  LocalizedText_t DisplayName;
  LocalizedText_t Description;

  static EUInformation_t fromUa(const ::UA_EUInformation &other);
  bool operator ==(const EUInformation_t &other) const;
};

}

REFL_TYPE(open62541Cpp::EUInformation_t, open62541Cpp::attribute::UaDataType(&UA_TYPES[UA_TYPES_EUINFORMATION]))
  REFL_FIELD(NamespaceUri)
  REFL_FIELD(UnitId)
  REFL_FIELD(DisplayName)
  REFL_FIELD(Description)
REFL_END
