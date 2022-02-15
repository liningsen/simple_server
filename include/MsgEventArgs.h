#ifndef __MSG_EVENT_ARGS_H__
#define __MSG_EVENT_ARGS_H__

#include "Poco/Types.h"

using Poco::Int32;

namespace simpleserver
{

class MsgEventArgs
{
public:
    MsgEventArgs(char*& buf, Int32 size);
    ~MsgEventArgs();

    void getBuffer(char*& buf, Int32& size);

private:
    char* _buffer;
    Int32 _bufSize;
};

}

#endif
