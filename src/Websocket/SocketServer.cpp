#include "Websocket/SocketServer.hpp"
#include "Websocket/WebsocketTypes.hpp"
#include "json.hpp"

using json = nlohmann::json;

using std::lock_guard;
using std::mutex;
using std::string;
using std::thread;
using std::to_string;
using std::vector;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;

// Global static pointer used to ensure a single instance of the class.
SocketServer *SocketServer::socketServer = NULL;

// Public methods
SocketServer *SocketServer::Instance()
{
    if (!socketServer)
        socketServer = new SocketServer();
    return socketServer;
}

SocketMessage SocketServer::GetMessage()
{
    const lock_guard<mutex> lock(messageLock);
    if (!messageQueue.empty())
    {
        SocketMessage newMessage = messageQueue.front();
        messageQueue.pop();
        return newMessage;
    }
    else
    {
        SocketMessage msg = {
            .UUID = "",
            .Message = ""};
        return msg;
    }
}

bool SocketServer::SendMessage(SocketMessage message)
{
    DeviceRegistration *devicePtr = getRegisteredDevice(message.UUID);
    if (devicePtr != nullptr)
    {
        WebsocketMessage packet;
        packet.Handle = devicePtr->ConnectionHandle;
        packet.MessagePointer = devicePtr->MessagePointer;
        packet.MessagePointer->set_payload(message.Message);
        SendWebsocketppMessage(packet);
        return true;
    }
    return false;
}

// Private Methods
SocketServer::SocketServer()
{
    SetQueueAndLock(&websocketppRxQueue, &websocketppRxLock);

    websocketppThread = new thread(StartSocket);
    processRxThread = new thread(&SocketServer::parseIncommingMessages, this);
}

SocketServer::~SocketServer()
{
    delete websocketppThread;
    delete processRxThread;
}

void SocketServer::parseIncommingMessages()
{
    WebsocketMessage websocketppMessage;

    while (1)
    {
        sleep_for(milliseconds(500));

        const lock_guard<mutex> _websocketppLock(websocketppRxLock);
        if (!websocketppRxQueue.empty())
        {
            websocketppMessage = websocketppRxQueue.front();
            websocketppRxQueue.pop();
        }
        else
        {
            _websocketppLock.~lock_guard();
            continue;
        }
        _websocketppLock.~lock_guard();

        if (websocketppMessage.MessagePointer->get_payload().find("registration") != string::npos)
        {
            DeviceRegistration newDevice{
                .UUID = "1234",
                .ConnectionHandle = websocketppMessage.Handle,
                .MessagePointer = websocketppMessage.MessagePointer};
            registeredDevices.push_back(newDevice);
            SocketMessage newMessage{
                .UUID = "1234",
                .Message = "Hello Test"};
            SendMessage(newMessage);
        }
        else if (websocketppMessage.MessagePointer->get_payload().find("heartbeat") != string::npos)
        {
            SocketMessage newMessage{
                .UUID = "1234",
                .Message = "Heartbeat response"};
            SendMessage(newMessage);
        }
        else
        {
            const lock_guard<mutex> _messageLock(messageLock);
            SocketMessage newSocketMessage{
                .UUID = "n/a",
                .Message = "n/a"};
            messageQueue.push(newSocketMessage);
            _messageLock.~lock_guard();
        }
    }
}

bool SocketServer::isMessageValid(string message)
{
    json jsonMessage = json::parse(message);

    if (!jsonMessage.contains("UUID") || !jsonMessage.contains("Type") || !jsonMessage.contains("Commands"))
        return false;
    return true;
}

void SocketServer::sendDeviceNotRegistered(string uuid)
{
    SocketMessage newMessage{
        .UUID = uuid,
        .Message = "{\"error\":" + to_string(1) + ",\"description\":\"Device not registered!\"}"};
    SendMessage(newMessage);
}

void SocketServer::sendIncorrectMessageFormat(string uuid)
{
}

DeviceRegistration *SocketServer::getRegisteredDevice(string uuid)
{
    for (vector<DeviceRegistration>::iterator i = registeredDevices.begin(); i < registeredDevices.end(); i++)
    {
        if (i->UUID.find(uuid) != string::npos)
        {
            return &(*i);
        }
    }
    return nullptr;
}