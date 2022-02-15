#ifndef __SRV_FRAMEWORK_H__
#define __SRV_FRAMEWORK_H__

#include "MsgEventArgs.h"

namespace simpleserver
{

class SrvFramework
{
public:
    void onMessage(const void* pSender, MsgEventArgs& args);
};

}
#endif

