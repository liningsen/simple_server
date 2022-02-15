#include "SrvFramework.h"
#include "LogWrapper.h"

namespace simpleserver
{

void SrvFramework::onMessage(const void* pSender, char*& msg)
{
    TRACE("msg[%x]:%s", msg, msg);
}

}
