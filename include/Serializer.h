#ifndef __SERIALIZER_H__
#define __SERIALIZER_H__

#include "ErrorCode.h"
#include "Poco/Types.h"

using Poco::Int32;

namespace simpleserver
{

template<class T>
Int32 serialize(const T& message, char* buf, Int32& retlen)
{
    retlen = message.ByteSize();
    if (!message.SerializeToArray(buf, retlen)) {
        return ERR_SERIALIZE;
    }

    return OK;
}

template<class T>
Int32 deSerialize(const void * buf, const Int32 len, T& ret_message)
{
    if (NULL == buf || len <= 0) {
        return ERR_DESERIALIZE_INPUT;
    }

    if (!ret_message.ParseFromArray(buf, len)) {
        return ERR_DESERIALIZE;
    }

    return OK;
}

}

#endif
