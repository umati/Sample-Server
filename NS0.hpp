#pragma once

#include "BindableMemberValue.hpp"
#include "OpcUaTypes/Attributes.hpp"
#include "OpcUaTypes/LocalizedText.hpp"
#include "OpcUaTypes/EUInformation.hpp"
#include "OpcUaTypes/DateTime.hpp"
#include <string>
#include <refl.hpp>
#include <vector>
#include "Util.hpp"

///\TODO enable if BINDABLEMEMBER_T is BindableMember(Value)
template <template <typename...> class BINDABLEMEMBER_T, typename T /*, typename = std::enable_if_t<is_base_of_template<BindableMember, BINDABLEMEMBER_T<T>>::value>*/>
class BindableMemberPlaceholder;

namespace constants
{
  constexpr const char *Ns0Uri = "http://opcfoundation.org/UA/";
}

///\todo limit T to IOrderedObject
template <typename T>
struct OrderedList_t
{
  BindableMemberValue<std::string> NodeVersion;
  BindableMemberPlaceholder<BindableMember, T> OrderedObjects;
};

REFL_TEMPLATE((typename T), (OrderedList_t<T>), open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ORDEREDLISTTYPE)})
REFL_FIELD(NodeVersion, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(OrderedObjects,
               open62541Cpp::attribute::MemberInTypeNodeId{
                   .NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ORDEREDLISTTYPE_ORDEREDOBJECT_PLACEHOLDER)},
               open62541Cpp::attribute::PlaceholderOptional())
REFL_END

template <typename T>
struct AnalogUnitRangeType_t
{
  BindableMemberValue<T> Value;
  BindableMemberValue<UA_Range> EURange;
  BindableMemberValue<open62541Cpp::EUInformation_t> EngineeringUnits;
};

REFL_TEMPLATE((typename T), (AnalogUnitRangeType_t<T>), open62541Cpp::attribute::UaVariableType{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ANALOGUNITRANGETYPE)})
REFL_FIELD(Value, open62541Cpp::attribute::UaVariableTypeValue())
REFL_FIELD(EURange)
REFL_FIELD(EngineeringUnits)
REFL_END

struct BaseEventType_t
{
  open62541Cpp::DateTime_t Time = std::chrono::system_clock::now();
  std::string SourceName;
  std::uint16_t Severity;
  open62541Cpp::LocalizedText_t Message;
};

REFL_TYPE(BaseEventType_t, open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_BASEEVENTTYPE)})
REFL_FIELD(Time)
REFL_FIELD(SourceName)
REFL_FIELD(Severity)
REFL_FIELD(Message)
REFL_END

struct BaseModelChangeEvent_t : public BaseEventType_t
{

};
REFL_TYPE(BaseModelChangeEvent_t,
          Bases<BaseEventType_t>(),
          open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_BASEMODELCHANGEEVENTTYPE)})
REFL_END

struct GeneralModelChangeEvent_t : public BaseModelChangeEvent_t
{
  BindableMemberValue<std::vector<UA_ModelChangeStructureDataType>> Changes;
};

REFL_TYPE(GeneralModelChangeEvent_t,
          Bases<BaseModelChangeEvent_t>(),
          open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_GENERALMODELCHANGEEVENTTYPE)})
REFL_FIELD(Changes)
REFL_END


struct StateVariable_t
{
  // id omitted and defined in subsequent types
  open62541Cpp::LocalizedText_t Value;
  UA_QualifiedName Name;
  UA_UInt32 Number;
  open62541Cpp::LocalizedText_t EffectiveDisplayName;
};

REFL_TYPE(StateVariable_t,
          open62541Cpp::attribute::UaVariableType{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_STATEVARIABLETYPE)})
REFL_FIELD(Value, open62541Cpp::attribute::UaVariableTypeValue())
REFL_FIELD(Name, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(Number, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(EffectiveDisplayName, open62541Cpp::attribute::PlaceholderOptional())
REFL_END

struct TwoStateVariable_t : public StateVariable_t
{
  bool Id;
  open62541Cpp::DateTime_t EffectiveTransitionTime;
  open62541Cpp::DateTime_t TransitionTime;
  open62541Cpp::LocalizedText_t FalseState;
  open62541Cpp::LocalizedText_t TrueState;
};

REFL_TYPE(TwoStateVariable_t,
          Bases<StateVariable_t>(),
          open62541Cpp::attribute::UaVariableType{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_TWOSTATEVARIABLETYPE)})
REFL_FIELD(Id)
REFL_FIELD(EffectiveTransitionTime, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(TransitionTime, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(FalseState, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(TrueState, open62541Cpp::attribute::PlaceholderOptional())
REFL_END

template <typename T>
struct ConditionVariable_t
{
  T Value;
  open62541Cpp::DateTime_t SourceTimestamp;
};

REFL_TEMPLATE((typename T), (ConditionVariable_t<T>), open62541Cpp::attribute::UaVariableType{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_CONDITIONVARIABLETYPE)})
REFL_FIELD(Value, open62541Cpp::attribute::UaVariableTypeValue())
REFL_FIELD(SourceTimestamp)
REFL_END

struct Condition_t : public BaseEventType_t
{
  UA_NodeId BranchId;
  std::string ClientUserId;
  ConditionVariable_t<open62541Cpp::LocalizedText_t> Comment;
  UA_NodeId ConditionClassId;
  open62541Cpp::LocalizedText_t ConditionClassName;
  std::string ConditionName;
  UA_NodeId ConditionSubClassId;
  open62541Cpp::LocalizedText_t ConditionSubClassName;
  TwoStateVariable_t EnabledState;
  ConditionVariable_t<open62541Cpp::LocalizedText_t> LastSeverity;
  ConditionVariable_t<UA_StatusCode> Quality;
  bool Retain;
};

REFL_TYPE(Condition_t,
          Bases<BaseEventType_t>(),
          open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_CONDITIONTYPE)})
REFL_FIELD(BranchId)
REFL_FIELD(ClientUserId)
REFL_FIELD(Comment)
REFL_FIELD(ConditionClassId)
REFL_FIELD(ConditionClassName)
REFL_FIELD(ConditionName)
REFL_FIELD(ConditionSubClassId, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(ConditionSubClassName, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(EnabledState)
REFL_FIELD(LastSeverity)
REFL_FIELD(Quality)
REFL_FIELD(Retain)
REFL_END

class AcknowledgeableCondition_t : public Condition_t
{
public:
  BindableMember<TwoStateVariable_t> EnabledState;
  BindableMember<TwoStateVariable_t> AckedState;
  BindableMember<TwoStateVariable_t> ConfirmedState;
};

REFL_TYPE(AcknowledgeableCondition_t,
          Bases<Condition_t>(),
          open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ACKNOWLEDGEABLECONDITIONTYPE)})
REFL_FIELD(EnabledState, open62541Cpp::attribute::MemberInTypeNodeId{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ACKNOWLEDGEABLECONDITIONTYPE_ENABLEDSTATE)})
REFL_FIELD(AckedState, open62541Cpp::attribute::MemberInTypeNodeId{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ACKNOWLEDGEABLECONDITIONTYPE_ACKEDSTATE)})
REFL_FIELD(ConfirmedState, open62541Cpp::attribute::MemberInTypeNodeId{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ACKNOWLEDGEABLECONDITIONTYPE_CONFIRMEDSTATE)}, open62541Cpp::attribute::PlaceholderOptional())
REFL_END

struct AlarmCondition_t : public AcknowledgeableCondition_t
{
  ///\todo declare fields
};

REFL_TYPE(AlarmCondition_t,
          Bases<AcknowledgeableCondition_t>(),
          open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ALARMCONDITIONTYPE)})

REFL_END

#include "BindableMemberPlaceholder.hpp"
