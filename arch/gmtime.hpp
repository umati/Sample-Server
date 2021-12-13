/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2021 (c) Götz Görisch, VDW - Verein Deutscher Werkzeugmaschinenfabriken e.V.
 */

#include <ctime>

#if defined(__STDC_LIB_EXT1__)
#define UMATI_GMTIME(pTimeT, pTm) gmtime_s(pTimeT, pTm)
#elif defined(_WIN32)
#define UMATI_GMTIME(pTimeT, pTm) gmtime_s(pTm, pTimeT)
#elif (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 1) || defined(_XOPEN_SOURCE) || defined(_BSD_SOURCE) || defined(_SVID_SOURCE) || defined(_POSIX_SOURCE)
#define UMATI_GMTIME(pTimeT, pTm) gmtime_r(pTimeT, pTm)
#else
#error "No safe gmtime found"
#endif
