///
/// \file setupEvents.hpp
/// \author Christian von Arnim
/// \date 14.01.2020
///

#pragma once

#include <string>
#include <open62541/server.h>

struct NotificationEvent_t {
  std::string Identifier;
  std::string Message;
  std::string SourceName;
  uint16_t Severity;
};
struct AlertCondition_t : public NotificationEvent_t{
  bool Retain;
  bool Enable = true;
};

UA_NodeId setupAlertConditionType(UA_Server *pServer, AlertCondition_t ev);
UA_NodeId setupNotificationEvent(UA_Server *pServer, NotificationEvent_t ev);
