#include "MsgEventArgs.h"

namespace simpleserver
{

MsgEventArgs::MsgEventArgs(char*& buf, Int32 size) :
    _buffer(buf),
    _bufSize(size)
{
}

MsgEventArgs::~MsgEventArgs()
{
}

void MsgEventArgs::getBuffer(char*& buf, Int32& size)
{
    buf = _buffer;
    size = _bufSize;
}

}
