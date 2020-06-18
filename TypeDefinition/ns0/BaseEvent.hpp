#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"

namespace ns0
{

struct BaseEvent_t
{
  open62541Cpp::DateTime_t Time = std::chrono::system_clock::now();
  std::string SourceName;
  std::uint16_t Severity;
  open62541Cpp::LocalizedText_t Message;
};

} // namespace ns0

REFL_TYPE(ns0::BaseEvent_t,
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_BASEEVENTTYPE)})
REFL_FIELD(Time)
REFL_FIELD(SourceName)
REFL_FIELD(Severity)
REFL_FIELD(Message)
REFL_END
