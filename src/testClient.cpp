#include "Serializer.h"
#include "ReqRsp.pb.h"
#include "Poco/Types.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/StringTokenizer.h"
#include "Poco/NumberParser.h"
#include "Poco/Exception.h"
#include <string>
#include <iostream>

using Poco::Int64;
using Poco::UInt32;
using Poco::StringTokenizer;
using Poco::NumberParser;
using Poco::Net::SocketAddress;
using Poco::Net::StreamSocket;
using Poco::SyntaxException;
using std::string;
using std::cin;
using std::cout;
using std::endl;

int main (int argc, char** argv)
{
    SocketAddress sa("127.0.0.1", 7070);
    StreamSocket socket(sa);

    const UInt32 PACK_HEADER_SIZE = 4;
    string str;
    cout << "Input data to send:" << endl;
    // while (getline(cin, str)) {
    //     UInt32 lSize = str.size();;
    //     StringTokenizer st(str, " ", StringTokenizer::TOK_IGNORE_EMPTY | StringTokenizer::TOK_TRIM);
    //     if (st.count() > 1) {
    //         try {
    //             lSize = NumberParser::parse(st[0]);
    //             size_t pos = str.find(st[1]);
    //             str = str.substr(pos);
    //         } catch(SyntaxException& ex) {
    //             lSize = str.size();
    //         }
    //     }
    //     cout << "Expected pack size " << lSize << endl;
    //     UInt32 nSize = poco_hton_32(lSize);
    //     char* buf = new char[PACK_HEADER_SIZE + lSize];
    //     memset(buf, 0, PACK_HEADER_SIZE+lSize);
    //     memcpy(buf, &nSize, PACK_HEADER_SIZE);

    //     // simulate TCP incomplete packages
    //     lSize = str.size();
    //     cout << "Real pack size " << lSize << " buf " << str << endl;
    //     memcpy(buf+PACK_HEADER_SIZE, str.c_str(), lSize);
    //     int nsend = socket.sendBytes(buf, PACK_HEADER_SIZE+lSize);
    //     cout << nsend << " bytes has sent complete" << endl;
    //     free(buf);
    // }

    Int64 id;
    string inStr;
    string name;
    int val;
    const int BUF_SIZE = 4096;
    char buf[BUF_SIZE] = {0};
    int len = 0;
    while (true) {
        cout << "input id: ";
        cin  >> inStr;
        if (inStr == "x") {
            break;
        }
        cout << "input name: ";
        cin  >> name;
        id = NumberParser::parse64(inStr);
        simpleserver::Request q;
        q.set_id(id);
        q.set_name(name);

        while (true) {
            cout << "input value: ";
            cin  >> inStr;
            if (inStr == "x") {
                break;
            }
            try {
                val = NumberParser::parse(inStr);
            } catch (Poco::SyntaxException) {
                continue;
            }
            q.add_values(val);
        }

        simpleserver::serialize(q, buf+PACK_HEADER_SIZE, len);
        UInt32 nSize = poco_hton_32(len);
        memcpy(buf, &nSize, PACK_HEADER_SIZE);
        int nsend = socket.sendBytes(buf, PACK_HEADER_SIZE+len);
        cout << nsend << " bytes has sent complete" << endl;

    }
    return 0;
}
