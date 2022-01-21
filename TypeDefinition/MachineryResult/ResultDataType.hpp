/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Sebastian Friedl, ISW University of Stuttgart (for VDMA e.V.)
 */

#pragma once
#include "Constants.hpp"
#include "ResultMetaDataType.hpp"

namespace machinery_result {
    struct ResultDataType_t {
        UA_ResultMetaDataType resultMetaData;
        //size_t resultContentSize;
        //UA_Variant *resultContent;
    };
}  // namespace machinery_result


REFL_TYPE(machinery_result::ResultDataType_t, UmatiServerLib::attribute::UaDataType(&UA_TYPES_MACHINERY_RESULT[UA_TYPES_MACHINERY_RESULT_RESULTDATATYPE]))
REFL_FIELD(resultMetaData)
//REFL_FIELD(resultContent)
REFL_END