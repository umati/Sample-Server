#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "Prognosis.hpp"

namespace machineTool
{

struct MaintenancePrognosis_t : public Prognosis_t
{
  BindableMemberValue<open62541Cpp::LocalizedText_t> Activity;
};

} // namespace machineTool

REFL_TYPE(
    machineTool::MaintenancePrognosis_t,
    Bases<machineTool::Prognosis_t>(),
    open62541Cpp::attribute::UaObjectType{
        .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MAINTENANCEPROGNOSISTYPE)})
REFL_FIELD(Activity)
REFL_END
