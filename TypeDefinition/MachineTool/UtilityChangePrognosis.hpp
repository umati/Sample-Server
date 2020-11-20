#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "Prognosis.hpp"

namespace machineTool
{

  struct UtilityChangePrognosis_t : public Prognosis_t
  {
    BindableMemberValue<std::string> UtilityName;
  };

} // namespace machineTool

REFL_TYPE(
    machineTool::UtilityChangePrognosis_t,
    Bases<machineTool::Prognosis_t>(),
    UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_UTILITYCHANGEPROGNOSISTYPE)))
REFL_FIELD(UtilityName)
REFL_END
