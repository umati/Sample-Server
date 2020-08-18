#pragma once
#include "FullMachineTool.hpp"

// Chang address space during runtime
class FullMachineToolDynamic : public FullMachineTool
{
  public:
  FullMachineToolDynamic(UA_Server *pServer);

  void Simulate() override;

  protected:
  // Allow skipping initialization in subclasses
  FullMachineToolDynamic(UA_Server *pServer, bool initialize);
};
