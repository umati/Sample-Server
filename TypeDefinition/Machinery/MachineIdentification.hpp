
#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "IMachineTagNameplate.hpp"

namespace machinery {

struct MachineIdentification_t : public IMachineryItemVendorNameplateType_t {};

}  // namespace machinery

REFL_TYPE(
  machinery::MachineIdentification_t,
  Bases<di::IVendorNameplate_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineryUri, UA_MACHINERYID_IMACHINEVENDORNAMEPLATETYPE)))

REFL_END
