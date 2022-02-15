#ifndef __SRV_HANDLER_H__
#define __SRV_HANDLER_H__

#include "MsgEventArgs.h"
#include "SrvFramework.h"
#include "Poco/Observer.h"
#include "Poco/BasicEvent.h"
#include "Poco/Net/SocketNotification.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketReactor.h"
#include <string>

using Poco::Int32;
using Poco::Observer;
using Poco::BasicEvent;
using Poco::Net::StreamSocket;
using Poco::Net::SocketReactor;
using Poco::Net::ReadableNotification;
using Poco::Net::WritableNotification;
using Poco::Net::TimeoutNotification;
using Poco::Net::ErrorNotification;
using Poco::Net::ShutdownNotification;
using std::string;

namespace simpleserver
{

class SrvHandler {
public:
    SrvHandler(StreamSocket& socket, SocketReactor& reactor);
    ~SrvHandler();

private:
    void onReadable(ReadableNotification* pNf);
    void onWriteable(WritableNotification* pNf);
    void onShutdown(ShutdownNotification* pNf);
    void onTimeout(TimeoutNotification* pNf);
    void onError(ErrorNotification* pNf);

    Int32 readMessage();
    Int32 readHeader();
    Int32 parseBodySize();
    Int32 readBody();
    Int32 readN(Int32 n);

    const Int32 BUF_SIZE;
    const Int32 PACK_HEADER_SIZE;

    string _peerAddr;
    StreamSocket _socket;
    SocketReactor& _reactor;
    Observer<SrvHandler, ReadableNotification> _or;
    Observer<SrvHandler, WritableNotification> _ow;
    Observer<SrvHandler, ShutdownNotification> _os;
    Observer<SrvHandler, TimeoutNotification> _ot;
    Observer<SrvHandler, ErrorNotification> _oe;
    char* _buffer;
    Int32 _packBodySize;
    BasicEvent<MsgEventArgs> _msgEvent;
    SrvFramework _framework;
};
}

#endif
