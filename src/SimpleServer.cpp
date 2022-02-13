#include "common.h"
#include "LogWrapper.h"
#include "SrvHandler.h"

#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/File.h"
#include "Poco/String.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/ParallelSocketAcceptor.h"

#include <iostream>

using Poco::Util::ServerApplication;
using Poco::Util::OptionSet;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;
using Poco::icompare;
using Poco::Net::ServerSocket;
using Poco::Net::ParallelSocketAcceptor;

Logger* g_logger(NULL);
int     g_log_qmax(0);
long    g_log_waitmax_ms(0);
long    g_log_bufsize(4096);

namespace simpleserver
{

class SimpleServer: public ServerApplication
{
public:
    SimpleServer();
    virtual ~SimpleServer();

    virtual void defineOptions(OptionSet &options);
    void initialize(Application &self);
    virtual int main(const std::vector<std::string> &args);
    void uninitialize();

private:
    void handleHelp(const std::string &name, const std::string &value);
    void displayHelp();
    void handleInput(const std::string &name, const std::string &value);
    bool findAppConfigFile(std::vector<std::string> &confnames);
    bool handleConfig();

    bool _helpRequested;
};

SimpleServer::SimpleServer():
    _helpRequested(false)
{
    g_logger = &logger();
}

SimpleServer::~SimpleServer()
{
}

void SimpleServer::defineOptions(OptionSet &options)
{
    ServerApplication::defineOptions(options);

    options.addOption(
        Option("help", "h", "display help information")
        .required(false)
        .repeatable(false)
        .callback(OptionCallback<SimpleServer>(
            this, &SimpleServer::handleHelp)
            )
        );

    options.addOption(
        Option("conf_path", "p", "set config file path")
        .required(false)
        .repeatable(false)
        .argument("string")
        .callback(OptionCallback<SimpleServer>(
            this, &SimpleServer::handleInput)
            )
        );
}

void SimpleServer::initialize(Application &self)
{
    ERROR("SimpleServer initialize");

    std::vector<std::string> configFiles;
    if (findAppConfigFile(configFiles))
    {
        for (std::vector<std::string>::iterator itf = configFiles.begin();
             itf != configFiles.end(); ++itf)
        {
            loadConfiguration(*itf);
        }

        ServerApplication::initialize(self);

        if (!handleConfig()) {
            throw Poco::InvalidArgumentException("config file error");
        }
    } else {
        ERROR("not found any config file in %s",
              config().getString("conf_path").c_str());
        ServerApplication::initialize(self);
    }

    for (std::vector<std::string>::iterator itf = configFiles.begin();
         itf != configFiles.end(); ++itf)
    {
        ERROR("load config file: %s", itf->c_str());
    }
}

void SimpleServer::uninitialize()
{
    LogWrapper::teardown();

    ServerApplication::uninitialize();
}

void SimpleServer::handleHelp(const std::string &name, const std::string &value)
{
    _helpRequested = true;
    displayHelp();
    stopOptionsProcessing();
}

void SimpleServer::displayHelp()
{
    HelpFormatter hf(options());    // Application::options()
    hf.setCommand(commandName());   // Application::commandName()
    hf.setUsage("OPTIONS");
    hf.setHeader(
        "SimpleServer is used to"
        "simulate a simple server."
        );
    hf.format(std::cout);
}

void SimpleServer::handleInput(const std::string &name, const std::string &value)
{
    config().setString(name, value);
}

bool SimpleServer::findAppConfigFile(std::vector<std::string> &confnames)
{
    if (!config().has("conf_path")) {
        //return false;
        std::string binPath = config().getString("application.path");
        std::string::size_type pos = binPath.rfind("/");
        std::string cfgPath = binPath.substr(0, pos);
        pos = cfgPath.rfind("/");
        cfgPath = cfgPath.substr(0, pos);
        cfgPath += "/conf";
        config().setString("conf_path", cfgPath);
    }

    std::string confname;
    std::string cmdpath = config().getString("conf_path");
    std::string::size_type pos = cmdpath.rfind("/");
    if (pos != cmdpath.length() - 1) {
        cmdpath += "/";
        config().setString("conf_path", cmdpath);
    }

    std::vector<std::string> names;
    Poco::File configPath(cmdpath);
    configPath.list(names);

    const short N = 3;
    const char *name[N] = {"properties", "xml", "ini"};
    std::vector<std::string> configtype(name, name + N);
    for (std::vector<std::string>::const_iterator itn = names.begin();
         itn != names.end(); ++itn)
    {
        pos = itn->rfind('.');
        if (pos == std::string::npos) {
            continue;
        }

        std::string ext = itn->substr(pos + 1);
        for (std::vector<std::string>::const_iterator ittp = configtype.begin();
             ittp != configtype.end(); ++ittp)
        {
            if (icompare(ext, *ittp) == 0) {
                confname = cmdpath + *itn;
                confnames.push_back(confname);
            }
        }
    }

    return !confnames.empty();
}

bool SimpleServer::handleConfig()
{
    g_log_qmax       = config().getUInt("log_queue_max"  , 1000 );
    g_log_waitmax_ms = config().getUInt("log_wait_max_ms", 60000);
    g_log_bufsize    = config().getUInt("log_buffer_size", 4096 );

    LogWrapper::setup();

    return true;
}

int SimpleServer::main(const std::vector<std::string> &args)
{
    if (_helpRequested) {
        return Application::EXIT_USAGE;
    }

    int port = config().getInt("port");
    ERROR("Listen Port: %d", port);

    ServerSocket serverSocket(port);
    SocketReactor reactor;
    ParallelSocketAcceptor<SrvHandler, SocketReactor> acceptor(serverSocket, reactor);
    ERROR("SimpleServer Starting ...");
    reactor.run();
    waitForTerminationRequest();
    ERROR("SimpleServer Stoped.");

    return Application::EXIT_OK;
}

}
POCO_SERVER_MAIN(simpleserver::SimpleServer);
