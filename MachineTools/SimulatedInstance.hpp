#pragma once
#include <random>

class SimulatedInstance
{
  public:
  SimulatedInstance();
  // Should be called every second
  virtual void Simulate() = 0;
  virtual ~SimulatedInstance() = 0;

  protected:
  // Seeded random generator
  std::mt19937 rnd;
};
