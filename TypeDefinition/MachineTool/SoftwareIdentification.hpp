#pragma once
#include "../../OpcUaTypes/LocalizedText.hpp"
#include "../DI/Constants.hpp"
#include "../TypeDefinition.hpp"
#include "Constants.hpp"

namespace machineTool {

struct SoftwareIdentification_t {
  BindableMemberValue<std::string> SoftwareRevision;
  BindableMemberValue<std::string> Identifier;
  BindableMemberValue<UmatiServerLib::LocalizedText_t> Manufacturer;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::SoftwareIdentification_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_SOFTWAREIDENTIFICATIONTYPE)))
REFL_FIELD(SoftwareRevision, UmatiServerLib::attribute::UaBrowseName(constants::NsDIUri))
REFL_FIELD(Identifier)
REFL_FIELD(
  Manufacturer,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_SOFTWAREIDENTIFICATIONTYPE_MANUFACTURER)))
REFL_END
