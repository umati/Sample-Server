/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../../src_generated/namespace_di_generated.h"
#include "../../src_generated/namespace_ia_generated.h"
#include "../../src_generated/namespace_machinery_generated.h"
#include "../../src_generated/namespace_machinetool_generated.h"
#include "UAServer_Test.hpp"

class MTServer_Test : public UAServer_Test {
 protected:
  void SetUp() override {
    UAServer_Test::SetUp();
    namespace_di_generated(pServer);
    namespace_ia_generated(pServer);
    namespace_machinery_generated(pServer);
    namespace_machinetool_generated(pServer);
  }

  void TearDown() override { UAServer_Test::TearDown(); }
};
