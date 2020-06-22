#pragma once
#include <random>

class SimulatedMachineTool
{
  public:
  SimulatedMachineTool();
  // Should be called every second
  virtual void Simulate() = 0;
  virtual ~SimulatedMachineTool() = 0;

  protected:
  // Seeded random generator
  std::mt19937 rnd;
};
