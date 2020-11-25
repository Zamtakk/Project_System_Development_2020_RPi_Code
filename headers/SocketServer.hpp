#ifndef SOCKETSERVER_HPP
#define SOCKETSERVER_HPP

#include <string>
#include <queue>
#include <thread>

class SocketServer
{
public:
    static SocketServer* Instance();
    std::string GetMessage();
    void SendMessage(std::string message);

private:
    static SocketServer* _SocketServer;

    std::queue<std::string>* _Rx;
    std::queue<std::string>* _Tx;

    std::thread* _WebsocketppThread;

    SocketServer();
    ~SocketServer();
};

#endif