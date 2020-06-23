#include "SimulatedMachineTool.hpp"
#include <cstdint>

SimulatedMachineTool::SimulatedMachineTool()
{
  rnd.seed(static_cast<unsigned int>(reinterpret_cast<std::uintptr_t>(this)));
}

SimulatedMachineTool::~SimulatedMachineTool()
{
}
