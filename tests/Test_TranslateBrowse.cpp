/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#include <gtest/gtest.h>

#include <Open62541Cpp/UA_QualifiedName.hpp>

#include "../TypeDefinition/MachineTool/Constants.hpp"
#include "../UmatiServerLib/Util.hpp"
#include "util/MTServer_Test.hpp"

class TranslateBrowse : public MTServer_Test {};

TEST_F(TranslateBrowse, translateBrowsePathToolListType) {
  open62541Cpp::UA_QualifiedName qnToolList(nsFromUri(pServer, constants::NsMachineToolUri), "ToolListType");
  UA_BrowsePath p;
  UA_RelativePathElement el;
  p.startingNode = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE);
  p.relativePath.elementsSize = 1;
  p.relativePath.elements = &el;
  el.isInverse = UA_FALSE;
  el.referenceTypeId = UA_NODEID_NULL;
  el.targetName = *qnToolList.QualifiedName;
  UA_BrowsePathResult ret = UA_Server_translateBrowsePathToNodeIds(pServer, &p);
  ASSERT_EQ(ret.statusCode, UA_STATUSCODE_GOOD);
  ASSERT_EQ(ret.targetsSize, 1);
  ASSERT_EQ(ret.targets[0].targetId.nodeId.identifier.numeric, UA_MACHINETOOLID_TOOLLISTTYPE);
  UA_BrowsePathResult_clear(&ret);
}
