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

struct FiniteStateVariableType_t
{
  BindableMemberValue<open62541Cpp::LocalizedText_t> Value;
  BindableMemberValue<UA_UInt32> Number;
};
REFL_TYPE(FiniteStateVariableType_t, open62541Cpp::attribute::UaVariableType())
REFL_FIELD(Value, open62541Cpp::attribute::UaVariableTypeValue())
REFL_FIELD(Number, open62541Cpp::attribute::UaBrowseName{.NsURI = constants::Ns0Uri})
REFL_END

struct State_t
{
  BindableMember<FiniteStateVariableType_t> CurrentState;
};

REFL_TYPE(State_t, open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBSTATEMACHINETYPE)})
REFL_FIELD(CurrentState, open62541Cpp::attribute::UaBrowseName{.NsURI = constants::Ns0Uri})
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
  BindableMember<State_t> State;
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
  BindableMember<ProdictionJobList_t> ProductionPlan;
};

REFL_TYPE(Production_t, open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONTYPE)})
REFL_FIELD(ProductionPlan, open62541Cpp::attribute::PlaceholderOptional(),
           open62541Cpp::attribute::MemberInTypeNodeId{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONTYPE_PRODUCTIONPLAN)})
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
