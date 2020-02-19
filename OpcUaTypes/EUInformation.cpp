
#include "EUInformation.hpp"
#include "LocalizedText.hpp"

namespace open62541Cpp
{

EUInformation_t EUInformation_t::fromUa(const ::UA_EUInformation &other)
{
    EUInformation_t ret{
        .NamespaceUri = std::string((const char *)other.namespaceUri.data, other.namespaceUri.length),
        .UnitId = other.unitId,
        .DisplayName = LocalizedText_t::fromUa(other.displayName),
        .Description = LocalizedText_t::fromUa(other.description),
    };
    return ret;
}

bool EUInformation_t::operator==(const EUInformation_t &other) const
{
    return this->NamespaceUri == other.NamespaceUri &&
           this->UnitId == other.UnitId &&
           this->Description == other.Description &&
           this->DisplayName == other.DisplayName;
}

} // namespace open62541Cpp
