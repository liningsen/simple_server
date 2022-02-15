#include "ErrorCode.h"
#include "SrvHandler.h"
#include "LogWrapper.h"
#include "Poco/Exception.h"

#include "Poco/Delegate.h"
using Poco::delegate;

using Poco::Int32;
using Poco::UInt32;
using Poco::Exception;

namespace simpleserver
{

SrvHandler::SrvHandler(StreamSocket& socket, SocketReactor& reactor) :
    BUF_SIZE(6),
    PACK_HEADER_SIZE(4),
    _socket(socket),
    _reactor(reactor),
    _or(*this, &SrvHandler::onReadable),
    _ow(*this, &SrvHandler::onWriteable),
    _os(*this, &SrvHandler::onShutdown),
    _ot(*this, &SrvHandler::onTimeout),
    _oe(*this, &SrvHandler::onError)
{
    _buffer = new char[BUF_SIZE];
    _peerAddr = socket.peerAddress().toString();
    ERROR("connection from %s", _peerAddr.c_str());
    _socket.setNoDelay(true);
    _socket.setBlocking(false);
    _reactor.addEventHandler(_socket, _or);
    _reactor.addEventHandler(_socket, _ow);
    _reactor.addEventHandler(_socket, _os);
    _reactor.addEventHandler(_socket, _ot);
    _reactor.addEventHandler(_socket, _oe);
    _msgEvent += delegate(&_framework, &SrvFramework::onMessage);
}

SrvHandler::~SrvHandler()
{
    free(_buffer);
    _reactor.removeEventHandler(_socket, _or);
    _reactor.removeEventHandler(_socket, _ow);
    _reactor.removeEventHandler(_socket, _os);
    _reactor.removeEventHandler(_socket, _ot);
    _reactor.removeEventHandler(_socket, _oe);
    _msgEvent -= delegate(&_framework, &SrvFramework::onMessage);
}

void SrvHandler::onReadable(ReadableNotification* pNf)
{
     pNf->release();
     try
     {
         Int32 retCode = ERR_READABLE;
         if (OK != (retCode = readMessage())) {
            ERROR("readMessage fail %d", retCode);
            return;
         }
         ERROR("receive %d buf[%x]:%s", _packBodySize, _buffer, _buffer);

         // delegate to framework
         MsgEventArgs args = MsgEventArgs(_buffer,_packBodySize);
         _msgEvent.notify(this, args);
     }
     catch (Exception& exc)
     {
         _socket.shutdownSend();
         delete this;
     }
}

Int32 SrvHandler::readMessage()
{
    Int32 retCode = ERR_READ_MSG;
    if (OK != (retCode = readHeader())) {
        ERROR("readHeader fail %d", retCode);
        return retCode;
    }

    return readBody();
}

Int32 SrvHandler::readHeader()
{
    Int32 retCode = ERR_READ_HEADER;
    if (OK != (retCode = readN(PACK_HEADER_SIZE))) {
        ERROR("readN fail %d", retCode);
        return retCode;
    }
    return parseBodySize();
}

Int32 SrvHandler::parseBodySize()
{
    UInt32 nSize = *(UInt32*)(_buffer);
    _packBodySize = poco_ntoh_32(nSize);
    TRACE("nSize %d lSize %d", nSize, _packBodySize);
    if (_packBodySize < 0 || _packBodySize > BUF_SIZE) {
        return ERR_PARSE_BODY_SIZE;
    }
    return OK;
}

Int32 SrvHandler::readBody()
{
    return readN(_packBodySize);
}

Int32 SrvHandler::readN(Int32 n)
{
    Int32 retCode = ERR_READ_N;
    Int32 nread = 0;
    Int32 nleft = n;

    char* p = _buffer;
    while (nleft) {
        nread = _socket.receiveBytes(p, nleft);
        if (nread < 0) {
            if (POCO_EAGAIN == errno) {
                continue;
            }
            ERROR("receiveBytes errno %d", errno);
            _socket.shutdownSend();
            delete this;
            return errno;
        } else if (0 == nread) {
            ERROR("peer %s closed", _peerAddr.c_str());
            _socket.shutdownSend();
            delete this;
            return ERR_PEER_SHUTDOWN;
        } else {
            nleft -= nread;
            TRACE("total %d recv %d left %d buf %s", n, nread, nleft, string(p, nread).c_str());
            p += nread;
        }
    }
    return OK;
}

void SrvHandler::onWriteable(WritableNotification* pNf)
{
}

void SrvHandler::onShutdown(ShutdownNotification* pNf)
{
}

void SrvHandler::onTimeout(TimeoutNotification* pNf)
{
}

void SrvHandler::onError(ErrorNotification* pNf)
{
}

}
