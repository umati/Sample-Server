#pragma once
#include <list>
#include "src_generated/machinetool_nodeids.h"
#include "src_generated/di_nodeids.h"
#include "src_generated/machinery.h"
#include "src_generated/types_machinetool_generated.h"
#include "NS0.hpp"
#include <variant>

namespace constants
{
constexpr const char *NsDIUri = "http://opcfoundation.org/UA/DI/";
constexpr const char *NsMachineryUri = "http://opcfoundation.org/UA/Machinery/";
constexpr const char *NsMachineToolUri = "http://opcfoundation.org/UA/MachineTool/";
constexpr const char *NsInstanceUri = "http://isw.example.com";
} // namespace constants

struct ITagNameplate_t
{
  BindableMemberValue<std::string> AssetId;
  BindableMemberValue<open62541Cpp::LocalizedText_t> ComponentName;
};

REFL_TYPE(ITagNameplate_t,
          open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsDIUri, UA_DIID_ITAGNAMEPLATETYPE)})
REFL_FIELD(AssetId, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(ComponentName, open62541Cpp::attribute::PlaceholderOptional())
REFL_END

struct IMachineTagNameplate_t : public ITagNameplate_t
{
  BindableMemberValue<std::string> Location;
};

REFL_TYPE(IMachineTagNameplate_t,
          Bases<ITagNameplate_t>(),
          open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsDIUri, UA_DIID_ITAGNAMEPLATETYPE)})
REFL_FIELD(Location, open62541Cpp::attribute::PlaceholderOptional())
REFL_END

struct IVendorNameplate_t
{
  BindableMemberValue<std::string> ProductInstanceUri;
  BindableMemberValue<open62541Cpp::LocalizedText_t> Model;
  BindableMemberValue<std::string> SoftwareRevision;
  BindableMemberValue<open62541Cpp::LocalizedText_t> Manufacturer;
  BindableMemberValue<std::string> SerialNumber;
};
REFL_TYPE(IVendorNameplate_t,
          open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE)})
REFL_FIELD(ProductInstanceUri, open62541Cpp::attribute::PlaceholderOptional());
REFL_FIELD(Model, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(SoftwareRevision, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(Manufacturer, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(SerialNumber, open62541Cpp::attribute::PlaceholderOptional())
REFL_END

struct IMachineVendorNameplate_t : public IVendorNameplate_t
{
  BindableMemberValue<std::uint16_t> YearOfConstruction;
};
REFL_TYPE(IMachineVendorNameplate_t,
          Bases<IVendorNameplate_t>(),
          open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineryUri, UA_MACHINERY_ID_IMACHINEVENDORNAMEPLATETYPE)})
REFL_FIELD(YearOfConstruction, open62541Cpp::attribute::PlaceholderOptional(),
           open62541Cpp::attribute::MemberInTypeNodeId{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineryUri, UA_MACHINERY_ID_IMACHINEVENDORNAMEPLATETYPE_YEAROFCONSTRUCTION)})
REFL_FIELD(Manufacturer);
REFL_FIELD(ProductInstanceUri);
REFL_FIELD(SerialNumber);
REFL_END

struct MachineToolIdentification_t : public IMachineVendorNameplate_t, public IMachineTagNameplate_t
{
};

REFL_TYPE(MachineToolIdentification_t,
          Bases<IMachineVendorNameplate_t, IMachineTagNameplate_t>(),
          open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MACHINETOOLIDENTIFICATIONTYPE)})
REFL_END

struct SoftwareIdentification_t
{
  BindableMemberValue<std::string> SoftwareRevision;
  BindableMemberValue<std::string> Identifier;
};

REFL_TYPE(SoftwareIdentification_t, open62541Cpp::attribute::UaObjectType())
REFL_FIELD(SoftwareRevision)
REFL_FIELD(Identifier)
REFL_END

struct ChannelMonitoring_t
{
  BindableMemberValue<UA_ChannelState> ChannelState;
  BindableMember<AnalogUnitRangeType_t<double>> FeedOverride;
};

REFL_TYPE(ChannelMonitoring_t, open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_CHANNELMONITORINGTYPE)})
REFL_FIELD(ChannelState)
REFL_FIELD(FeedOverride)
REFL_END

struct Prognosis_t
{
  BindableMemberValue<open62541Cpp::DateTime_t> PredictedTime;
};
REFL_TYPE(Prognosis_t, open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PROGNOSISTYPE)})
REFL_FIELD(PredictedTime)
REFL_END

struct MaintenancePrognosis_t : public Prognosis_t
{
  BindableMemberValue<open62541Cpp::LocalizedText_t> Activity;
};
REFL_TYPE(
  MaintenancePrognosis_t,
  Bases<Prognosis_t>(),
  open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MAINTENANCEPROGNOSISTYPE)})
REFL_FIELD(Activity)
REFL_END

struct UtilityChangePrognosis_t : public Prognosis_t
{
  BindableMemberValue<open62541Cpp::LocalizedText_t> UtilityName;
};
REFL_TYPE(
  UtilityChangePrognosis_t,
  Bases<Prognosis_t>(),
  open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_UTILITYCHANGEPROGNOSISTYPE)})
REFL_FIELD(UtilityName)
REFL_END


//\todo correct inheritance
struct FiniteStateVariable_t : public StateVariable_t<UA_NodeId>
{
};
REFL_TYPE(
  FiniteStateVariable_t,
  Bases<StateVariable_t<UA_NodeId>>(),
  open62541Cpp::attribute::UaVariableType{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_FINITESTATEVARIABLETYPE)})
REFL_FIELD(Value, open62541Cpp::attribute::UaVariableTypeValue())
REFL_END

/// Virtual definition, not a UA type
struct Production_FiniteStateVariable_t : public FiniteStateVariable_t
{};
REFL_TYPE(Production_FiniteStateVariable_t,
  Bases<FiniteStateVariable_t>(),
  open62541Cpp::attribute::UaVariableType{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_FINITESTATEVARIABLETYPE)})
REFL_FIELD(Number) // Override mandatory
REFL_END

template<typename T>
struct TransitionVariable_t
{
  BindableMemberValue<open62541Cpp::LocalizedText_t> Value;
  BindableMemberValue<T> Id;
  BindableMemberValue<std::uint32_t> Number;
  BindableMemberValue<UA_QualifiedName> Name;
  BindableMemberValue<open62541Cpp::DateTime_t> TransitionTime;
};
REFL_TEMPLATE((typename T), (TransitionVariable_t<T>), open62541Cpp::attribute::UaVariableType{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_TRANSITIONVARIABLETYPE)})
REFL_FIELD(Id)
REFL_FIELD(Value, open62541Cpp::attribute::UaVariableTypeValue())
REFL_FIELD(Number, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(Name, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(TransitionTime, open62541Cpp::attribute::PlaceholderOptional())
REFL_END

struct FiniteTransitionVariable_t : public TransitionVariable_t<UA_NodeId>
{

};
REFL_TYPE(FiniteTransitionVariable_t,
          Bases<TransitionVariable_t<UA_NodeId>>(),
          open62541Cpp::attribute::UaVariableType{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_FINITETRANSITIONVARIABLETYPE)})
REFL_END

/// Virtual definition, not a UA type
struct Production_FiniteTransitionVariable_t : public FiniteTransitionVariable_t
{};
REFL_TYPE(Production_FiniteTransitionVariable_t,
  Bases<FiniteTransitionVariable_t>(),
  open62541Cpp::attribute::UaVariableType{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_FINITETRANSITIONVARIABLETYPE)})
REFL_FIELD(Number) // Override mandatory
REFL_END

///\todo correc inheritance
struct FiniteStateMachine_t
{
  BindableMember<FiniteStateVariable_t> CurrentState;
  BindableMember<FiniteTransitionVariable_t> LastTransition;
};
REFL_TYPE(FiniteStateMachine_t,
          open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_FINITESTATEMACHINETYPE)})
REFL_FIELD(CurrentState)
REFL_FIELD(LastTransition, open62541Cpp::attribute::PlaceholderOptional())
REFL_END

struct ProductionStateMachine_t : public FiniteStateMachine_t
{
  BindableMember<Production_FiniteStateVariable_t> CurrentState; // Override
  BindableMember<Production_FiniteTransitionVariable_t> LastTransition; // Override
};
REFL_TYPE(ProductionStateMachine_t,
          Bases<FiniteStateMachine_t>(),
          open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE)})
REFL_FIELD(CurrentState, open62541Cpp::attribute::UaBrowseName{.NsURI = constants::Ns0Uri})
REFL_FIELD(LastTransition, open62541Cpp::attribute::UaBrowseName{.NsURI = constants::Ns0Uri}, open62541Cpp::attribute::PlaceholderOptional())
REFL_END

struct NotificationEvent_t : public BaseEventType_t
{
  std::string Identifier;
};
REFL_TYPE(NotificationEvent_t,
          Bases<BaseEventType_t>(),
          open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_NOTIFICATIONEVENTTYPE)})
REFL_FIELD(Identifier)
REFL_END
struct Alert_t : public AlarmCondition_t
{
  std::string ErrorCode;
};

REFL_TYPE(Alert_t,
          Bases<AlarmCondition_t>(),
          open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_ALERTTYPE)})
REFL_FIELD(ErrorCode)
REFL_END

struct ProductionJob_t
{
  BindableMemberValue<std::string> Identifier;
  BindableMemberValue<std::uint32_t> RunsCompleted;
  BindableMemberValue<std::uint32_t> RunsPlanned;
  BindableMember<ProductionStateMachine_t> State;
};

REFL_TYPE(ProductionJob_t,
          open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBTYPE)})
REFL_FIELD(Identifier)
REFL_FIELD(RunsCompleted)
REFL_FIELD(RunsPlanned)
REFL_FIELD(State)
REFL_END

struct ProdictionJobList_t : public OrderedList_t<ProductionJob_t>
{
};

REFL_TYPE(ProdictionJobList_t,
          Bases<OrderedList_t<ProductionJob_t>>(),
          open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBLISTTYPE)})
REFL_FIELD(OrderedObjects,
               open62541Cpp::attribute::MemberInTypeNodeId{
                   .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBLISTTYPE_ORDEREDOBJECT_PLACEHOLDER)},
               open62541Cpp::attribute::PlaceholderOptional())
REFL_END

struct ProductionProgram_t
{
  BindableMemberValue<std::string> Name;
  BindableMember<ProductionStateMachine_t> State;
};
REFL_TYPE(ProductionProgram_t,
          open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONPROGRAMTYPE)})
REFL_FIELD(Name)
REFL_FIELD(State,
  open62541Cpp::attribute::MemberInTypeNodeId{
                   .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONPROGRAMTYPE_STATE)},
  open62541Cpp::attribute::PlaceholderOptional())
REFL_END

struct ProductionActiveProgram_t : public ProductionProgram_t
{
  BindableMemberValue<std::string> JobIdentifier;
  BindableMemberValue<UA_NodeId> JobNodeId;
};
REFL_TYPE(ProductionActiveProgram_t,
          Bases<ProductionProgram_t>(),
          open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONACTIVEPROGRAMTYPE)})
//REFL_FIELD(State) // Override mandatory ///\todo requires fix in nodeset, i=5030 needs to be ProductionActiveProgramType not ProductionProgramType
REFL_FIELD(JobIdentifier, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(JobNodeId, open62541Cpp::attribute::PlaceholderOptional())
REFL_END

struct Monitoring_t
{
  BindableMemberPlaceholder<BindableMember, ChannelMonitoring_t> Channels;
};

REFL_TYPE(Monitoring_t, open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MONITORINGTYPE)})
REFL_FIELD(Channels, open62541Cpp::attribute::MemberInTypeNodeId{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MONITORINGTYPE_MONITOREDELEMENT)},
           open62541Cpp::attribute::PlaceholderOptional())
REFL_END

struct Production_t
{
  BindableMember<ProductionActiveProgram_t> ActiveProgram;
  BindableMember<ProdictionJobList_t> ProductionPlan;
};

REFL_TYPE(Production_t, open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONTYPE)})
REFL_FIELD(ProductionPlan, open62541Cpp::attribute::PlaceholderOptional(),
           open62541Cpp::attribute::MemberInTypeNodeId{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONTYPE_PRODUCTIONPLAN)})
REFL_FIELD(ActiveProgram)
REFL_END

struct PrognosisList_t
{
  BindableMemberValue<std::string> NodeVersion;
  BindableMemberPlaceholder<BindableMember, std::variant<MaintenancePrognosis_t, UtilityChangePrognosis_t>> Prognosis;
};
REFL_TYPE(PrognosisList_t, open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PROGNOSISLISTTYPE)})
REFL_FIELD(Prognosis, open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PROGNOSISLISTTYPE_PROGNOSIS)},
           open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(NodeVersion, open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PROGNOSISLISTTYPE_NODEVERSION)},
           open62541Cpp::attribute::PlaceholderOptional())
REFL_END

struct Notification_t
{
  BindableMember<PrognosisList_t> Prognoses;
};
REFL_TYPE(Notification_t, open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PROGNOSISLISTTYPE)})
REFL_FIELD(Prognoses, open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_NOTIFICATIONTYPE_PROGNOSES)},
           open62541Cpp::attribute::PlaceholderOptional())
REFL_END

namespace MT
{
struct MachineTool_t
{
  BindableMember<MachineToolIdentification_t> Identification;
  BindableMember<Monitoring_t> Monitoring;
  BindableMember<Production_t> Production;
  BindableMember<Notification_t> Notification;
};
} // namespace MT

REFL_TYPE(MT::MachineTool_t, open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MACHINETOOLTYPE)})
REFL_FIELD(Identification, open62541Cpp::attribute::UaBrowseName{.NsURI = constants::NsDIUri})
REFL_FIELD(Monitoring)
REFL_FIELD(Production)
REFL_FIELD(Notification)
REFL_END
