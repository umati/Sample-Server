#pragma once
#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "../TypeDefinition.hpp"
#include "../ns0/BaseObject.hpp"
#include "../ns0/Constants.hpp"
#include "Constants.hpp"

namespace woodworking {

struct IWwUnitFlags_t {
  BindableMemberValue<bool> MachineOn;
  BindableMemberValue<bool> MachineInitialized;
  BindableMemberValue<bool> PowerPresent;
  BindableMemberValue<bool> AirPresent;
  BindableMemberValue<bool> DustChipSuction;
  BindableMemberValue<bool> Emergency;
  BindableMemberValue<bool> Safety;
  BindableMemberValue<bool> Calibrated;
  BindableMemberValue<bool> Remote;
  BindableMemberValue<bool> WorkpiecePresent;
  BindableMemberValue<bool> Moving;
  BindableMemberValue<bool> Error;
  BindableMemberValue<bool> Alarm;
  BindableMemberValue<bool> Warning;
  BindableMemberValue<bool> Hold;
  BindableMemberValue<bool> RecipeInRun;
  BindableMemberValue<bool> RecipeInSetup;
  BindableMemberValue<bool> RecipeInHold;
  BindableMemberValue<bool> ManualActivityRequired;
  BindableMemberValue<bool> LoadingEnabled;
  BindableMemberValue<bool> WaitUnload;
  BindableMemberValue<bool> WaitLoad;
  BindableMemberValue<bool> EnergySaving;
  BindableMemberValue<bool> ExternalEmergency;
  BindableMemberValue<bool> MaintenanceRequired;
  BindableMemberValue<bool> FeedRuns;
};
}  // namespace woodworking

REFL_TYPE(
  woodworking::IWwUnitFlags_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITFLAGSTYPE)))
REFL_FIELD(MachineOn)
REFL_FIELD(MachineInitialized)
REFL_FIELD(PowerPresent)
REFL_FIELD(
  AirPresent,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITFLAGSTYPE_AIRPRESENT)))
REFL_FIELD(
  DustChipSuction,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITFLAGSTYPE_DUSTCHIPSUCTION)))
REFL_FIELD(Emergency)
REFL_FIELD(
  Safety,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITFLAGSTYPE_SAFETY)))
REFL_FIELD(Calibrated)
REFL_FIELD(
  Remote,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITFLAGSTYPE_REMOTE)))
REFL_FIELD(
  WorkpiecePresent,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITFLAGSTYPE_WORKPIECESPRESENT)))
REFL_FIELD(
  Moving,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITFLAGSTYPE_MOVING)))
REFL_FIELD(Error)
REFL_FIELD(Alarm)
REFL_FIELD(Warning)
REFL_FIELD(
  Hold,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITFLAGSTYPE_HOLD)))
REFL_FIELD(RecipeInRun)
REFL_FIELD(
  RecipeInSetup,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITFLAGSTYPE_RECIPEINSETUP)))
REFL_FIELD(
  RecipeInHold,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITFLAGSTYPE_RECIPEINHOLD)))
REFL_FIELD(
  ManualActivityRequired,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITFLAGSTYPE_MANUALACTIVITYREQUIRED)))
REFL_FIELD(
  LoadingEnabled,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITFLAGSTYPE_LOADINGENABLED)))
REFL_FIELD(
  WaitUnload,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITFLAGSTYPE_WAITUNLOAD)))
REFL_FIELD(
  WaitLoad,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITFLAGSTYPE_WAITLOAD)))
REFL_FIELD(
  EnergySaving,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITFLAGSTYPE_ENERGYSAVING)))
REFL_FIELD(
  ExternalEmergency,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITFLAGSTYPE_EXTERNALEMERGENCY)))
REFL_FIELD(
  MaintenanceRequired,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITFLAGSTYPE_MAINTENANCEREQUIRED)))
REFL_FIELD(
  FeedRuns,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITFLAGSTYPE_FEEDRUNS)))
REFL_END
