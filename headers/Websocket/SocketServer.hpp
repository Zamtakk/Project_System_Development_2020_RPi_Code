#ifndef SOCKETSERVER_HPP
#define SOCKETSERVER_HPP

#include <string>
#include <queue>
#include <thread>
#include <mutex>

#include "WebsocketBackend.hpp"
#include "SocketTypes.hpp"

using std::mutex;
using std::queue;
using std::string;
using std::thread;
using std::vector;

class SocketServer
{
public:
    static SocketServer *Instance();
    string GetMessage();
    bool SendMessage(string uuid, string jsonStringMessage);

private:
    SocketServer();
    ~SocketServer();

    void parseIncomingMessages();
    bool isDeviceRegistered(string uuid);
    bool isMessageValid(string message);
    void sendDeviceNotRegistered(WebsocketMessage websocketppMessage);
    void sendIncorrectMessageFormat(WebsocketMessage websocketppMessage);
    void registerDevice(WebsocketMessage websocketppMessage);
    void heartbeat();

    DeviceRegistration *getRegisteredDevice(string uuid);

    static SocketServer *socketServer;

    queue<string> messageQueue;
    mutex messageLock;

    queue<WebsocketMessage> websocketppRxQueue;
    mutex websocketppRxLock;

    queue<string> heartbeatQueue;
    mutex heartbeatLock;

    thread *websocketppThread;
    thread *processRxThread;
    thread *heartbeatThread;

    vector<DeviceRegistration> registeredDevices;
};
#endif