#ifndef __COMMON_H__
#define __COMMON_H__

#include "Poco/Logger.h"

using Poco::Logger;
extern Logger              * g_logger;
// max queue size of AsyncChannelEx
extern int                   g_log_qmax;
// max wait check time when log queue is full
extern long                  g_log_waitmax_ms;
// format buffer size for logging
extern long                  g_log_bufsize;

#endif
