#include "SrvFramework.h"
#include "LogWrapper.h"

namespace simpleserver
{

void SrvFramework::onMessage(const void* pSender, MsgEventArgs& args)
{
    char* buf = NULL;
    Int32 bufSize = 0;
    args.getBuffer(buf, bufSize);
    TRACE("msg[%x] size %d :%s", buf, bufSize, buf);
}

}
