#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "../Machinery/IMachineVendorNameplate.hpp"
#include "../Machinery/IMachineTagNameplate.hpp"
#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "../ns0/Constants.hpp"
#include "SoftwareIdentification.hpp"

namespace machineTool
{

struct MachineToolIdentification_SoftwareIdentification_t
{
  BindableMemberPlaceholder<BindableMember, SoftwareIdentification_t> SoftwareItem;
};

struct MachineToolIdentification_t : public machinery::IMachineVendorNameplate_t, public machinery::IMachineTagNameplate_t
{
  BindableMember<MachineToolIdentification_SoftwareIdentification_t> SoftwareIdentification;

};

} // namespace machineTool

REFL_TYPE(machineTool::MachineToolIdentification_t,
          Bases<machinery::IMachineVendorNameplate_t, machinery::IMachineTagNameplate_t>(),
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MACHINETOOLIDENTIFICATIONTYPE)})
REFL_FIELD(SoftwareIdentification, UmatiServerLib::attribute::PlaceholderOptional(),
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MACHINETOOLIDENTIFICATIONTYPE_SOFTWAREIDENTIFICATION)})
REFL_END

REFL_TYPE(machineTool::MachineToolIdentification_SoftwareIdentification_t,
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_BASEOBJECTTYPE)})
REFL_FIELD(SoftwareItem, UmatiServerLib::attribute::PlaceholderOptional(),
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MACHINETOOLIDENTIFICATIONTYPE_SOFTWAREIDENTIFICATION_SOFTWAREITEM)})
REFL_END
