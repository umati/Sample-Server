
#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "IMachineTagNameplate.hpp"

namespace machinery
{

struct MachineIdentification_t : public IMachineVendorNameplate_t
{

};

} // namespace machinery

REFL_TYPE(machinery::IMachineVendorNameplate_t,
          Bases<di::IVendorNameplate_t>(),
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineryUri, UA_MACHINERY_ID_IMACHINEVENDORNAMEPLATETYPE)})

REFL_END
