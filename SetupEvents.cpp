///
/// \file setupEvents.cpp
/// \author Christian von Arnim
/// \date 14.01.2020
///

#include <open62541Cpp/UA_String.hpp>
#include <atomic>
#include <mutex>
#include <chrono>
#include <thread>
#include "BindValue.hpp"
#include "NodesMaster.hpp"
#include "NodeValue.hpp"
#include <functional>
#include <cstdint>
#include "src_generated/umati_nodeids.h"
#include "src_generated/iswexample.h"
#include "src_generated/namespace_umati_generated.h"
#include "src_generated/namespace_iswexample_generated.h"
#include <iostream>
#include "SetupEvents.hpp"

UA_NodeId setupAlertConditionType(UA_Server *pServer, AlertCondition_t ev)
{
  UA_NodeId eventNodeId;
  UA_StatusCode retval = UA_Server_createCondition(
      pServer,
      UA_NODEID_NULL /*UA_NODEID_NUMERIC(0, UA_NS0ID_SYSTEMCONDITIONCLASSTYPE)*/,
      UA_NODEID_NUMERIC(2, UA_UMATIID_ALERTCONDITIONTYPE),
      UA_QUALIFIEDNAME_ALLOC(0, "SystemConditionClassType"),
      UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER),
      UA_NODEID_NULL, &eventNodeId);

  if(retval != UA_STATUSCODE_GOOD){
    std::cerr << "Could not create event: " << retval << std::endl;
    return UA_NODEID_NULL;
  }

  // Set this at the beginning as it overrides the message
  {
    UA_Variant value;
    UA_Boolean *pBoolEnableState = UA_Boolean_new();
    *pBoolEnableState = ev.Enable;
    UA_QualifiedName enabledStateField = UA_QUALIFIEDNAME_ALLOC(0,"EnabledState");
    UA_QualifiedName enabledStateIdField = UA_QUALIFIEDNAME_ALLOC(0,"Id");
    UA_Variant_setScalar(&value, pBoolEnableState, &UA_TYPES[UA_TYPES_BOOLEAN]);
    retval = UA_Server_setConditionVariableFieldProperty(
        pServer,
        eventNodeId,
        &value,
        enabledStateField,
        enabledStateIdField
    );
  }

  // Setting the Time is required or else the event will not show up in UAExpert!
  UA_DateTime eventTime = UA_DateTime_now();
  UA_Server_writeObjectProperty_scalar(
      pServer,
      eventNodeId,
      UA_QUALIFIEDNAME_ALLOC(0, "Time"),
      &eventTime,
      &UA_TYPES[UA_TYPES_DATETIME]
  );
  {
    open62541Cpp::UA_String strIdentifier(ev.Identifier);
    UA_Variant *val = UA_Variant_new();
    UA_Variant_clear(val);
    UA_Variant_setScalarCopy(val, strIdentifier.String, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_writeObjectProperty(
        pServer,
        eventNodeId,
        UA_QUALIFIEDNAME_ALLOC(2, "Identifier"),
        *val
    );
  }

  {
    open62541Cpp::UA_String strIdentifier(ev.SourceName);
    UA_Variant *val = UA_Variant_new();
    UA_Variant_clear(val);
    UA_Variant_setScalarCopy(val, strIdentifier.String, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_writeObjectProperty(
        pServer,
        eventNodeId,
        UA_QUALIFIEDNAME_ALLOC(0, "SourceName"),
        *val
    );
  }

  {
    UA_Variant *val = UA_Variant_new();
    UA_Variant_clear(val);
    UA_Variant_setScalarCopy(val, &ev.Severity, &UA_TYPES[UA_TYPES_UINT16]);
    UA_Server_writeObjectProperty(
        pServer,
        eventNodeId,
        UA_QUALIFIEDNAME_ALLOC(0, "Severity"),
        *val
    );
  }

  UA_Boolean * pBoolRetain = UA_Boolean_new();
  *pBoolRetain = ev.Retain;
  UA_Server_writeObjectProperty_scalar(pServer, eventNodeId, UA_QUALIFIEDNAME_ALLOC(0, "Retain"),
      pBoolRetain, &UA_TYPES[UA_TYPES_BOOLEAN]);


  {
    open62541Cpp::UA_String strMessage(ev.Message);
    UA_Variant *val = UA_Variant_new();
    UA_LocalizedText ltxtMessage = UA_LOCALIZEDTEXT_ALLOC("en-en", "");
    ltxtMessage.text = *strMessage.String;
    UA_Variant_clear(val);
    UA_Variant_setScalarCopy(val, &ltxtMessage, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
    UA_Server_writeObjectProperty(
        pServer,
        eventNodeId,
        UA_QUALIFIEDNAME_ALLOC(0, "Message"),
        *val
    );
  }

  return eventNodeId;
}
UA_NodeId setupNotificationEvent(UA_Server *pServer, NotificationEvent_t ev){
  UA_NodeId eventNodeId;
  UA_StatusCode retval = UA_Server_createEvent(
      pServer,
      UA_NODEID_NUMERIC(2, UA_UMATIID_NOTIFICATIONEVENTTYPE),
      &eventNodeId);

  if(retval != UA_STATUSCODE_GOOD){
    std::cerr << "Could not create event: " << retval << std::endl;
    return UA_NODEID_NULL;
  }

  // Setting the Time is required or else the event will not show up in UAExpert!
  UA_DateTime eventTime = UA_DateTime_now();
  UA_Server_writeObjectProperty_scalar(
      pServer,
      eventNodeId,
      UA_QUALIFIEDNAME_ALLOC(0, "Time"),
      &eventTime,
      &UA_TYPES[UA_TYPES_DATETIME]
      );
  {
    open62541Cpp::UA_String strIdentifier(ev.Identifier);
    UA_Variant *val = UA_Variant_new();
    UA_Variant_clear(val);
    UA_Variant_setScalarCopy(val, strIdentifier.String, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_writeObjectProperty(
        pServer,
        eventNodeId,
        UA_QUALIFIEDNAME_ALLOC(2, "Identifier"),
      *val
    );
  }

  {
    open62541Cpp::UA_String strIdentifier(ev.SourceName);
    UA_Variant *val = UA_Variant_new();
    UA_Variant_clear(val);
    UA_Variant_setScalarCopy(val, strIdentifier.String, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_writeObjectProperty(
        pServer,
        eventNodeId,
        UA_QUALIFIEDNAME_ALLOC(0, "SourceName"),
        *val
    );
  }

  {
    open62541Cpp::UA_String strMessage(ev.Message);
    UA_Variant *val = UA_Variant_new();
    UA_LocalizedText ltxtMessage = UA_LOCALIZEDTEXT_ALLOC("en-en", "");
    ltxtMessage.text = *strMessage.String;
    UA_Variant_clear(val);
    UA_Variant_setScalarCopy(val, &ltxtMessage, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
    UA_Server_writeObjectProperty(
        pServer,
        eventNodeId,
        UA_QUALIFIEDNAME_ALLOC(0, "Message"),
        *val
    );
  }

  {
    UA_Variant *val = UA_Variant_new();
    UA_Variant_clear(val);
    UA_Variant_setScalarCopy(val, &ev.Severity, &UA_TYPES[UA_TYPES_UINT16]);
    UA_Server_writeObjectProperty(
        pServer,
        eventNodeId,
        UA_QUALIFIEDNAME_ALLOC(0, "Severity"),
        *val
    );
  }

  return eventNodeId;
}
