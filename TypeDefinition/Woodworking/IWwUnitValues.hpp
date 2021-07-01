#pragma once
#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "../TypeDefinition.hpp"
#include "../ns0/Constants.hpp"
#include "../ns0/BaseObject.hpp"
#include "Constants.hpp"

namespace woodworking{

struct IWwUnitValues_t {
    BindableMemberValue<uint32_t> AxisOverride;
    BindableMemberValue<uint32_t> SpindleOverride;
    BindableMemberValue<double> FeedSpeed;
    BindableMemberValue<double> ActualCycle;
    BindableMemberValue<uint64_t> AbsoluteMachineOffTime;
    BindableMemberValue<uint64_t> AbsoluteStandbyTime;
    BindableMemberValue<uint64_t> RelativeStandbyTime;
    BindableMemberValue<uint64_t> AbsoluteReadyTime;
    BindableMemberValue<uint64_t> RelativeReadyTime;
    BindableMemberValue<uint64_t> AbsoluteWorkingTime;
    BindableMemberValue<uint64_t> RelativeWorkingTime;
    BindableMemberValue<uint64_t> AbsoluteErrorTime;
    BindableMemberValue<uint64_t> RelativeErrorTime;
    BindableMemberValue<uint64_t> AbsoluteMachineOnTime;
    BindableMemberValue<uint64_t> RelativeMachineOnTime;
    BindableMemberValue<uint64_t> AbsolutePowerPresentTime;
    BindableMemberValue<uint64_t> RelativePowerPresentTime;
    BindableMemberValue<uint64_t> AbsoluteProductionTime;
    BindableMemberValue<uint64_t> RelativeProductionTime;
    BindableMemberValue<uint64_t> AbsoluteProductionWithoutWorkpieceTime;
    BindableMemberValue<uint64_t> RelativeProductionWithoutWorkpieceTime;
    BindableMemberValue<uint64_t> AbsoluteProductionWaitWorkpieceTime;
    BindableMemberValue<uint64_t> RelativeProductionWaitWorkpieceTime;
    BindableMemberValue<uint64_t> AbsoluteRunsGood;
    BindableMemberValue<uint64_t> RelativeRunsGood;
    BindableMemberValue<uint64_t> AbsoluteRunsTotal;
    BindableMemberValue<uint64_t> RelativeRunsTotal;
    BindableMemberValue<uint64_t> AbsoluteRunsAborted;
    BindableMemberValue<uint64_t> RelativeRunsAborted;
    BindableMemberValue<uint64_t> AbsoluteLength;
    BindableMemberValue<uint64_t> RelativeLength;
    BindableMemberValue<uint64_t> AbsolutePiecesIn;
    BindableMemberValue<uint64_t> RelativePiecesIn;
    BindableMemberValue<uint64_t> AbsolutePiecesOut;
    BindableMemberValue<uint64_t> RelativePiecesOut;
};
}  // namespace woodworking

REFL_TYPE(
  woodworking::IWwUnitValues_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE))
)

REFL_FIELD(
    AxisOverride,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_AXISOVERRIDE))
)

REFL_FIELD(
    SpindleOverride,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_SPINDLEOVERRIDE))
)

REFL_FIELD(
    FeedSpeed,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_FEEDSPEED))
)

REFL_FIELD(
    ActualCycle,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_ACTUALCYCLE))
)

REFL_FIELD(
    AbsoluteMachineOffTime,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_ABSOLUTEMACHINEOFFTIME))
)

REFL_FIELD(
    AbsoluteStandbyTime,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_ABSOLUTESTANDBYTIME))
)

REFL_FIELD(
    RelativeStandbyTime,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_RELATIVESTANDBYTIME))
)

REFL_FIELD(
    AbsoluteReadyTime,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_ABSOLUTEREADYTIME))
)

REFL_FIELD(
    RelativeReadyTime,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_RELATIVEREADYTIME))
)

REFL_FIELD(
    AbsoluteWorkingTime,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_ABSOLUTEWORKINGTIME))
)

REFL_FIELD(
    RelativeWorkingTime,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_RELATIVEWORKINGTIME))
)

REFL_FIELD(
    AbsoluteErrorTime,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_ABSOLUTEERRORTIME))
)

REFL_FIELD(
    RelativeErrorTime,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_RELATIVEERRORTIME))
)

REFL_FIELD(
    AbsoluteMachineOnTime,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_ABSOLUTEMACHINEONTIME))
)

REFL_FIELD(
    RelativeMachineOnTime,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_RELATIVEMACHINEONTIME))
)

REFL_FIELD(
    AbsolutePowerPresentTime,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_ABSOLUTEPOWERPRESENTTIME))
)

REFL_FIELD(
    RelativePowerPresentTime,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_RELATIVEPOWERPRESENTTIME))
)

REFL_FIELD(
    AbsoluteProductionTime,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_ABSOLUTEPRODUCTIONTIME))
)

REFL_FIELD(
    RelativeProductionTime,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_RELATIVEPRODUCTIONTIME))
)

REFL_FIELD(
    AbsoluteProductionWithoutWorkpieceTime,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_ABSOLUTEPRODUCTIONWITHOUTWORKPIECETIME))
)

REFL_FIELD(
    RelativeProductionWithoutWorkpieceTime,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_RELATIVEPRODUCTIONWITHOUTWORKPIECETIME))
)

REFL_FIELD(
    AbsoluteProductionWaitWorkpieceTime,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_ABSOLUTEPRODUCTIONWAITWORKPIECETIME))
)

REFL_FIELD(
    RelativeProductionWaitWorkpieceTime,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_RELATIVEPRODUCTIONWAITWORKPIECETIME))
)

REFL_FIELD(
    AbsoluteRunsGood,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_ABSOLUTERUNSGOOD))
)

REFL_FIELD(
    RelativeRunsGood,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_RELATIVERUNSGOOD))
)

REFL_FIELD(
    AbsoluteRunsTotal,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_ABSOLUTERUNSTOTAL))
)

REFL_FIELD(
    RelativeRunsTotal,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_RELATIVERUNSTOTAL))
)

REFL_FIELD(
    AbsoluteRunsAborted,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_ABSOLUTERUNSABORTED))
)

REFL_FIELD(
    RelativeRunsAborted,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_RELATIVERUNSABORTED))
)

REFL_FIELD(
    AbsoluteLength,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_ABSOLUTELENGTH))
)

REFL_FIELD(
    RelativeLength,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_RELATIVELENGTH))
)

REFL_FIELD(
    AbsolutePiecesIn,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_ABSOLUTEPIECESIN))
)

REFL_FIELD(
    RelativePiecesIn,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_RELATIVEPIECESIN))
)

REFL_FIELD(
    AbsolutePiecesOut,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_ABSOLUTEPIECESOUT))
)

REFL_FIELD(
    RelativePiecesOut,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITVALUESTYPE_RELATIVEPIECESOUT))
)

REFL_END
