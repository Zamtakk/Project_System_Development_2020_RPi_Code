#include "SocketServer.hpp"
#include "WebsocketBackend.hpp"

// Global static pointer used to ensure a single instance of the class.
SocketServer *SocketServer::_SocketServer = NULL;

// Public methods
SocketServer *SocketServer::Instance()
{
    if (!_SocketServer)
        _SocketServer = new SocketServer();
    return _SocketServer;
}

std::string SocketServer::GetMessage()
{
    if (!_Rx->empty())
    {
        std::string message = _Rx->front();
        _Rx->pop();
        return message;
    }
    else
    {
        return "";
    }
}

void SocketServer::SendMessage(std::string UNUSED(message))
{
    throw;
}

// Private Methods
SocketServer::SocketServer()
{
    _Rx = new std::queue<std::string>();
    _Tx = new std::queue<std::string>();

    SetRxTx(_Rx, _Tx);

    _WebsocketppThread = new std::thread(StartSocket);
}

SocketServer::~SocketServer()
{
    delete _Rx;
    delete _Tx;
    delete _WebsocketppThread;
}