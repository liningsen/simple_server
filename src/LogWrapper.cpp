#include "LogWrapper.h"
#include "AsyncChannelEx.h"

#include "Poco/LoggingRegistry.h"
#include "Poco/Channel.h"
#include "Poco/Logger.h"
#include "Poco/AutoPtr.h"
#include "Poco/SharedPtr.h"
#include <cstdarg>

using Poco::LoggingRegistry;
using Poco::Channel;
using Poco::AsyncChannelEx;
using Poco::Logger;
using Poco::AutoPtr;
using Poco::SharedPtr;
using Poco::ReferenceCounter;
using Poco::ReleaseArrayPolicy;



void LogWrapper::setup()
{
    Channel * pChannel = 
        LoggingRegistry::defaultRegistry().channelForName("c1");
    AutoPtr<AsyncChannelEx> pAsync(new AsyncChannelEx(pChannel));
    // AsyncChannelEx refCount becomes 1

    // create child node AppLogRoot.test
    g_logger = &Logger::get("AppLogRoot.test");
    g_logger->setChannel(pAsync);
    // AsyncChannelEx refCount adds to 2
}
// AsyncChannelEx refCount minus to 1 
// cause the AutoPtr object pAsyn destructs
// the only remaining comes from AppLogRoot.test->_pChannel

void LogWrapper::teardown()
{
    Logger::destroy("AppLogRoot.test");
    // AsyncChannelEx refCount 1 --> 0
    // g_logger->getChannel()->close();
    g_logger = NULL;
}

void LogWrapper::mylog(Message::Priority prio, const char* file, int line, 
                       const char* funcname, const char * format, ...)
{
    // funcname, format... -->> text
    int multiple = 1, curbuflen = 0, part1 = 0, part2 = 0, remain = 0;
    SharedPtr<char, ReferenceCounter, ReleaseArrayPolicy<char> > cur;

    while (true) {
        curbuflen = multiple * g_log_bufsize;
        char * buf = new char[curbuflen];
        cur = buf;

        part1 = snprintf(cur, curbuflen, "[%s]_", funcname);
        remain = curbuflen - part1;
        if (part1 >= 0 && part1 < curbuflen && remain > 0) {
            va_list ap;
            va_start(ap, format);
            part2 = vsnprintf(cur + part1, remain, format, ap);
            va_end(ap);
            if (part2 >= 0 && part2 < remain) {
                *(cur + part1 + part2) = '\0';
                break;
            }
        }

        do {
            multiple *= 2;
        } while (multiple * g_log_bufsize < part1 + part2);
    }

    std::string text(cur);
    g_logger->log(Message(g_logger->name(), text, prio, file, line));
}

