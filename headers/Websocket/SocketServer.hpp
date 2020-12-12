#ifndef SOCKETSERVER_HPP
#define SOCKETSERVER_HPP

#include <string>
#include <queue>
#include <thread>
#include <mutex>

#include "WebsocketBackend.hpp"
#include "SocketTypes.hpp"

using std::string;
using std::queue;
using std::mutex;
using std::thread;
using std::vector;

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
    bool isMessageValid(string message);
    void sendDeviceNotRegistered(string uuid);
    void sendIncorrectMessageFormat(string uuid);

    DeviceRegistration *getRegisteredDevice(string uuid);

    static SocketServer *socketServer;

    queue<SocketMessage> messageQueue;
    mutex messageLock;

    queue<WebsocketMessage> websocketppRxQueue;
    mutex websocketppRxLock;

    thread *websocketppThread;
    thread *processRxThread;

    vector<DeviceRegistration> registeredDevices;
};
#endif