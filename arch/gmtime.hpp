
#include <ctime>

#if defined (__STDC_LIB_EXT1__)
#define UMATI_GMTIME(pTimeT, pTm) gmtime_s(pTimeT, pTm)
#elif defined(_WIN32)
#define UMATI_GMTIME(pTimeT, pTm) gmtime_s(pTm, pTimeT)
#elif (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 1) || defined(_XOPEN_SOURCE) || defined(_BSD_SOURCE) || defined(_SVID_SOURCE) || defined(_POSIX_SOURCE)
#define UMATI_GMTIME(pTimeT, pTm) gmtime_r(pTimeT, pTm)
#else
#error "No safe gmtime found"
#endif
