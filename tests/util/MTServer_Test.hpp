#pragma once
#include "UAServer_Test.hpp"

#include "../../src_generated/namespace_machinetool_generated.h"
#include "../../src_generated/namespace_machinery_generated.h"
#include "../../src_generated/namespace_di_generated.h"
#include "../../src_generated/namespace_ia_generated.h"

class MTServer_Test : public UAServer_Test
{
protected:
  void SetUp() override
  {
    UAServer_Test::SetUp();
    namespace_di_generated(pServer);
    namespace_ia_generated(pServer);
    namespace_machinery_generated(pServer);
    namespace_machinetool_generated(pServer);
  }

  void TearDown() override
  {
    UAServer_Test::TearDown();
  }
};
