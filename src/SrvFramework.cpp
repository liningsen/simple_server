#include "SrvFramework.h"
#include "LogWrapper.h"
#include "Serializer.h"
#include "ReqRsp.pb.h"

namespace simpleserver
{

void SrvFramework::onMessage(const void* pSender, MsgEventArgs& args)
{
    char* buf = NULL;
    Int32 bufSize = 0;
    args.getBuffer(buf, bufSize);
    TRACE("msg[%x] size %d :%s", buf, bufSize, buf);

    Request rq;
    Int32 retCode = deSerialize(buf, bufSize, rq);
    TRACE("request id %ld name %s", rq.id(), rq.name().c_str());
    for (int i = 0; i < rq.values_size(); ++i) {
        TRACE("value[%d]: %d", i, rq.values(i));
    }
}

}
