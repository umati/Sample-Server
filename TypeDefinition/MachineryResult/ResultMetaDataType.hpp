/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Sebastian Friedl, ISW University of Stuttgart (for VDMA e.V.)
 */

#pragma once
#include "Constants.hpp"

namespace machinery_result {
    struct ResultMetaDataType_t {
        UA_String resultId;
        UA_Boolean *hasTransferableDataOnFile;
        UA_Boolean *isPartial;
        UA_Boolean *isSimulated;
        UA_Int32 *resultState;
        UA_String *taskId;
        UA_String *partId;
        UA_String *externalRecipeId;
        UA_String *internalRecipeId;
        UA_String *productId;
        UA_String *externalConfigurationId;
        UA_String *internalConfigurationId;
        UA_String *jobId;
        UA_DateTime *creationTime;
        UA_ProcessingTimesDataType *processingTimes;
        size_t resultUriSize;
        UA_Variant *resultUri;
        UA_ResultEvaluationEnum *resultEvaluation;
        UA_Int32 *resultEvaluationCode;
        UA_LocalizedText *resultEvaluationDetails;
        size_t fileFormatSize;
        UA_String *fileFormat;
    };
}  // namespace machineTool

REFL_TYPE(machinery_result::ResultMetaDataType_t, UmatiServerLib::attribute::UaDataType(&UA_TYPES_MACHINERY_RESULT[UA_TYPES_MACHINERY_RESULT_RESULTMETADATATYPE]))
REFL_FIELD(resultId)
REFL_END