#include "SimulatedMachineTool.hpp"

SimulatedMachineTool::SimulatedMachineTool()
{
  rnd.seed(reinterpret_cast<unsigned int>(this));
}

SimulatedMachineTool::~SimulatedMachineTool()
{
}
