/**
 * @file OpcUaEvent.hpp
 * @author Christian von Arnim
 * @date 2020-02-25
 * @copyright Copyright (c) 2020
 */
#pragma once
#include <open62541/server.h>

#include <Open62541Cpp/UA_NodeId.hpp>

/**
 * @brief Helper for OPC UA Evenets
 *
 * @tparam T An structure, which represents an OPC UA Object Type (which must be a Subtype of BaseEventype)
 */
template <typename T>
class OpcUaEvent {
 public:
  /// The data of the event. Can be changed before triggering.
  T Data;
  open62541Cpp::UA_NodeId EventNodeId;
  open62541Cpp::UA_NodeId Origin;
  /**
   * @brief Construct a new Opc Ua Event object, triggers the event
   *
   * The data is copied to the public variable Data and this public member is bind to the EventObject in the namespace.
   *
   * @param data The event data
   * @param pServer Pointer to UA_Server
   * @param origin Node, where the event should be triggered
   */
  OpcUaEvent(const T &data, UA_Server *pServer, open62541Cpp::UA_NodeId origin) : Data(data), pServer(pServer), nodesMaster(pServer), Origin(origin) {
    addEventToAddressSpace();
    Trigger();
  }
  /// \TODO add move constructor

  /**
   * @brief Trigger the event.
   *
   */
  void Trigger() {
    /// \todo ask for source node
    auto retValEvTrigger = UA_Server_triggerEvent(pServer, *EventNodeId.NodeId, *Origin.NodeId, NULL, UA_FALSE);
    if (retValEvTrigger != UA_STATUSCODE_GOOD) {
      std::cout << "Trigger of event failed. Reason: " << UA_StatusCode_name(retValEvTrigger) << std::endl;
    }
  }

  virtual ~OpcUaEvent() {
    if (UA_StatusCode retValNodeDelete = UA_Server_deleteNode(pServer, *EventNodeId.NodeId, true); retValNodeDelete != UA_STATUSCODE_GOOD) {
      std::cerr << "Could not delete event node: " << UA_StatusCode_name(retValNodeDelete) << std::endl;
    }
  }

 protected:
  UA_Server *pServer;
  NodesMaster nodesMaster;
  void addEventToAddressSpace() {
    // Get Event Type from reflection Information
    static_assert(
      refl::descriptor::has_attribute<UmatiServerLib::attribute::UaObjectType>(refl::reflect<T>()), "Event must have an UaObjectType-attribute with node ID.");
    auto objTypeAttr = refl::descriptor::get_attribute<UmatiServerLib::attribute::UaObjectType>(refl::reflect<T>());
    auto eventTypeNodeId = objTypeAttr.NodeId.UANodeId(pServer);

    UA_StatusCode retvalCreateEvent = UA_Server_createEvent(pServer, *eventTypeNodeId.NodeId, EventNodeId.NodeId);

    if (retvalCreateEvent != UA_STATUSCODE_GOOD) {
      std::cerr << "Could not create event: " << UA_StatusCode_name(retvalCreateEvent) << std::endl;
      throw std::runtime_error("Could not create event.");
    }

    UmatiServerLib::Bind::MembersRefl(Data, pServer, EventNodeId, nodesMaster);
  }
};
