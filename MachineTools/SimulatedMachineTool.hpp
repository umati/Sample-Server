#pragma once

class SimulatedMachineTool
{
  public:
  // Should be called every second
  virtual void Simulate() = 0;
  virtual ~SimulatedMachineTool() = 0;
};
