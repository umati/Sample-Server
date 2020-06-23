#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "../Machinery/IMachineVendorNameplate.hpp"
#include "../Machinery/IMachineTagNameplate.hpp"

namespace machineTool
{

struct MachineToolIdentification_t : public machinery::IMachineVendorNameplate_t, public machinery::IMachineTagNameplate_t
{
};

} // namespace machineTool

REFL_TYPE(machineTool::MachineToolIdentification_t,
          Bases<machinery::IMachineVendorNameplate_t, machinery::IMachineTagNameplate_t>(),
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MACHINETOOLIDENTIFICATIONTYPE)})
REFL_END
