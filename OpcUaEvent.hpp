/**
 * @file OpcUaEvent.hpp
 * @author Christian von Arnim
 * @date 2020-02-25
 * @copyright Copyright (c) 2020
 */

#include <open62541/server.h>
#include <Open62541Cpp/UA_NodeId.hpp>

template <typename T>
class OpcUaEvent
{
public:
    T Data;
    open62541Cpp::UA_NodeId EventNodeId;
    OpcUaEvent(const T &data, UA_Server *pServer) : Data(data), pServer(pServer), nodesMaster(pServer)
    {
        addEventToAddressSpace();
        Trigger();
    }
    /// \TODO add move constructor

    void Trigger()
    {
        /// \todo ask for source node
        UA_Server_triggerEvent(pServer, *EventNodeId.NodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER), NULL, UA_FALSE);
    }

    virtual ~OpcUaEvent()
    {
        if (UA_StatusCode retValNodeDelete = UA_Server_deleteNode(pServer, *EventNodeId.NodeId, true); retValNodeDelete != UA_STATUSCODE_GOOD)
        {
            std::cerr << "Could not delete event node: " << UA_StatusCode_name(retValNodeDelete) << std::endl;
        }
    }

protected:
    UA_Server *pServer;
    NodesMaster nodesMaster;
    void addEventToAddressSpace()
    {
        // Get Event Type from reflection Information
        static_assert(refl::descriptor::has_attribute<open62541Cpp::attribute::UaObjectType>(refl::reflect<T>()), "Event must have an UaObjectType-attribute with node ID.");
        auto objTypeAttr = refl::descriptor::get_attribute<open62541Cpp::attribute::UaObjectType>(refl::reflect<T>());
        auto eventTypeNodeId = objTypeAttr.NodeId.UANodeId(pServer);

        UA_StatusCode retvalCreateEvent = UA_Server_createEvent(
            pServer,
            *eventTypeNodeId.NodeId,
            EventNodeId.NodeId);

        if (retvalCreateEvent != UA_STATUSCODE_GOOD)
        {
            std::cerr << "Could not create event: " << UA_StatusCode_name(retvalCreateEvent) << std::endl;
            throw std::runtime_error("Could not create event.");
        }

        bindMembersRefl(Data, pServer, EventNodeId, nodesMaster);
        nodesMaster.SetCallbacks();
    }
};
