#ifndef __LOG_WRAPPER_H__
#define __LOG_WRAPPER_H__

#include "common.h"
#include "Poco/Message.h"

using Poco::Message;

class LogWrapper
{
public:
    static void setup();

    static void teardown();

    static void mylog(Message::Priority prio, const char* file, int line, 
                      const char* funcname, const char * format, ...);
};

#define FATAL(format...) \
    if (NULL != g_logger) { \
        LogWrapper::mylog(Message::PRIO_FATAL, __FILE__, __LINE__, \
                          __FUNCTION__, format); \
    } \
    else ( void ) 0

#define CRITICAL(format...) \
    if (NULL != g_logger) { \
        LogWrapper::mylog(Message::PRIO_CRITICAL, __FILE__, __LINE__, \
                          __FUNCTION__, format); \
    } \
    else ( void ) 0

#define ERROR(format...) \
    if (NULL != g_logger) { \
        LogWrapper::mylog(Message::PRIO_ERROR, __FILE__, __LINE__, \
                          __FUNCTION__, format); \
    } \
    else ( void ) 0

#define WARNING(format...) \
    if (NULL != g_logger) { \
        LogWrapper::mylog(Message::PRIO_WARNING, __FILE__, __LINE__, \
                          __FUNCTION__, format); \
    } \
    else ( void ) 0

#define NOTICE(format...) \
    if (NULL != g_logger) { \
        LogWrapper::mylog(Message::PRIO_NOTICE, __FILE__, __LINE__, \
                          __FUNCTION__, format); \
    } \
    else ( void ) 0

#define INFORMATION(format...) \
    if (NULL != g_logger) { \
        LogWrapper::mylog(Message::PRIO_INFORMATION, __FILE__, __LINE__, \
                          __FUNCTION__, format); \
    } \
    else ( void ) 0

#define DEBUG(format...) \
    if (NULL != g_logger) { \
        LogWrapper::mylog(Message::PRIO_DEBUG, __FILE__, __LINE__, \
                          __FUNCTION__, format); \
    } \
    else ( void ) 0

#define TRACE(format...) \
    if (NULL != g_logger) { \
        LogWrapper::mylog(Message::PRIO_TRACE, __FILE__, __LINE__, \
                          __FUNCTION__, format); \
    } \
    else ( void ) 0


#endif
