#include "SimulatedInstance.hpp"

#include <cstdint>

SimulatedInstance::SimulatedInstance() { rnd.seed(static_cast<unsigned int>(reinterpret_cast<std::uintptr_t>(this))); }

SimulatedInstance::~SimulatedInstance() {}
