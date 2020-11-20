#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "BaseModelChangeEvent.hpp"

namespace ns0
{

  struct GeneralModelChangeEvent_t : public BaseModelChangeEvent_t
  {
    BindableMemberValue<std::vector<UA_ModelChangeStructureDataType>> Changes;
  };

} // namespace ns0

REFL_TYPE(ns0::GeneralModelChangeEvent_t,
          Bases<ns0::BaseModelChangeEvent_t>(),
          UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_GENERALMODELCHANGEEVENTTYPE)))
REFL_FIELD(Changes)
REFL_END
