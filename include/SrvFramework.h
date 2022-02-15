#ifndef __SRV_FRAMEWORK_H__
#define __SRV_FRAMEWORK_H__

namespace simpleserver
{

class SrvFramework
{
public:
    void onMessage(const void* pSender, char*& msg);
};

}
#endif

