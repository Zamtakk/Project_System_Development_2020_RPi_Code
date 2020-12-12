#ifndef SOCKETSERVER_HPP
#define SOCKETSERVER_HPP

#include <string>
#include <queue>
#include <thread>
#include <mutex>

#include "WebsocketBackend.hpp"
#include "SocketTypes.hpp"

class SocketServer
{
public:
    static SocketServer *Instance();
    SocketMessage GetMessage();
    bool SendMessage(SocketMessage message);

private:
    SocketServer();
    ~SocketServer();

    void parseIncommingMessages();
    DeviceRegistration *getRegisteredDevice(std::string uuid);

    static SocketServer *socketServer;

    std::queue<SocketMessage> messageQueue;
    std::mutex messageLock;

    std::queue<WebsocketMessage> websocketppRxQueue;
    std::mutex websocketppRxLock;

    std::thread *websocketppThread;
    std::thread *processRxThread;

    std::vector<DeviceRegistration> registeredDevices;
};
#endif